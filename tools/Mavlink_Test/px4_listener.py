#!/usr/bin/env python3
"""Simple MAVLink receiver for PX4 SITL.

Listens on UDP 14540 (PX4's onboard/API link), prints a few key messages,
and shows a per-message-type rate summary every few seconds.

Usage:
    pip install pymavlink
    python3 px4_listener.py            # key messages + rate summary
    python3 px4_listener.py --all      # print every message
    python3 px4_listener.py --port 14550   # use the GCS port instead

Note: only one program can bind a given UDP port. Close MAVSDK scripts
(also port 14540) before running this, or pass --port 14550 and close QGC.
"""
import argparse
import math
import time
from collections import Counter

from pymavlink import mavutil

KEY_TYPES = {"HEARTBEAT", "ATTITUDE", "GLOBAL_POSITION_INT", "GPS_RAW_INT",
             "SYS_STATUS", "BATTERY_STATUS", "STATUSTEXT", "VFR_HUD"}


def fmt(msg):
    t = msg.get_type()
    if t == "HEARTBEAT":
        armed = bool(msg.base_mode & mavutil.mavlink.MAV_MODE_FLAG_SAFETY_ARMED)
        return (f"HEARTBEAT armed={armed} custom_mode={msg.custom_mode} "
                f"system_status={msg.system_status}")
    if t == "ATTITUDE":
        return (f"ATTITUDE roll={math.degrees(msg.roll):7.2f} "
                f"pitch={math.degrees(msg.pitch):7.2f} "
                f"yaw={math.degrees(msg.yaw):7.2f} deg")
    if t == "GLOBAL_POSITION_INT":
        return (f"GLOBAL_POSITION_INT lat={msg.lat / 1e7:.6f} lon={msg.lon / 1e7:.6f} "
                f"alt={msg.alt / 1000:.1f} m rel_alt={msg.relative_alt / 1000:.1f} m "
                f"vx={msg.vx / 100:.2f} vy={msg.vy / 100:.2f} vz={msg.vz / 100:.2f} m/s")
    if t == "GPS_RAW_INT":
        return (f"GPS_RAW_INT fix_type={msg.fix_type} "
                f"satellites={msg.satellites_visible}")
    if t == "SYS_STATUS":
        return (f"SYS_STATUS battery={msg.voltage_battery / 1000:.2f} V "
                f"remaining={msg.battery_remaining}%")
    if t == "STATUSTEXT":
        return f"STATUSTEXT severity={msg.severity} text={msg.text!r}"
    if t == "VFR_HUD":
        return (f"VFR_HUD airspeed={msg.airspeed:.1f} groundspeed={msg.groundspeed:.1f} "
                f"alt={msg.alt:.1f} climb={msg.climb:.2f}")
    return str(msg)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--port", type=int, default=14540)
    ap.add_argument("--all", action="store_true", help="print every message")
    ap.add_argument("--interval", type=float, default=5.0,
                    help="seconds between rate summaries")
    args = ap.parse_args()

    conn = mavutil.mavlink_connection(f"udpin:0.0.0.0:{args.port}")
    print(f"Waiting for heartbeat on UDP {args.port} ...")
    conn.wait_heartbeat()
    print(f"Connected: system {conn.target_system}, component {conn.target_component}\n")

    counts = Counter()
    window_start = time.time()
    last_print = {}  # throttle noisy messages to ~2 Hz on screen

    try:
        while True:
            msg = conn.recv_match(blocking=True, timeout=1)
            now = time.time()
            if msg is not None and msg.get_type() != "BAD_DATA":
                t = msg.get_type()
                counts[t] += 1
                if args.all or t in KEY_TYPES:
                    if args.all or now - last_print.get(t, 0) >= 0.5 or t in ("HEARTBEAT", "STATUSTEXT"):
                        print(fmt(msg))
                        last_print[t] = now

            elapsed = now - window_start
            if elapsed >= args.interval:
                print(f"\n--- message rates over {elapsed:.1f} s ---")
                for name, n in counts.most_common():
                    print(f"  {name:28s} {n / elapsed:6.1f} Hz")
                print()
                counts.clear()
                window_start = now
    except KeyboardInterrupt:
        print("\nStopped.")


if __name__ == "__main__":
    main()