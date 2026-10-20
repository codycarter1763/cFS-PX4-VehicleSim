#!/usr/bin/env python3
"""
Merge cFS Data Storage events and the PX4 ULog into one timeline on PX4's boot clock,
then show the causal sequence: fault injection -> first divergence -> PX4 failsafe -> restore.

Usage:
    python3 merge_timeline.py \
        --ds ~/cFS/build-native_std/exe/cpu1/cf/tline*.dat \
        --ulog ~/PX4-Autopilot/build/px4_sitl_default/rootfs/fs/log/<date>/<flight>.ulg \
        --json run.json --csv run.csv

Needs:  pip install pyulog     (read_ds_timeline.py must be in the same folder)


ULog timestamps are microseconds since PX4 boot, the same clock as MAVLink
time_boot_ms, so cFS events that carry px4_ms and PX4 events share one axis.
cFS events without px4_ms are placed with the median (px4_ms - cFE time) offset
and marked with '~'. 

Anchors for the causal sequence:
    Fault:   PX4's own "Injected" log line (falls back to the cFS GPS_FAIL send).
    Restore: PX4's own "Cleared" log line  (falls back to the cFS GPS_RESTORE send).

Output files are numbered (run_001.json, run_002.json, ...) so a new run never
overwrites an old one. Use --overwrite to write the exact name instead.
"""

import argparse
import csv
import glob
import hashlib
import json
import os
import re
import statistics
import sys

import numpy as np
from pyulog import ULog

from read_ds_timeline import parse_data_storage_file, format_px4_time


# constants

GPS_TOPICS = ("vehicle_gnss", "sensor_gnss", "sensor_gps")
MIN_GPS_GAP_MS = 500

NAV_STATES = {
    0: "MANUAL", 1: "ALTCTL", 2: "POSCTL", 3: "AUTO_MISSION",
    4: "AUTO_LOITER", 5: "AUTO_RTL", 12: "DESCEND",
    14: "OFFBOARD", 15: "STAB", 17: "AUTO_TAKEOFF", 18: "AUTO_LAND",
}
ARMING_STATES = {1: "DISARMED", 2: "ARMED"}
LOG_LEVELS = {0: "EMERG", 1: "ALERT", 2: "CRIT", 3: "ERR",
              4: "WARN", 5: "NOTICE", 6: "INFO", 7: "DEBUG"}

# Estimator flags that show the EKF losing or regaining GPS aiding.
EKF_AIDING_FLAGS = ("cs_gps_hgt", "cs_gnss_pos", "cs_gnss_vel",
                    "cs_inertial_dead_reckoning")

# Stages that count as "the first thing PX4 did differently".
DIVERGENCE_STAGES = ("EKF_AIDING_LOSS", "GPS_FIX_CHANGE", "POS_ERROR",
                     "FAILSAFE_DECISION", "FAILSAFE_ACTION")

# PX4 only writes non-default parameters to the log, so a missing name means "at default".
PARAMETER_NAMES = ("EKF2_NOAID_TOUT", "EKF2_GPS_CTRL", "SYS_FAILURE_EN",
                   "NAV_RCL_ACT", "COM_ARM_WO_GPS")

# The restore phase starts this long before the cFS restore send (clock slop).
RESTORE_MARGIN_MS = 20


# events
"""
Dictionary for the events from the logs to follow.
"""
def make_event(time_ms, side, text, key=None, signal=None, old=None, new=None,
               estimated=False):
    
    return {
        "time": time_ms,        # PX4 boot time in ms
        "side": side,           # "cFS" or "PX4"
        "event": text,
        "key": key,             # cFS: timeline event code.  PX4: kind of event.
        "signal": signal,       # PX4 signal name for state changes
        "old": old,
        "new": new,
        "estimated": estimated, # True if placed with the cFE-clock offset
    }

"""
Return event as GPS_FAIL based on the logs.
"""
def is_injection(event):
    return event["side"] == "cFS" and (
        event["key"] == 1 or "GPS_FAIL sent" in event["event"])

