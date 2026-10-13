#!/usr/bin/env python3
"""
Read DS files (timeline MID 0x08B7 + EVS long events 0x0808) and print the
fault-propagation timeline.

usage: python3 read_ds_timeline.py ~/cFS/build-native_std/exe/cpu1/cf/tline*.dat [--csv out.csv]

Time columns:
  d_px4 = PX4 time_boot_ms delta from the injection (ms)   <- common clock
  d_cfe = cFE packet timestamp delta from the injection (ms) <- cFS-side clock
Injection = first 'GPS_FAIL sent' event or TL_INJECT_SENT packet.
"""
import re, struct, sys, glob

MID_EVS, MID_TL = 0x0808, 0x08B7
TL_NAMES = {1: "INJECT_SENT", 2: "RESTORE_SENT", 3: "MISSION_SENT", 4: "CMD_ACK",
            5: "GPS_FIX", 6: "GPS_STALE", 7: "GPS_RESUME", 8: "MODE_CHANGE",
            9: "STATUSTEXT"}

def walk(buf, start):
    """Walk CCSDS packets from start; return list of (off,len) or None if it doesn't tile the file."""
    pkts, p = [], start
    while p < len(buf):
        if p + 12 > len(buf):
            return None
        mid = struct.unpack_from(">H", buf, p)[0]
        if mid not in (MID_EVS, MID_TL):
            return None
        total = struct.unpack_from(">H", buf, p + 4)[0] + 7
        if total < 16 or p + total > len(buf):
            return None
        pkts.append((p, total))
        p += total
    return pkts if pkts else None

def parse_file(path):
    buf = open(path, "rb").read()
    for start in range(0, min(len(buf), 512)):          # skip cFE + DS file headers
        pk = walk(buf, start)
        if pk:
            break
    else:
        sys.exit(f"{path}: could not find a packet chain (is it the right file / closed?)")
    rows = []
    for off, total in pk:
        mid = struct.unpack_from(">H", buf, off)[0]
        secs, sub = struct.unpack_from(">IH", buf, off + 6)   # cFE time, secondary header
        cfe = secs + sub / 65536.0
        if mid == MID_TL:
            code, a8, a16, px4, a32 = struct.unpack_from("<BBHII", buf, off + 16)
            rows.append(dict(cfe=cfe, px4=px4, src="TL",
                             what=f"{TL_NAMES.get(code, code)} a8={a8} a16={a16} a32={a32}", code=code))
        else:
            app = buf[off+16:off+36].split(b"\0")[0].decode(errors="replace")
            eid = struct.unpack_from("<H", buf, off + 36)[0]
            msg = buf[off+48:off+total].split(b"\0")[0].decode(errors="replace")
            m = re.search(r"px4_ms=(\d+)", msg)
            rows.append(dict(cfe=cfe, px4=int(m.group(1)) if m else None, src="EVS",
                             what=f"{app} {eid}: {msg}", code=None))
    return rows

def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    csv_out = sys.argv[sys.argv.index("--csv") + 1] if "--csv" in sys.argv else None
    if csv_out and csv_out in args: args.remove(csv_out)
    files = sorted(f for a in args for f in glob.glob(a))
    if not files: sys.exit(__doc__)
    rows = [r for f in files for r in parse_file(f)]
    rows.sort(key=lambda r: r["cfe"])
    # keep only mavlink events from EVS to cut the noise
    rows = [r for r in rows if r["src"] == "TL" or "MAVLINK_APP" in r["what"] or r["what"].startswith("SC ")]
    t0 = next((r for r in rows if r["code"] == 1 or "GPS_FAIL sent" in r["what"]), None)
    print(f"{'d_px4 ms':>9} {'d_cfe ms':>9} {'px4_ms':>9}  src  event")
    out = ["d_px4_ms,d_cfe_ms,px4_ms,src,event"]
    for r in rows:
        dp = (r["px4"] - t0["px4"]) if (t0 and t0["px4"] is not None and r["px4"] is not None) else None
        dc = round((r["cfe"] - t0["cfe"]) * 1000) if t0 else None
        f = lambda v: "" if v is None else str(v)
        print(f"{f(dp):>9} {f(dc):>9} {f(r['px4']):>9}  {r['src']:<3}  {r['what']}")
        out.append(f"{f(dp)},{f(dc)},{f(r['px4'])},{r['src']},\"{r['what']}\"")
    if csv_out:
        open(csv_out, "w").write("\n".join(out) + "\n"); print("wrote", csv_out)

if __name__ == "__main__":
    main()
