#!/usr/bin/env python3
"""cFS MAVLink dashboard: enables TO_LAB output, decodes MAVLINK_APP packets, shows them in Tk."""
import socket
import struct
import threading
import time
import tkinter as tk
import tkinter.font as tkfont
from tkinter import ttk

# ---- Configuration --------------------------------------------------------
LISTEN_ADDR = ("0.0.0.0", 2234)         # where TO_LAB sends telemetry
CI_ADDR = ("127.0.0.1", 1234)           # CI_LAB command port
TO_LAB_DEST_IP = "127.0.0.1"
TO_CMD_MID, ENABLE_FC = 0x1880, 0x06    # TO_LAB "enable output" command
HK_MID, NAV_MID = 0x08B4, 0x08B5        # MAVLINK_APP telemetry message IDs
HEADER_LEN = 16                         # CCSDS primary (6) + cFE telemetry secondary (10)

# Seconds without any packet before the dashboard reports DISCONNECTED.
STALE_TIMEOUT_S = 2.0

SC_CMD_MID = 0x18A9                     # SC command MID
SC_START_RTS_FC, SC_ENABLE_RTS_FC = 4, 7
MISSION_RTS = 3                         # RTS table holding START_MISSION
GPS_FAILURE_RTS = 4                     # RTS table holding GPS failure command
GPS_RESTORE_RTS = 5                     # RTS table holding GPS restore command

# ---- Packet layouts (must match the C structs) ----------------------------
HK_FMT = "<BBBBBBBbffI"                 # 20 bytes
HK_FIELDS = (
    "command_counter", "command_error_counter", "armed", "main_mode",
    "sub_mode", "gps_fix_type", "gps_satellites", "battery_remaining",
    "battery_voltage", "battery_current", "heartbeat_count"
)

NAV_FMT = "<ffffffiiihhhH"              # 44 bytes
NAV_FIELDS = (
    "roll", "pitch", "yaw", "roll_rate", "pitch_rate", "yaw_rate",
    "latitude", "longitude", "relative_altitude", "vx", "vy", "vz",
    "heading"
)
NAV_SCALE = {
    "latitude": 1e7,
    "longitude": 1e7,
    "relative_altitude": 1000,
    "vx": 100,
    "vy": 100,
    "vz": 100,
    "heading": 100
}

# ---- Shared state ---------------------------------------------------------
telemetry = {k: 0 for k in HK_FIELDS + NAV_FIELDS}
# last_rx: time.monotonic() of the most recent packet (0 = nothing received yet)
# bind_error: message string if the telemetry socket could not be bound
telemetry.update(last_rx=0.0, last_mid=0, packet_count=0, bind_error=None)
lock = threading.Lock()
running = True


def enable_telemetry(dest_ip):
    """Send TO_LAB_ENABLE_OUTPUT through CI_LAB."""
    ip = dest_ip.encode()[:15].ljust(16, b"\x00")
    packet = struct.pack(">HHH", TO_CMD_MID, 0xC000, 2 + len(ip) - 1)
    packet += struct.pack(">BB", ENABLE_FC & 0x7F, 0) + ip

    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
        s.sendto(packet, CI_ADDR)


def send_cmd(mid, fc, payload=b""):
    """Build a cFS command packet and send it through CI_LAB."""
    body = struct.pack(">HHHB", mid, 0xC000, len(payload) + 1, fc & 0x7F)

    checksum = 0xFF
    for b in body + payload:
        checksum ^= b

    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
        s.sendto(body + bytes([checksum]) + payload, CI_ADDR)


def decode(fields, fmt, payload, scale=()):
    size = struct.calcsize(fmt)
    if len(payload) < size:
        return

    values = struct.unpack(fmt, payload[:size])

    with lock:
        for key, value in zip(fields, values):
            telemetry[key] = value / scale[key] if key in scale else value


def handle_packet(data):
    if len(data) < 6:
        return

    mid = struct.unpack(">H", data[:2])[0]

    with lock:
        telemetry["last_mid"] = mid
        telemetry["packet_count"] += 1
        telemetry["last_rx"] = time.monotonic()

    if len(data) >= HEADER_LEN:
        if mid == HK_MID:
            decode(HK_FIELDS, HK_FMT, data[HEADER_LEN:])
        elif mid == NAV_MID:
            decode(NAV_FIELDS, NAV_FMT, data[HEADER_LEN:], NAV_SCALE)