"""
Return event as GPS_RESTORE based on the logs.
"""
def is_restore(event):
    return event["side"] == "cFS" and (
        event["key"] == 2 or "GPS_RESTORE sent" in event["event"]
        or "GPS_OK sent" in event["event"])

"""
Wrapper for --ms switch. 
"""
def format_time(time_ms, raw_ms):
    if raw_ms:
        return f"{time_ms:.0f}"
    return format_px4_time(time_ms)


# cFS side
"""
Looks at the path and finds the DS files in the cFS build directory.
"""
def find_ds_files(patterns):
    files = sorted(path for pattern in patterns
                   for path in glob.glob(os.path.expanduser(pattern)))
    if not files:
        sys.exit("No Data Storage files matched --ds.")
    return files


"""
Read relevant cFS events and put them on the PX4 clock.
"""
def load_cfs_events(files, run_info):
    raw_events = []
    for path in files:
        raw_events.extend(parse_data_storage_file(path))

    raw_events = [
        event for event in raw_events
        if event["source"] == "TL"
        or "MAVLINK_APP" in event["description"]
        or event["description"].startswith("SC ")
    ]
    raw_events.sort(key=lambda event: event["cfe"])

    # How far the cFE clock is from the PX4 clock, from events that carry both.
    offsets = [event["px4"] - event["cfe"] * 1000
               for event in raw_events if event["px4"] is not None]
    offset = statistics.median(offsets) if offsets else None

    if offsets:
        deviations = [value - offset for value in offsets]
        run_info["sync"] = {
            "events": len(offsets),
            "spread_min_ms": min(deviations),
            "spread_max_ms": max(deviations),
            "typical_abs_ms": statistics.median(abs(d) for d in deviations),
        }
        print(f"[sync] cFE->PX4 offset from {len(offsets)} events: spread "
              f"{min(deviations):+.0f}..{max(deviations):+.0f} ms "
              f"(typ. +/-{run_info['sync']['typical_abs_ms']:.0f} ms); "
              "'~' rows carry this error", file=sys.stderr)

    events = []
    for event in raw_events:
        if event["px4"] is not None:
            time_ms, estimated = float(event["px4"]), False
        elif offset is not None:
            time_ms, estimated = event["cfe"] * 1000 + offset, True
        else:
            continue
        events.append(make_event(time_ms, "cFS", event["description"],
                                 key=event["code"], estimated=estimated))
    return events


# PX4 side
"""
Return a ULog topic's data, or None if it was not recorded.
"""
def get_topic(log, name):
    try:
        return log.get_dataset(name).data
    except Exception:
        return None

"""
Add one event each time a value changes.
"""
def add_changes(events, times_us, values, label, key, signal, names=None):
    previous = None
    for time_us, value in zip(times_us, values):
        value = int(value)
        if previous is not None and value != previous:
            old = names.get(previous, previous) if names else previous
            new = names.get(value, value) if names else value
            events.append(make_event(time_us / 1000, "PX4",
                                     f"{label} {old} -> {new}", key=key,
                                     signal=signal, old=old, new=new))
        previous = value

"""
GPS fix changes, plus gaps (injection stops the publisher; no fix_type=0 appears).
"""
def add_gps_events(log, events):
    candidates = []
    for name in GPS_TOPICS:
        data = get_topic(log, name)
        if data is not None and len(data["timestamp"]) > 2:
            candidates.append((name, data))
    if not candidates:
        print("[px4] no GPS topic found", file=sys.stderr)
        return

    name, gps = max(candidates, key=lambda item: len(item[1]["timestamp"]))
    times = gps["timestamp"]

    if "fix_type" in gps:
        add_changes(events, times, gps["fix_type"], f"{name} fix_type",
                    "gps_fix", f"{name}.fix_type")

    gaps_ms = [(times[i] - times[i - 1]) / 1000 for i in range(1, len(times))]
    gap_limit = max(MIN_GPS_GAP_MS, 5 * statistics.median(gaps_ms))
    for i, gap in enumerate(gaps_ms, start=1):
        if gap <= gap_limit:
            continue
        events.append(make_event(
            times[i - 1] / 1000, "PX4",
            f"{name} last sample before {gap:.0f} ms gap", key="gps_stop",
            signal=f"{name}.samples", old="streaming", new="stopped"))
        events.append(make_event(
            times[i] / 1000, "PX4",
            f"{name} samples resume after {gap:.0f} ms gap", key="gps_resume",
            signal=f"{name}.samples", old="stopped", new="streaming"))
    print(f"[px4] GPS source: {name} ({len(times)} samples, "
          f"gap threshold {gap_limit:.0f} ms)", file=sys.stderr)

