#!/usr/bin/env python3
"""
Read cFS Data Storage (DS) files and print a fault-propagation timeline.

Reads:
    Timeline packets (MID 0x08B7)
    EVS long-event packets (MID 0x0808)

Usage:
    python3 read_ds_timeline.py ~/cFS/build-native_std/exe/cpu1/cf/tline*.dat
    python3 read_ds_timeline.py ~/cFS/build-native_std/exe/cpu1/cf/tline*.dat --csv timeline.csv
    python3 read_ds_timeline.py ~/cFS/build-native_std/exe/cpu1/cf/tline*.dat --ms

Time columns:
    d_px4_ms: Time since injection using the PX4 boot clock.
    d_cfe_ms: Time since injection using the cFE packet timestamp.
    px4_time: PX4 boot time formatted as HH:MM:SS.mmm.

The injection point is the first INJECT_SENT timeline packet or
GPS_FAIL sent EVS event.
"""

import glob
import re
import struct
import sys


# cFS message IDs
MID_EVS = 0x0808
MID_TIMELINE = 0x08B7


# Timeline event names
TIMELINE_EVENTS = {
    1: "INJECT_SENT",
    2: "RESTORE_SENT",
    3: "MISSION_SENT",
    4: "CMD_ACK",
    5: "GPS_FIX",
    6: "GPS_STALE",
    7: "GPS_RESUME",
    8: "MODE_CHANGE",
    9: "STATUSTEXT",
}


# Patterns for extracting PX4 timestamps from EVS messages
PX4_MS_PATTERN = re.compile(
    r"\s*px4_ms=(\d+)(?:\s*\([\d:.]+\))?"
)

PX4_TIME_PATTERN = re.compile(
    r"\s*px4_time\s*=\s*(\d+):(\d+):(\d+)\.(\d+)"
)


def format_px4_time(milliseconds):
    """Convert PX4 boot milliseconds to HH:MM:SS.mmm."""

    if milliseconds is None:
        return ""

    hours, remainder = divmod(int(round(milliseconds)), 3_600_000)
    minutes, remainder = divmod(remainder, 60_000)
    seconds, milliseconds = divmod(remainder, 1_000)

    return (
        f"{hours:02d}:"
        f"{minutes:02d}:"
        f"{seconds:02d}."
        f"{milliseconds:03d}"
    )


def find_ccsds_packets(data, start_offset):
    """
    Walk through consecutive CCSDS packets.

    Returns a list of (offset, packet_length) tuples.
    Returns None if the data does not form a valid packet sequence.
    """

    packets = []
    offset = start_offset

    while offset < len(data):

        # A packet must have enough bytes for its primary header
        # and timestamp fields.
        if offset + 12 > len(data):
            return None

        message_id = struct.unpack_from(">H", data, offset)[0]

        if message_id not in (MID_EVS, MID_TIMELINE):
            return None

        # CCSDS length field stores packet data length minus one.
        packet_length = (
            struct.unpack_from(">H", data, offset + 4)[0] + 7
        )

        if packet_length < 16:
            return None

        if offset + packet_length > len(data):
            return None

        packets.append((offset, packet_length))
        offset += packet_length

    return packets if packets else None


def extract_px4_timestamp(message):
    """
    Extract the PX4 timestamp from an EVS message.

    Supported formats:
        px4_ms=80636
        px4_ms=80636 (00:01:20.636)
        px4_time = 0:01:20.636

    Returns:
        (timestamp_ms, cleaned_message)
    """

    timestamp_ms = None

    match = PX4_MS_PATTERN.search(message)

    if match:
        timestamp_ms = int(match.group(1))

    else:
        match = PX4_TIME_PATTERN.search(message)

        if match:
            hours = int(match.group(1))
            minutes = int(match.group(2))
            seconds = int(match.group(3))
            fraction = match.group(4)

            milliseconds = int((fraction + "00")[:3])

            timestamp_ms = (
                (hours * 60 + minutes) * 60 + seconds
            ) * 1000 + milliseconds

    # Remove timestamps from the event text because they have
    # their own dedicated output columns.
    message = PX4_MS_PATTERN.sub("", message)
    message = PX4_TIME_PATTERN.sub("", message)

    return timestamp_ms, message.replace("\t", "").rstrip()


