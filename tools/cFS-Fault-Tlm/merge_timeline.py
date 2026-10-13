#!/usr/bin/env python3
"""
Merge cFS DS events and the PX4 ULog into ONE timeline on PX4's boot clock.

usage:
  python3 merge_timeline.py --ds ~/cFS/build-native_std/exe/cpu1/cf/tline*.dat \
                            --ulog ~/PX4-Autopilot/build/px4_sitl_default/rootfs/log   (file or dir -> newest .ulg)
                            [--csv out.csv] [--pre 2] [--post 15]

needs:  pip install pyulog        (read_ds_timeline.py must sit in the same folder)

Clock: ULog timestamps are microseconds since PX4 boot = MAVLink time_boot_ms * 1000,
so cFS events that carry px4_ms and PX4 log entries share one axis. cFS events
without px4_ms are placed by the median (px4_ms - cFE time) offset and marked '~'.

Zero point (t=0): the 'GPS_FAIL sent' event (or TL_INJECT_SENT packet) on the cFS side.
PX4 side reports only changes (transitions), never steady state:
  - GPS topic (vehicle_gnss / sensor_gnss) sample gaps and fix_type changes
  - vehicle_status nav_state, arming_state, failsafe
  - every bool in failsafe_flags and estimator_status_flags that flips near the fault
  - PX4 logged messages (the text PX4 prints, e.g. 'Failsafe activated')
"""
import argparse, glob, os, sys, statistics
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from read_ds_timeline import parse_file

NAV_STATE = {0: "MANUAL", 1: "ALTCTL", 2: "POSCTL", 3: "AUTO_MISSION", 4: "AUTO_LOITER",
             5: "AUTO_RTL", 12: "DESCEND", 14: "OFFBOARD", 15: "STAB",
             17: "AUTO_TAKEOFF", 18: "AUTO_LAND"}
ARMING = {1: "DISARMED", 2: "ARMED"}
GPS_GAP_MS = 500   # sensor_gps normally publishes at 5-10 Hz
LOG_LVL = {0: "EMERG", 1: "ALERT", 2: "CRIT", 3: "ERR", 4: "WARN", 5: "NOTICE", 6: "INFO", 7: "DEBUG"}

# ------------------------------------------------------------------ cFS side
def cfs_events(files):
    rows = []
    for f in files:
        rows += parse_file(f)
    rows = [r for r in rows if r["src"] == "TL" or "MAVLINK_APP" in r["what"] or r["what"].startswith("SC ")]
    rows.sort(key=lambda r: r["cfe"])
    offs = [r["px4"] - r["cfe"] * 1000 for r in rows if r["px4"] is not None]
    off = statistics.median(offs) if offs else None
    out = []
    for r in rows:
        if r["px4"] is not None:
            out.append(dict(t=float(r["px4"]), est=False, side="cFS", what=r["what"], key=r["code"]))
        elif off is not None:
            out.append(dict(t=r["cfe"] * 1000 + off, est=True, side="cFS", what=r["what"], key=r["code"]))
    return out

# ------------------------------------------------------------------ PX4 side
def get(ulog, name):
    try:
        return ulog.get_dataset(name).data
    except Exception:
        return None

def transitions(ts_us, vals, fmt):
    """yield (t_ms, old, new) at each value change"""
    prev = None
    for t, v in zip(ts_us, vals):
        v = int(v) if hasattr(v, "item") or isinstance(v, (int, float)) else v
        if prev is not None and v != prev:
            yield t / 1000.0, prev, v
        prev = v