"""
The EKF starts or stops fusing GNSS position.
"""
def add_ekf_fusion_events(log, events):
    
    data = get_topic(log, "estimator_aid_src_gnss_pos")
    if data is not None and "fused" in data:
        add_changes(events, data["timestamp"], data["fused"],
                    "ekf gnss_pos fused", "ekf_gnss",
                    "estimator_aid_src_gnss_pos.fused")

"""
Helper for vehicle status events.
"""
def add_vehicle_status_events(log, events):
    status = get_topic(log, "vehicle_status")
    if status is None:
        return
    times = status["timestamp"]
    if "nav_state" in status:
        add_changes(events, times, status["nav_state"], "nav_state",
                    "nav_state", "vehicle_status.nav_state", NAV_STATES)
    if "arming_state" in status:
        add_changes(events, times, status["arming_state"], "arming",
                    "arming", "vehicle_status.arming_state", ARMING_STATES)
    if "failsafe" in status:
        add_changes(events, times, status["failsafe"], "vehicle_status.failsafe",
                    "failsafe", "vehicle_status.failsafe")

"""
Helper to find failsafe flags.
"""
def add_flag_events(log, events, topic, start_ms, end_ms):
    data = get_topic(log, topic)
    if data is None:
        return
    times = data["timestamp"]
    flips_in_log = 0
    flips_in_window = 0
    for name, column in data.items():
        if name in ("timestamp", "timestamp_sample") or name.startswith("_padding"):
            continue
        # 1-byte flags only (bool / uint8 / int8); skips counters and mode_req_* masks.
        if column.dtype.kind not in "biu" or column.dtype.itemsize > 1:
            continue
        flag_events = []
        add_changes(flag_events, times, column, f"{topic}.{name}", topic,
                    f"{topic}.{name}")
        flips_in_log += len(flag_events)
        for event in flag_events:
            if start_ms <= event["time"] <= end_ms:
                events.append(event)
                flips_in_window += 1
    print(f"[px4] {topic}: {flips_in_log} flag flips in whole log, "
          f"{flips_in_window} inside the window", file=sys.stderr)

"""
The text PX4 itself printed (e.g. 'Injected: gps off', 'Failsafe: blind land').
"""
def add_log_message_events(log, events):
    for message in log.logged_messages:
        level = message.log_level
        if level >= 48:                      # ULog stores the level as an ASCII digit
            level -= 48
        level_name = LOG_LEVELS.get(level, level)
        events.append(make_event(message.timestamp / 1000, "PX4",
                                 f"PX4 log [{level_name}] {message.message.strip()}",
                                 key="log"))

"""
Value of each parameter of interest and where it came from.
"""
def read_parameters(log):
    boot_values = dict(log.initial_parameters)

    defaults = {}
    for default_type in (1, 2):              # 1 = system default, 2 = airframe default
        try:
            defaults.update(log.get_default_parameters(default_type))
        except Exception:
            pass

    changes = {}
    for time_us, name, value in log.changed_parameters:
        changes.setdefault(name, []).append((time_us / 1000, value))

    result = {}
    for name in PARAMETER_NAMES:
        if name in boot_values:
            result[name] = {"value": boot_values[name], "source": "boot value"}
        elif name in defaults:
            result[name] = {"value": defaults[name], "source": "default"}
        else:
            result[name] = {"value": None, "source": "not in log"}
        if name in changes:
            result[name]["changed_during_flight"] = changes[name]
    return result