def receive_loop():
    global running

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    try:
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind(LISTEN_ADDR)
        sock.settimeout(1.0)
    except OSError as exc:
        msg = f"Could not bind UDP {LISTEN_ADDR[0]}:{LISTEN_ADDR[1]}: {exc}"
        print(msg)
        with lock:
            telemetry["bind_error"] = msg
        sock.close()
        return

    while running:
        try:
            data, _ = sock.recvfrom(4096)
            handle_packet(data)
        except socket.timeout:
            continue
        except OSError:
            break

    sock.close()


# ---- GUI layout -----------------------------------------------------------
def _armed(v):
    return "ARMED" if v else "DISARMED"


SECTIONS = [
    ("Flight Status", 0, 0, [
        ("Armed", "armed", _armed),
        ("Main Mode", "main_mode", "{}"),
        ("Sub Mode", "sub_mode", "{}"),
        ("Heartbeats", "heartbeat_count", "{}")
    ]),
    ("GPS / Position", 0, 1, [
        ("Fix Type", "gps_fix_type", "{}"),
        ("Satellites", "gps_satellites", "{}"),
        ("Latitude", "latitude", "{:.2f}°"),
        ("Longitude", "longitude", "{:.2f}°"),
        ("Relative Altitude", "relative_altitude", "{:.1f} m"),
        ("Heading", "heading", "{:.2f}°")
    ]),
    ("Battery", 1, 0, [
        ("Remaining", "battery_remaining", "{}%"),
        ("Voltage", "battery_voltage", "{:.2f} V")
    ]),
    ("Attitude", 1, 1, [
        ("Roll", "roll", "{:.2f} rad"),
        ("Pitch", "pitch", "{:.2f} rad"),
        ("Yaw", "yaw", "{:.2f} rad")
    ]),
    ("Angular Rates", 2, 0, [
        ("Roll Rate", "roll_rate", "{:.2f} rad/s"),
        ("Pitch Rate", "pitch_rate", "{:.2f} rad/s"),
        ("Yaw Rate", "yaw_rate", "{:.2f} rad/s")
    ]),
    ("Velocity", 2, 1, [
        ("Vx", "vx", "{:.2f} m/s"),
        ("Vy", "vy", "{:.2f} m/s"),
        ("Vz", "vz", "{:.2f} m/s")
    ])
]