def px4_events(path, t_lo, t_hi):
    from pyulog import ULog
    ul = ULog(path)
    ev = []
    lo_us, hi_us = t_lo * 1000, t_hi * 1000

    def add(t, what, key=None):
        ev.append(dict(t=t, est=False, side="PX4", what=what, key=key))

    # GPS: PX4 1.18 logs it as sensor_gnss / vehicle_gnss (older: sensor_gps). Use the busiest one.
    cands = [(n, get(ul, n)) for n in ("vehicle_gnss", "sensor_gnss", "sensor_gps")]
    cands = [(n, d) for n, d in cands if d is not None and len(d["timestamp"]) > 2]
    if cands:
        gname, g = max(cands, key=lambda c: len(c[1]["timestamp"]))
        ts = g["timestamp"]
        if "fix_type" in g:
            for t, a, b in transitions(ts, g["fix_type"], None):
                add(t, f"{gname} fix_type {a} -> {b}", "gps_fix")
        # injection stops the publisher (no fix_type=0 sample ever appears) -> look for gaps
        dts = [(ts[i] - ts[i - 1]) / 1000.0 for i in range(1, len(ts))]
        thr = max(GPS_GAP_MS, 5 * statistics.median(dts))
        for i, gap in enumerate(dts, 1):
            if gap > thr:
                add(ts[i - 1] / 1000.0, f"{gname} last sample before {gap:.0f} ms gap", "gps_stop")
                add(ts[i] / 1000.0, f"{gname} samples resume after {gap:.0f} ms gap", "gps_resume")
        print(f"[px4] GPS source: {gname} ({len(ts)} samples, gap threshold {thr:.0f} ms)", file=sys.stderr)
    else:
        print("[px4] no GPS topic found (vehicle_gnss/sensor_gnss/sensor_gps)", file=sys.stderr)

    # EKF fusing GNSS position (publishes only on new data, so also shows fused flag flips)
    e = get(ul, "estimator_aid_src_gnss_pos")
    if e is not None and "fused" in e:
        for t, a, b in transitions(e["timestamp"], e["fused"], None):
            add(t, f"ekf gnss_pos fused {a} -> {b}", "ekf_gnss")

    # vehicle status
    s = get(ul, "vehicle_status")
    if s is not None:
        for t, a, b in transitions(s["timestamp"], s["nav_state"], None):
            add(t, f"nav_state {NAV_STATE.get(a, a)} -> {NAV_STATE.get(b, b)}", "nav_state")
        for t, a, b in transitions(s["timestamp"], s["arming_state"], None):
            add(t, f"arming {ARMING.get(a, a)} -> {ARMING.get(b, b)}", "arming")
        if "failsafe" in s:
            for t, a, b in transitions(s["timestamp"], s["failsafe"], None):
                add(t, f"vehicle_status.failsafe {a} -> {b}", "failsafe")

    # every flag that flips inside the window (steady-state flags are skipped)
    for topic in ("failsafe_flags", "estimator_status_flags"):
        d = get(ul, topic)
        if d is None:
            continue
        ts = d["timestamp"]
        nflip = nwin = 0
        for name, col in d.items():
            if name in ("timestamp", "timestamp_sample") or name.startswith("_padding"):
                continue
            if col.dtype.kind not in "biu" or col.dtype.itemsize > 1:
                continue          # keep 1-byte flags only (bool / uint8 / int8); skips counters and mode_req_* masks
            flips = list(transitions(ts, col, None))
            nflip += len(flips)
            for t, a, b in flips:
                if lo_us <= t * 1000 <= hi_us:
                    add(t, f"{topic}.{name} {a} -> {b}", topic)
                    nwin += 1
        print(f"[px4] {topic}: {nflip} flag flips in whole log, {nwin} inside the window", file=sys.stderr)

    # PX4's own log messages
    for m in ul.logged_messages:
        add(m.timestamp / 1000.0, f"PX4 log [{LOG_LVL.get(m.log_level % 48 if m.log_level >= 48 else m.log_level, m.log_level)}] {m.message.strip()}", "log")
    return ev

def find_ulog(p):
    p = os.path.expanduser(p)
    if os.path.isdir(p):
        c = glob.glob(os.path.join(p, "**", "*.ulg"), recursive=True)
        if not c:
            sys.exit(f"no .ulg under {p}")
        return max(c, key=os.path.getmtime)
    return p

# ------------------------------------------------------------------ merge / report
def merge(cfs, px4, pre_ms, post_ms):
    t0 = next((e["t"] for e in cfs if e["key"] == 1 or "GPS_FAIL sent" in e["what"]), None)
    if t0 is None:
        sys.exit("no injection event (GPS_FAIL sent / TL_INJECT_SENT) found in the DS files")
    t_end = max((e["t"] for e in cfs), default=t0)
    lo, hi = t0 - pre_ms, t_end + post_ms
    rows = [e for e in cfs + px4 if lo <= e["t"] <= hi]
    rows.sort(key=lambda e: e["t"])
    for e in rows:
        e["d"] = e["t"] - t0
    return t0, rows

