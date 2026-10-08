#!/usr/bin/env python3
"""cFS MAVLink dashboard: enables TO_LAB output, decodes MAVLINK_APP packets, shows them in Tk."""
import socket
import struct
import threading
import tkinter as tk
from tkinter import ttk

# ---- Configuration --------------------------------------------------------
LISTEN_ADDR = ("0.0.0.0", 2234)         # where TO_LAB sends telemetry
CI_ADDR = ("127.0.0.1", 1234)           # CI_LAB command port
TO_LAB_DEST_IP = "127.0.0.1"
TO_CMD_MID, ENABLE_FC = 0x1880, 0x06    # TO_LAB "enable output" command
HK_MID, NAV_MID = 0x08B4, 0x08B5        # MAVLINK_APP telemetry message IDs
HEADER_LEN = 16                         # CCSDS primary (6) + cFE telemetry secondary (10)
CMD_MID = 0x18B2                        # 0x1800 + MAVLINK_APP_MISSION_CMD_TOPICID (confirmed from cFS event)
START_MISSION_FC = 4                    # MAVLINK_APP_START_MISSION_CC

# ---- Packet layouts (must match the C structs) ----------------------------
HK_FMT = "<BBBBBBBbffI"                 # 20 bytes
HK_FIELDS = ("command_counter", "command_error_counter", "armed", "main_mode", "sub_mode",
             "gps_fix_type", "gps_satellites", "battery_remaining", "battery_voltage",
             "battery_current", "heartbeat_count")

NAV_FMT = "<ffffffiiihhhH"              # 44 bytes
NAV_FIELDS = ("roll", "pitch", "yaw", "roll_rate", "pitch_rate", "yaw_rate", "latitude",
              "longitude", "relative_altitude", "vx", "vy", "vz", "heading")
NAV_SCALE = {"latitude": 1e7, "longitude": 1e7, "relative_altitude": 1000,
             "vx": 100, "vy": 100, "vz": 100, "heading": 100}  # mm, cm/s, centideg -> SI

# ---- Shared state ---------------------------------------------------------
telemetry = {k: 0 for k in HK_FIELDS + NAV_FIELDS}
telemetry.update(connected=False, last_mid=0, packet_count=0)
lock = threading.Lock()
running = True


def enable_telemetry(dest_ip):
    """Send TO_LAB_ENABLE_OUTPUT through CI_LAB."""
    ip = dest_ip.encode()[:15].ljust(16, b"\x00")
    packet = struct.pack(">HHH", TO_CMD_MID, 0xC000, 2 + len(ip) - 1)
    packet += struct.pack(">BB", ENABLE_FC & 0x7F, 0) + ip
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
        s.sendto(packet, CI_ADDR)


def send_command(fc):
    """Send a no-payload cFS command to MAVLINK_APP through CI_LAB."""
    packet = struct.pack(">HHHBB", CMD_MID, 0xC000, 1, fc & 0x7F, 0)
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
        s.sendto(packet, CI_ADDR)


def decode(fields, fmt, payload, scale=()):
    size = struct.calcsize(fmt)
    if len(payload) < size:
        return
    values = struct.unpack(fmt, payload[:size])
    with lock:
        for key, value in zip(fields, values):
            telemetry[key] = value / scale[key] if key in scale else value
        telemetry["connected"] = True


def handle_packet(data):
    if len(data) < 6:
        return
    mid = struct.unpack(">H", data[:2])[0]
    with lock:
        telemetry["last_mid"] = mid
        telemetry["packet_count"] += 1
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
    except OSError:
        running = False
        return
    while running:
        try:
            handle_packet(sock.recvfrom(4096)[0])
        except socket.timeout:
            continue
        except OSError:
            break
    sock.close()


# ---- GUI layout: (title, grid row, grid col, [(label, key, format)]) ------
def _armed(v):
    return "ARMED" if v else "DISARMED"


SECTIONS = [
    ("Flight Status", 0, 0, [
        ("Armed", "armed", _armed), ("Main Mode", "main_mode", "{}"),
        ("Sub Mode", "sub_mode", "{}"), ("Heartbeats", "heartbeat_count", "{}"),
        ("Command Count", "command_counter", "{}"), ("Command Errors", "command_error_counter", "{}")]),
    ("GPS / Position", 0, 1, [
        ("Fix Type", "gps_fix_type", "{}"), ("Satellites", "gps_satellites", "{}"),
        ("Latitude", "latitude", "{:.7f}°"), ("Longitude", "longitude", "{:.7f}°"),
        ("Relative Altitude", "relative_altitude", "{:.3f} m"), ("Heading", "heading", "{:.2f}°")]),
    ("Battery", 1, 0, [
        ("Remaining", "battery_remaining", "{}%"), ("Voltage", "battery_voltage", "{:.2f} V"),
        ("Current", "battery_current", "{:.2f} A")]),
    ("Attitude", 1, 1, [
        ("Roll", "roll", "{:.6f} rad"), ("Pitch", "pitch", "{:.6f} rad"), ("Yaw", "yaw", "{:.6f} rad")]),
    ("Angular Rates", 2, 0, [
        ("Roll Rate", "roll_rate", "{:.6f} rad/s"), ("Pitch Rate", "pitch_rate", "{:.6f} rad/s"),
        ("Yaw Rate", "yaw_rate", "{:.6f} rad/s")]),
    ("Velocity", 2, 1, [
        ("Vx", "vx", "{:.2f} m/s"), ("Vy", "vy", "{:.2f} m/s"), ("Vz", "vz", "{:.2f} m/s")]),
]