def parse_data_storage_file(file_path):
    """Read a DS file and decode its timeline and EVS packets."""

    with open(file_path, "rb") as file:
        data = file.read()

    # Skip cFE and DS file headers by searching for the first
    # valid sequence of CCSDS packets.
    packets = None

    for start_offset in range(min(len(data), 512)):
        packets = find_ccsds_packets(data, start_offset)

        if packets:
            break

    if not packets:
        sys.exit(
            f"{file_path}: Could not find a valid packet sequence. "
            "Check that this is the correct file and that it is closed."
        )

    events = []

    for offset, packet_length in packets:

        message_id = struct.unpack_from(">H", data, offset)[0]

        # Decode the cFE packet timestamp.
        seconds, subseconds = struct.unpack_from(
            ">IH", data, offset + 6
        )

        cfe_timestamp = seconds + subseconds / 65536.0

        if message_id == MID_TIMELINE:

            # Timeline payload:
            # event code, 8-bit value, 16-bit value,
            # PX4 timestamp, 32-bit value.
            event_code, value_8, value_16, px4_ms, value_32 = (
                struct.unpack_from("<BBHII", data, offset + 16)
            )

            event_name = TIMELINE_EVENTS.get(
                event_code, str(event_code)
            )

            description = (
                f"{event_name} "
                f"a8={value_8} "
                f"a16={value_16} "
                f"a32={value_32}"
            )

            events.append({
                "cfe": cfe_timestamp,
                "px4": px4_ms,
                "source": "TL",
                "description": description,
                "code": event_code,
            })

        else:

            # Decode the EVS application name and event ID.
            application = (
                data[offset + 16:offset + 36]
                .split(b"\0")[0]
                .decode(errors="replace")
            )

            event_id = struct.unpack_from(
                "<H", data, offset + 36
            )[0]

            # Extract the EVS event message.
            message = (
                data[offset + 48:offset + packet_length]
                .split(b"\0")[0]
                .decode(errors="replace")
            )

            px4_ms, message = extract_px4_timestamp(message)

            events.append({
                "cfe": cfe_timestamp,
                "px4": px4_ms,
                "source": "EVS",
                "description": f"{application} {event_id}: {message}",
                "code": None,
            })

    return events


def main():
    """Load DS files, find the injection point, and print the timeline."""

    arguments = sys.argv[1:]

    show_raw_milliseconds = "--ms" in arguments

    csv_path = None

    if "--csv" in arguments:
        csv_index = arguments.index("--csv")

        if csv_index + 1 >= len(arguments):
            sys.exit("Error: --csv requires an output filename.")

        csv_path = arguments[csv_index + 1]

    # Exclude command-line options and the CSV output filename
    # from the list of input file patterns.
    input_patterns = [
        argument
        for argument in arguments
        if not argument.startswith("--")
        and argument != csv_path
    ]

    file_paths = sorted(
        path
        for pattern in input_patterns
        for path in glob.glob(pattern)
    )

    if not file_paths:
        sys.exit(__doc__)

    events = [
        event
        for path in file_paths
        for event in parse_data_storage_file(path)
    ]

    # Sort events by the cFE timestamp.
    events.sort(key=lambda event: event["cfe"])

    # Keep timeline packets and relevant MAVLINK_APP / SC events.
    events = [
        event
        for event in events
        if event["source"] == "TL"
        or "MAVLINK_APP" in event["description"]
        or event["description"].startswith("SC ")
    ]

    # Find the first GPS injection event.
    injection_event = next(
        (
            event
            for event in events
            if event["code"] == 1
            or "GPS_FAIL sent" in event["description"]
        ),
        None,
    )

    # Configure the PX4 timestamp column.
    if show_raw_milliseconds:
        px4_column_name = "px4_ms"
        px4_column_width = 9
    else:
        px4_column_name = "px4_time"
        px4_column_width = 12

    # Print the table header.
    print(
        f"{'d_px4 ms':>9} "
        f"{'d_cfe ms':>9} "
        f"{px4_column_name:>{px4_column_width}}  "
        "src  event"
    )

    csv_rows = [
        "d_px4_ms,d_cfe_ms,px4_time,px4_ms,src,event"
    ]

    for event in events:

        # Calculate elapsed time since injection.
        if (
            injection_event is not None
            and injection_event["px4"] is not None
            and event["px4"] is not None
        ):
            delta_px4 = event["px4"] - injection_event["px4"]
        else:
            delta_px4 = None

        if injection_event is not None:
            delta_cfe = round(
                (event["cfe"] - injection_event["cfe"]) * 1000
            )
        else:
            delta_cfe = None

        # Format values for terminal output.
        px4_time = format_px4_time(event["px4"])

        if show_raw_milliseconds:
            px4_display = (
                "" if event["px4"] is None else str(event["px4"])
            )
        else:
            px4_display = px4_time

        delta_px4_display = (
            "" if delta_px4 is None else str(delta_px4)
        )

        delta_cfe_display = (
            "" if delta_cfe is None else str(delta_cfe)
        )

        print(
            f"{delta_px4_display:>9} "
            f"{delta_cfe_display:>9} "
            f"{px4_display:>{px4_column_width}}  "
            f"{event['source']:<3}  "
            f"{event['description']}"
        )

        # Escape event text for CSV output.
        csv_event = event["description"].replace('"', '""')

        csv_rows.append(
            f'{delta_px4_display},'
            f'{delta_cfe_display},'
            f'{px4_time},'
            f'{"" if event["px4"] is None else event["px4"]},'
            f'{event["source"]},'
            f'"{csv_event}"'
        )

    if csv_path:
        with open(csv_path, "w", encoding="utf-8") as file:
            file.write("\n".join(csv_rows) + "\n")

        print(f"\nCSV written to: {csv_path}")


if __name__ == "__main__":
    main()