def first(rows, pred, after=0.0):
    return next((e for e in rows if e["side"] == "PX4" and e["d"] >= after and pred(e)), None)

def summary(rows):
    restore = next((e for e in rows if e["side"] == "cFS" and
                    (e["key"] == 2 or "GPS_RESTORE sent" in e["what"] or "GPS_OK sent" in e["what"])), None)
    out = []
    def line(label, e, base=0.0):
        out.append(f"  {label:<46}" + (f"{e['d'] - base:>9.0f} ms" if e else "      (not seen)"))
    fix_lost = first(rows, lambda e: e["key"] in ("gps_stop",) or (e["key"] == "gps_fix" and e["what"].endswith(("0", "1", "2"))), after=-100.0)
    fo = first(rows, lambda e: e["key"] == "failsafe" and e["what"].endswith("1"))
    ns = first(rows, lambda e: e["key"] == "nav_state")
    msg = first(rows, lambda e: e["key"] == "log" and "ailsafe" in e["what"])
    out.append("Fault (t=0 -> PX4 reaction)")
    line("last GPS sample in PX4 (sensor_gps stops)", fix_lost)
    line("first nav_state change", ns)
    line("vehicle_status.failsafe set", fo)
    line("PX4 'failsafe' log message", msg)
    if restore:
        rb = restore["d"]
        out.append(f"Restore sent at +{rb:.0f} ms (from here on, relative to restore)")
        back = first(rows, lambda e: e["key"] == "gps_resume" or (e["key"] == "gps_fix" and e["what"].endswith(("3", "4", "5", "6"))), rb)
        fz = first(rows, lambda e: e["key"] == "ekf_gnss" and e["what"].endswith("0 -> 1"), rb)
        nsr = first(rows, lambda e: e["key"] == "nav_state", rb)
        line("GPS samples resume in PX4", back, rb)
        line("EKF fuses GNSS position again", fz, rb)
        line("nav_state change after restore", nsr, rb)
    return "\n".join(out)

def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--ds", nargs="+", required=True)
    ap.add_argument("--ulog", required=True)
    ap.add_argument("--csv")
    ap.add_argument("--topics", action="store_true", help="list logged topics and exit")
    ap.add_argument("--pre", type=float, default=2.0, help="seconds before injection to include")
    ap.add_argument("--post", type=float, default=15.0, help="seconds after last cFS event to include")
    a = ap.parse_args()

    files = sorted(f for p in a.ds for f in glob.glob(os.path.expanduser(p)))
    if not files:
        sys.exit("no DS files matched")
    ulog = find_ulog(a.ulog)
    print(f"DS files : {len(files)}\nULog     : {ulog}\n")

    if a.topics:
        from pyulog import ULog
        for d in sorted(ULog(ulog).data_list, key=lambda d: d.name):
            print(f"{d.name:<40} {len(d.data['timestamp']):>7} samples")
        return
    cfs = cfs_events(files)
    # figure out the window first so flag scans only keep events near the fault
    t0_guess = next((e["t"] for e in cfs if e["key"] == 1 or "GPS_FAIL sent" in e["what"]), None)
    if t0_guess is None:
        sys.exit("no injection event found in the DS files")
    t_end = max(e["t"] for e in cfs)
    px4 = px4_events(ulog, t0_guess - a.pre * 1000, t_end + a.post * 1000)
    t0, rows = merge(cfs, px4, a.pre * 1000, a.post * 1000)

    print(f"{'d_ms':>8} {'px4_ms':>9}  side  event")
    csv = ["d_ms,px4_ms,side,estimated,event"]
    for e in rows:
        mark = "~" if e["est"] else " "
        print(f"{e['d']:>8.0f} {e['t']:>9.0f}{mark} {e['side']:<4}  {e['what']}")
        csv.append(f"{e['d']:.0f},{e['t']:.0f},{e['side']},{int(e['est'])},\"{e['what']}\"")
    print("\n~ = placed from cFE time via median offset (event had no px4_ms)\n")
    print(summary(rows))
    if a.csv:
        open(a.csv, "w").write("\n".join(csv) + "\n")
        print("\nwrote", a.csv)

if __name__ == "__main__":
    main()