class Dashboard:
    def __init__(self, root):
        self.root = root
        root.title("cFS MAVLink Telemetry Dashboard")
        root.geometry("1000x720")
        root.minsize(850, 600)
        root.protocol("WM_DELETE_WINDOW", self.close)

        style = ttk.Style()
        try:
            style.theme_use("clam")
        except tk.TclError:
            pass
        style.configure("Title.TLabel", font=("TkDefaultFont", 18, "bold"))
        style.configure("Section.TLabelframe.Label", font=("TkDefaultFont", 11, "bold"))
        style.configure("Value.TLabel", font=("TkDefaultFont", 12, "bold"))

        top = ttk.Frame(root)
        top.pack(fill="x", padx=15, pady=(12, 5))
        ttk.Label(top, text="cFS MAVLink Telemetry Dashboard", style="Title.TLabel").pack(side="left")
        self.conn = ttk.Label(top, text="● DISCONNECTED", style="Value.TLabel")
        self.conn.pack(side="right")

        main = ttk.Frame(root)
        main.pack(fill="both", expand=True, padx=15, pady=5)
        for i in range(3):
            main.rowconfigure(i, weight=1)
        main.columnconfigure((0, 1), weight=1)

        self.fields = []  # (value label, key, format)
        for title, row, col, rows in SECTIONS:
            frame = ttk.LabelFrame(main, text=title, style="Section.TLabelframe")
            frame.grid(row=row, column=col, sticky="nsew", pady=5, padx=(0, 7) if col == 0 else (7, 0))
            frame.columnconfigure(1, weight=1)
            for r, (label, key, fmt) in enumerate(rows):
                ttk.Label(frame, text=label + ":").grid(row=r, column=0, sticky="w", padx=12, pady=5)
                value = ttk.Label(frame, text="--", style="Value.TLabel")
                value.grid(row=r, column=1, sticky="e", padx=12, pady=5)
                self.fields.append((value, key, fmt))

        cmds = ttk.LabelFrame(root, text="Mission", style="Section.TLabelframe")
        cmds.pack(fill="x", padx=15, pady=5)
        ttk.Button(cmds, text="Start Loaded Mission", command=self.start_mission).pack(side="left", padx=8, pady=8)
        self.cmd_status = ttk.Label(cmds, text="")
        self.cmd_status.pack(side="left", padx=8)

        bottom = ttk.Frame(root)
        bottom.pack(fill="x", padx=15, pady=(2, 10))
        self.packets = ttk.Label(bottom, text="Packets: 0")
        self.packets.pack(side="left")
        self.last_mid = ttk.Label(bottom, text="Last MID: --")
        self.last_mid.pack(side="right")

        self.refresh()

    def refresh(self):
        with lock:
            d = dict(telemetry)
        self.conn.config(text="● CONNECTED" if d["connected"] else "● DISCONNECTED")
        for label, key, fmt in self.fields:
            label.config(text=fmt(d[key]) if callable(fmt) else fmt.format(d[key]))
        self.packets.config(text=f"Packets: {d['packet_count']}")
        self.last_mid.config(text=f"Last MID: 0x{d['last_mid']:04X}" if d["last_mid"] else "Last MID: --")
        self.root.after(100, self.refresh)

    def start_mission(self):
        with lock:
            self.count_before = telemetry["command_counter"]
        send_command(START_MISSION_FC)
        self.cmd_status.config(text="Sent START_MISSION...")
        self.root.after(1500, self.check_command)

    def check_command(self):
        """Command Count is only updated in HK packets (about 1 Hz), so check after a delay."""
        with lock:
            now = telemetry["command_counter"]
        ok = now != self.count_before
        self.cmd_status.config(text="cFS accepted the command" if ok else "No response from cFS (check CMD_MID)")

    def close(self):
        global running
        running = False
        self.root.destroy()


def main():
    print("Enabling TO_LAB telemetry...")
    enable_telemetry(TO_LAB_DEST_IP)
    print(f"Listening on UDP {LISTEN_ADDR[0]}:{LISTEN_ADDR[1]}")
    threading.Thread(target=receive_loop, daemon=True).start()
    root = tk.Tk()
    Dashboard(root)
    root.mainloop()


if __name__ == "__main__":
    main()