"""
Everything from the ULog that happened inside the fault or restore window.
"""
def load_px4_events(log, start_ms, end_ms, run_info):
    events = []
    add_gps_events(log, events)
    add_ekf_fusion_events(log, events)
    add_vehicle_status_events(log, events)
    add_flag_events(log, events, "failsafe_flags", start_ms, end_ms)
    add_flag_events(log, events, "estimator_status_flags", start_ms, end_ms)
    add_log_message_events(log, events)

    run_info["px4_parameters"] = read_parameters(log)
    run_info["ulog_span_ms"] = (log.start_timestamp / 1000, log.last_timestamp / 1000)
    return [event for event in events if start_ms <= event["time"] <= end_ms]

"""
A .ulg file, or the newest .ulg under a folder.
"""
def find_ulog(path):
    path = os.path.expanduser(path)
    if not os.path.isdir(path):
        return path
    logs = glob.glob(os.path.join(path, "**", "*.ulg"), recursive=True)
    if not logs:
        sys.exit(f"No .ulg files found under {path}")
    return max(logs, key=os.path.getmtime)


# causal sequence
"""
Return (stage, role) for an event, or ("", "") if it is not part of the sequence.
"""
def classify_event(event, after_restore):
    key = event["key"]
    text = event["event"]
    signal = event["signal"] or ""

    if event["side"] == "cFS":
        if is_injection(event):
            return "INJECT_CMD", "command"
        if is_restore(event):
            return "RESTORE_CMD", "command"
        if text.startswith("SC "):
            return "SC_TRIGGER", "command"
        if key == 4:
            return ("RESTORE_ACK" if after_restore else "INJECT_ACK"), "ack"
        return "CFS_OBSERVES", "observation"

    if key == "log":
        if "Injected" in text:
            return "PX4_INJECTED", "fault applied"
        if "Cleared" in text:
            return "PX4_CLEARED", "fault removed"
        if "ailsafe" in text or "blind land" in text.lower():
            return ("FAILSAFE_CLEARED" if after_restore else "FAILSAFE_DECISION"), "decision"
        return "", ""

    if key == "gps_stop":
        return "LAST_GPS_SAMPLE", "sensor"
    if key == "gps_resume":
        return "GPS_RESUMED", "sensor"
    if key == "gps_fix":
        return ("GPS_RESUMED" if after_restore else "GPS_FIX_CHANGE"), "sensor"

    is_aiding_flag = (key == "estimator_status_flags"
                      and any(flag in signal for flag in EKF_AIDING_FLAGS))
    if key == "ekf_gnss" or is_aiding_flag:
        return ("EKF_REAIDING" if after_restore else "EKF_AIDING_LOSS"), "estimator"
    if key == "pos_err":
        return "POS_ERROR", "estimator"

    if key == "failsafe_flags":
        return ("FAILSAFE_CLEARED" if after_restore else "FAILSAFE_DECISION"), "decision"
    if key == "failsafe":
        return ("FAILSAFE_CLEARED" if after_restore else "FAILSAFE_ACTION"), "action"
    if key == "nav_state":
        return ("MODE_RECOVERED" if after_restore else "FAILSAFE_ACTION"), "action"
    return "", ""