class Dashboard:
    def __init__(self, root):
        self.root = root
        root.title("cFS MAVLink Telemetry Dashboard")
        root.geometry("1000x760")
        root.minsize(850, 680)
        root.protocol("WM_DELETE_WINDOW", self.close)

        self.count_before = 0

        style = ttk.Style()
        try:
            style.theme_use("clam")
        except tk.TclError:
            pass

        # Derive fonts from the platform's default font so the family is valid.
        # Keep references on self so they aren't garbage collected.
        base = tkfont.nametofont("TkDefaultFont")
        self.title_font = base.copy()
        self.title_font.configure(size=18, weight="bold")
        self.section_font = base.copy()
        self.section_font.configure(size=11, weight="bold")
        self.value_font = base.copy()
        self.value_font.configure(size=12, weight="bold")

        style.configure("Title.TLabel", font=self.title_font)
        style.configure("Section.TLabelframe.Label", font=self.section_font)
        style.configure("Value.TLabel", font=self.value_font)

        top = ttk.Frame(root)
        top.pack(fill="x", padx=15, pady=(12, 5))

        ttk.Label(
            top,
            text="cFS MAVLink Telemetry Dashboard",
            style="Title.TLabel"
        ).pack(side="left")

        self.conn = ttk.Label(
            top, text="● DISCONNECTED", style="Value.TLabel", foreground="red"
        )
        self.conn.pack(side="right")

        main = ttk.Frame(root)
        main.pack(fill="both", expand=True, padx=15, pady=5)

        for i in range(3):
            main.rowconfigure(i, weight=1)
        main.columnconfigure((0, 1), weight=1)

        self.fields = []

        for title, row, col, rows in SECTIONS:
            frame = ttk.LabelFrame(
                main,
                text=title,
                style="Section.TLabelframe"
            )
            frame.grid(
                row=row,
                column=col,
                sticky="nsew",
                pady=5,
                padx=(0, 7) if col == 0 else (7, 0)
            )
            frame.columnconfigure(1, weight=1)

            for r, (label, key, fmt) in enumerate(rows):
                ttk.Label(frame, text=label + ":").grid(
                    row=r, column=0, sticky="w", padx=12, pady=5
                )

                value = ttk.Label(frame, text="--", style="Value.TLabel")
                value.grid(row=r, column=1, sticky="e", padx=12, pady=5)
                self.fields.append((value, key, fmt))

        # ---- Mission and fault-injection controls -------------------------
        cmds = ttk.LabelFrame(
            root,
            text="Commands",
            style="Section.TLabelframe"
        )
        cmds.pack(fill="x", padx=15, pady=5)

        ttk.Button(
            cmds,
            text="Start Loaded Mission",
            command=self.start_mission
        ).pack(side="left", padx=8, pady=8)

        ttk.Button(
            cmds,
            text="Inject GPS Failure",
            command=self.gps_failure
        ).pack(side="left", padx=8, pady=8)

        ttk.Button(
            cmds,
            text="Restore GPS",
            command=self.restore_gps
        ).pack(side="left", padx=8, pady=8)

        self.cmd_status = ttk.Label(cmds, text="")
        self.cmd_status.pack(side="left", padx=8)

        bottom = ttk.Frame(root)
        bottom.pack(fill="x", padx=15, pady=(2, 10))

        self.packets = ttk.Label(bottom, text="Packets: 0")
        self.packets.pack(side="left")

        self.last_mid = ttk.Label(bottom, text="Last MID: --")
        self.last_mid.pack(side="right")

        # Shows socket errors (e.g. port already in use) in the GUI.
        self.error = ttk.Label(bottom, text="", foreground="red")
        self.error.pack(side="left", padx=15)

        self.refresh()

    def refresh(self):
        with lock:
            d = dict(telemetry)

        have_data = d["last_rx"] > 0
        alive = have_data and (time.monotonic() - d["last_rx"]) < STALE_TIMEOUT_S

        if d["bind_error"]:
            self.conn.config(text="● NO SOCKET", foreground="red")
            self.error.config(text=d["bind_error"])
        elif alive:
            self.conn.config(text="● CONNECTED", foreground="green")
            self.error.config(text="")
        else:
            self.conn.config(text="● DISCONNECTED", foreground="red")
            self.error.config(text="")

        # Show "--" until the first packet arrives so real zeros are
        # distinguishable from "no data yet". After a link loss the last
        # values stay visible, but the indicator above turns red.
        for label, key, fmt in self.fields:
            if not have_data:
                label.config(text="--")
            elif callable(fmt):
                label.config(text=fmt(d[key]))
            else:
                label.config(text=fmt.format(d[key]))

        self.packets.config(text=f"Packets: {d['packet_count']}")
        self.last_mid.config(
            text=f"Last MID: 0x{d['last_mid']:04X}"
            if d["last_mid"] else "Last MID: --"
        )

        self.root.after(100, self.refresh)

    def start_rts(self, rts_num, label):
        """Enable the selected SC RTS, then start it after a short delay."""
        with lock:
            self.count_before = telemetry["command_counter"]

        payload = struct.pack("<HH", rts_num, 0)
        try:
            send_cmd(SC_CMD_MID, SC_ENABLE_RTS_FC, payload)

            self.root.after(
                300,
                lambda: send_cmd(SC_CMD_MID, SC_START_RTS_FC, payload)
            )

            self.cmd_status.config(text=f"{label}: enabling RTS {rts_num}...")
            self.root.after(0)
        except OSError as exc:
            self.cmd_status.config(text=f"{label} failed to send: {exc}")

    def start_mission(self):
        self.start_rts(MISSION_RTS, "Start Loaded Mission")

    def gps_failure(self):
        """Enable and start RTS 4, which contains the GPS failure command."""
        self.start_rts(GPS_FAILURE_RTS, "Inject GPS Failure")

    def restore_gps(self):
        """Enable and start RTS 5, which contains the GPS restore command."""
        self.start_rts(GPS_RESTORE_RTS, "Restore GPS")

    def close(self):
        global running
        running = False
        self.root.destroy()


def main():
    print("Enabling TO_LAB telemetry...")
    try:
        enable_telemetry(TO_LAB_DEST_IP)
    except OSError as exc:
        print(f"Could not send TO_LAB enable command: {exc}")

    print(f"Listening on UDP {LISTEN_ADDR[0]}:{LISTEN_ADDR[1]}")
    threading.Thread(target=receive_loop, daemon=True).start()

    root = tk.Tk()
    Dashboard(root)
    root.mainloop()


if __name__ == "__main__":
    main()