"""
Add stage, role and times to every event. Returns the anchors used.

Events later than recovery_s after the restore anchor (e.g. the end of the mission,
return-to-launch, landing) are left out of the sequence; they stay in the timeline.
"""
def annotate_events(events, injection_time, recovery_s):

    for event in events:
        event["from_cfs_injection_ms"] = event["time"] - injection_time

    restore = next((e for e in events if is_restore(e)), None)
    restore_ms = restore["from_cfs_injection_ms"] if restore else None

    for event in events:
        event["after_restore"] = (
            restore_ms is not None
            and event["from_cfs_injection_ms"] >= restore_ms - RESTORE_MARGIN_MS)
        event["stage"], event["role"] = classify_event(event, event["after_restore"])

    px4_injected = next((e for e in events if e["stage"] == "PX4_INJECTED"), None)
    px4_cleared = next((e for e in events if e["stage"] == "PX4_CLEARED"), None)

    if px4_injected:
        inject_anchor = px4_injected["from_cfs_injection_ms"]
        inject_source = "PX4 'Injected' log"
    else:
        inject_anchor = 0.0
        inject_source = "cFS send (no PX4 'Injected' line found)"

    restore_anchor, restore_source = restore_ms, None
    if restore_ms is not None:
        if px4_cleared:
            restore_anchor = px4_cleared["from_cfs_injection_ms"]
            restore_source = "PX4 'Cleared' log"
        else:
            restore_source = "cFS send (no PX4 'Cleared' line found)"

    for event in events:
        anchor = restore_anchor if event["after_restore"] else inject_anchor
        event["since_anchor_ms"] = event["from_cfs_injection_ms"] - anchor
        if event["after_restore"] and event["since_anchor_ms"] > recovery_s * 1000:
            event["stage"], event["role"] = "", ""

    return {
        "inject_anchor_ms": inject_anchor, "inject_anchor_source": inject_source,
        "restore_anchor_ms": restore_anchor, "restore_anchor_source": restore_source,
        "cfs_inject_ms": 0.0, "cfs_restore_ms": restore_ms,
    }

"""
State before the fault: the 'old' value at each signal's first change.
"""
def find_baseline(events):
    baseline = {}
    for event in events:
        if (event["side"] == "PX4" and event["signal"] and not event["after_restore"]
                and event["from_cfs_injection_ms"] >= -100
                and event["signal"] not in baseline):
            baseline[event["signal"]] = event["old"]
    return baseline

"""
Finds first divergence in fault events.
"""
def find_first_divergence(events):
    for event in events:
        if (event["side"] == "PX4" and not event["after_restore"]
                and event["from_cfs_injection_ms"] >= -100
                and event["stage"] in DIVERGENCE_STAGES):
            return event
    return None

"""
Finds the last valid GPS signal before the fault turns GPS off.
"""
def find_last_good_gps_sample(events):
    for event in events:
        if event["stage"] == "LAST_GPS_SAMPLE" and event["from_cfs_injection_ms"] >= -100:
            return event
    return None

"""
Makes list of events.
"""
def summarize_stages(events):
    
    stages = []
    by_key = {}
    previous_time = None
    for event in events:
        if not event["stage"]:
            continue
        key = (event["stage"], event["after_restore"])
        if key not in by_key:
            gap = None if previous_time is None else event["time"] - previous_time
            by_key[key] = {
                "stage": event["stage"],
                "phase": "restore" if event["after_restore"] else "fault",
                "side": event["side"],
                "role": event["role"],
                "first_px4_ms": event["time"],
                "since_anchor_ms": event["since_anchor_ms"],
                "since_previous_stage_ms": gap,
                "count": 0,
                "first_event": event["event"],
                "estimated": event["estimated"],
            }
            stages.append(by_key[key])
            previous_time = event["time"]
        by_key[key]["count"] += 1
    return stages


# printing
"""
Helper to print the event timeline.
"""
def print_timeline(events, raw_ms):
    time_label = "px4_ms" if raw_ms else "px4_time"
    width = 9 if raw_ms else 12
    print(f"{'from inj ms':>11} {time_label:>{width}}  side  {'stage':<17} event")
    for event in events:
        mark = "~" if event["estimated"] else " "
        print(f"{event['from_cfs_injection_ms']:>+11.0f} "
              f"{format_time(event['time'], raw_ms):>{width}}{mark} "
              f"{event['side']:<4}  {event['stage']:<17} {event['event']}")
    print("\n~ = placed from cFE time via median offset (event had no px4_ms)")

"""
Prints values just before first change.
"""
def print_baseline(baseline):
    if not baseline:
        return
    print("Baseline before the fault (value just before the first change):")
    for signal, value in baseline.items():
        print(f"  {signal:<48} {value}")

"""
Prints phases of the casual sequence.
"""
def print_phase(events, phase_name, after_restore, anchor_ms, anchor_source,
                cfs_send_ms, first_divergence, last_good_sample, raw_ms):
    rows = [e for e in events if e["stage"] and e["after_restore"] == after_restore]
    if not rows:
        return
    send_offset = cfs_send_ms - anchor_ms
    print(f"\n{phase_name}   t=0 anchor: {anchor_source}   "
          f"(cFS send at {send_offset:+.0f} ms from it)")
    width = 9 if raw_ms else 12
    time_label = "px4_ms" if raw_ms else "px4_time"
    print(f"{'since':>8} {'+prev':>7} {time_label:>{width}}  {'side':<4} "
          f"{'stage':<17} {'role':<13} event")

    previous_stage = None
    previous_since = None
    for event in rows:
        stage_label = event["stage"] if event["stage"] != previous_stage else ""
        since_previous = ("" if previous_since is None
                          else f"{event['since_anchor_ms'] - previous_since:+.0f}")
        note = ""
        if event is first_divergence:
            note = "   <== FIRST OBSERVABLE DIVERGENCE"
        elif event is last_good_sample:
            note = "   <== last good GPS sample"
        mark = "~" if event["estimated"] else " "
        print(f"{event['since_anchor_ms']:>+8.0f} {since_previous:>7} "
              f"{format_time(event['time'], raw_ms):>{width}}{mark} "
              f"{event['side']:<4} {stage_label:<17} {event['role']:<13} "
              f"{event['event']}{note}")
        previous_stage = event["stage"]
        previous_since = event["since_anchor_ms"]

"""
Prints PX4 specific parameters for the output log.
"""
def print_parameters(parameters):
    print("\nPX4 parameters for this log")
    for name, info in parameters.items():
        value = "-" if info["value"] is None else info["value"]
        changed = ""
        if info.get("changed_during_flight"):
            changed = f"  CHANGED IN FLIGHT: {info['changed_during_flight']}"
        print(f"  {name:<20} {str(value):<14} ({info['source']}){changed}")

"""
Prints summary of first occurence of each stage.
"""
def print_stage_summary(stages):
    print("\nStage summary (first occurrence of each stage)")
    print(f"  {'phase':<8}{'stage':<18}{'side':<5}{'since anchor':>13}"
          f"{'since prev stage':>18}{'n':>4}")
    for stage in stages:
        gap = stage["since_previous_stage_ms"]
        gap_text = "" if gap is None else f"{gap:+.0f} ms"
        print(f"  {stage['phase']:<8}{stage['stage']:<18}{stage['side']:<5}"
              f"{stage['since_anchor_ms']:>+10.0f} ms{gap_text:>18}{stage['count']:>4}")

"""
Prints full casual fault and restore sequence.
"""
def print_sequence(events, anchors, baseline, first_divergence, last_good_sample,
                   parameters, raw_ms):
    print("\n================ CAUSAL SEQUENCE ================")
    print_baseline(baseline)
    print_phase(events, "FAULT", False, anchors["inject_anchor_ms"],
                anchors["inject_anchor_source"], anchors["cfs_inject_ms"],
                first_divergence, last_good_sample, raw_ms)
    if anchors["cfs_restore_ms"] is not None:
        print_phase(events, "RESTORE", True, anchors["restore_anchor_ms"],
                    anchors["restore_anchor_source"], anchors["cfs_restore_ms"],
                    first_divergence, last_good_sample, raw_ms)
    print_parameters(parameters)
    print_stage_summary(summarize_stages(events))
    print()
    if first_divergence:
        print(f"First observable divergence: {first_divergence['stage']} at "
              f"{first_divergence['since_anchor_ms']:+.0f} ms after the PX4 anchor "
              f"-> {first_divergence['event']}")
    if last_good_sample:
        print(f"Last good GPS sample       : "
              f"{last_good_sample['since_anchor_ms']:+.0f} ms relative to the PX4 anchor")


# output files
"""
Automatically increments file name number if there are two of the same file names.
"""
def next_run_number(output_paths):
    highest = 0
    for path in output_paths:
        stem, _ = os.path.splitext(path)
        folder = os.path.dirname(stem) or "."
        pattern = re.compile(re.escape(os.path.basename(stem)) + r"_(\d{3,})\.")
        if not os.path.isdir(folder):
            continue
        for name in os.listdir(folder):
            match = pattern.match(name)
            if match:
                highest = max(highest, int(match.group(1)))
    return highest + 1

"""
Adds a run number to a filename.
"""
def numbered_path(path, number):
    stem, extension = os.path.splitext(path)
    return f"{stem}_{number:03d}{extension}"

"""
Helper to compute hash for files, used to check what log files pertain to each other.
"""
def file_hash(path):
    try:
        with open(path, "rb") as file:
            return hashlib.sha256(file.read()).hexdigest()[:16]
    except OSError:
        return None

"""
Helper that turns event into a summary to be written into the JSON file.
"""
def describe_event(event):
    if event is None:
        return None
    return {"px4_ms": event["time"], "px4_time": format_px4_time(event["time"]),
            "since_anchor_ms": event["since_anchor_ms"], "stage": event["stage"],
            "event": event["event"]}

"""
Writes JSON file with log data.
"""
def write_json(path, events, anchors, baseline, first_divergence, last_good_sample,
               run_info, args, ds_files, ulog_path):
    document = {
        "provenance": {
            "ulog": {"path": ulog_path, "sha256_16": file_hash(ulog_path)},
            "ds_files": [{"path": p, "sha256_16": file_hash(p)} for p in ds_files],
            "script_args": {"pre_s": args.pre, "post_s": args.post,
                            "err_thresh_m": args.err_thresh,
                            "min_gps_gap_ms": MIN_GPS_GAP_MS},
            "sync": run_info.get("sync"),
            "px4_parameters": run_info.get("px4_parameters"),
            "max_position_error_m": run_info.get("max_position_error_m"),
        },
        "anchors": anchors,
        "baseline": baseline,
        "first_divergence": describe_event(first_divergence),
        "last_good_gps_sample": describe_event(last_good_sample),
        "stages": summarize_stages(events),
        "events": [{
            "px4_ms": e["time"], "px4_time": format_px4_time(e["time"]),
            "since_anchor_ms": e["since_anchor_ms"],
            "from_cfs_injection_ms": e["from_cfs_injection_ms"],
            "side": e["side"], "phase": "restore" if e["after_restore"] else "fault",
            "stage": e["stage"], "role": e["role"], "signal": e["signal"],
            "old": e["old"], "new": e["new"], "estimated": e["estimated"],
            "event": e["event"],
        } for e in events if e["stage"]],
    }
    with open(path, "w") as file:
        json.dump(document, file, indent=1, default=str)
    print("Wrote", path)

"""
Writes CSV with log data.
"""
def write_csv(path, events):
    columns = ("from_cfs_injection_ms", "since_anchor_ms", "px4_time", "px4_ms",
               "side", "estimated", "stage", "role", "signal", "old", "new", "event")
    with open(path, "w", newline="") as file:
        writer = csv.DictWriter(file, fieldnames=columns)
        writer.writeheader()
        for e in events:
            writer.writerow({
                "from_cfs_injection_ms": round(e["from_cfs_injection_ms"]),
                "since_anchor_ms": round(e["since_anchor_ms"]),
                "px4_time": format_px4_time(e["time"]),
                "px4_ms": round(e["time"]),
                "side": e["side"], "estimated": int(e["estimated"]),
                "stage": e["stage"], "role": e["role"], "signal": e["signal"] or "",
                "old": "" if e["old"] is None else e["old"],
                "new": "" if e["new"] is None else e["new"],
                "event": e["event"],
            })
    print("Wrote", path)


# main
"""
Checks if any PX4 events fall within the window of time selected.
"""
def warn_if_wrong_ulog(events, injection_time, run_info):
    span = run_info.get("ulog_span_ms")
    no_px4_events = not any(e["side"] == "PX4" for e in events)
    outside_log = span is not None and not span[0] <= injection_time <= span[1]
    if not (no_px4_events or outside_log):
        return
    span_text = "?" if span is None else f"{format_px4_time(span[0])} .. {format_px4_time(span[1])}"
    print("\n!!! WARNING: no PX4 events fall in this window. The ULog is probably "
          "not the flight these DS files came from.\n"
          f"    fault at PX4 time {format_px4_time(injection_time)}; this ULog covers {span_text}.\n"
          "    Pass the matching .ulg file to --ulog instead of the folder.\n",
          file=sys.stderr)

"""
Input argument parser.
"""
def parse_arguments():
    parser = argparse.ArgumentParser(
        description="Merge cFS Data Storage events and PX4 ULog events.")
    parser.add_argument("--ds", nargs="+", required=True,
                        help="Data Storage file paths or wildcard patterns")
    parser.add_argument("--ulog", required=True,
                        help="ULog file, or a folder (newest .ulg under it is used)")
    parser.add_argument("--csv", help="write the timeline to this CSV file")
    parser.add_argument("--json", help="write anchors, baseline, stages, events and "
                                       "provenance to this JSON file")
    parser.add_argument("--overwrite", action="store_true",
                        help="write --csv/--json exactly as named instead of adding _001, _002, ...")
    parser.add_argument("--seq-only", action="store_true",
                        help="print only the causal sequence, not the full timeline")
    parser.add_argument("--ms", action="store_true",
                        help="show PX4 time as raw milliseconds instead of hh:mm:ss.mmm")
    parser.add_argument("--err-thresh", type=float, default=1.0,
                        help="horizontal position error vs ground truth (m) that counts "
                             "as divergence; 0 turns it off (default: 1.0)")
    parser.add_argument("--recovery-s", type=float, default=10.0,
                        help="seconds after the restore to include in the sequence (default: 10)")
    parser.add_argument("--topics", action="store_true",
                        help="list the topics in the ULog and exit")
    parser.add_argument("--pre", type=float, default=2.0,
                        help="seconds to show before injection (default: 2)")
    parser.add_argument("--post", type=float, default=15.0,
                        help="seconds to show after the last cFS event (default: 15)")
    return parser.parse_args()


def main():
    args = parse_arguments()
    run_info = {}

    ds_files = find_ds_files(args.ds)
    ulog_path = find_ulog(args.ulog)
    print(f"DS files : {len(ds_files)}\nULog     : {ulog_path}\n")

    log = ULog(ulog_path)
    if args.topics:
        for dataset in sorted(log.data_list, key=lambda d: d.name):
            print(f"{dataset.name:<40} {len(dataset.data['timestamp']):>7} samples")
        return

    cfs_events = load_cfs_events(ds_files, run_info)
    injection = next((e for e in cfs_events if is_injection(e)), None)
    if injection is None:
        sys.exit("Could not find a GPS injection event in the cFS files.")

    start_ms = injection["time"] - args.pre * 1000
    end_ms = max(e["time"] for e in cfs_events) + args.post * 1000
    px4_events = load_px4_events(log, start_ms, end_ms, args.err_thresh, run_info)

    events = [e for e in cfs_events + px4_events if start_ms <= e["time"] <= end_ms]
    events.sort(key=lambda e: e["time"])

    anchors = annotate_events(events, injection["time"], args.recovery_s)
    warn_if_wrong_ulog(events, injection["time"], run_info)

    baseline = find_baseline(events)
    first_divergence = find_first_divergence(events)
    last_good_sample = find_last_good_gps_sample(events)

    if not args.seq_only:
        print_timeline(events, args.ms)
    print_sequence(events, anchors, baseline, first_divergence, last_good_sample,
                   run_info["px4_parameters"], args.ms)

    json_path, csv_path = args.json, args.csv
    outputs = [path for path in (json_path, csv_path) if path]
    if outputs and not args.overwrite:
        number = next_run_number(outputs)
        json_path = numbered_path(json_path, number) if json_path else None
        csv_path = numbered_path(csv_path, number) if csv_path else None

    if json_path:
        write_json(json_path, events, anchors, baseline, first_divergence,
                   last_good_sample, run_info, args, ds_files, ulog_path)
    if csv_path:
        write_csv(csv_path, events)


if __name__ == "__main__":
    main()