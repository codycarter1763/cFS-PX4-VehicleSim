# Enabling SBN Commands/Telemetry in the ground system

This directory contains data that can be used by the Ground System tool to send
commands to the SBN app and receive telemetry from it.

After following the steps below to enable SBN commands and telemetry in the
Ground System, you should be able to start the GUI as usual and immediately see
the pages associated to SBN.

## Commands

To send commands to SBN, add the following to the `command-pages.txt` file
under `tools/cFS-GroundSystem/Subsystems/cmdGui/` in your cFS distribution:

```
SBN, SBN_CMD, 0x18FA, LE, UdpCommands.py, 127.0.0.1, 1234
```

Add the file `SBN_CMD` under
`tools/cFS-GroundSystem/Subsystems/cmdGui/CommandFiles`, and place the files
`SBN_HK_*` under `tools/cFS-GroundSystem/Subsystems/cmdGui/ParameterFiles`.

## Telemetry

To receive telemetry from SBN, add the following to the `telemetry-pages.txt`
file and copy the corresponding files to the directory
`tools/cFS-GroundSystem/Subsystems/tlmGUI/` of your cFS distribution:

```
SBN HK Tlm,           GenericTelemetry.py, 0x8FB, cfs-sbn-hk-tlm.txt
SBN HK Net Tlm,       GenericTelemetry.py, 0x8FC, cfs-sbn-hknet-tlm.txt
SBN HK Peer Tlm,      GenericTelemetry.py, 0x8FD, cfs-sbn-hkpeer-tlm.txt
SBN HK My Subs Tlm,   GenericTelemetry.py, 0x8FE, cfs-sbn-hkmysubs-tlm.txt
SBN HK Peer Subs Tlm, GenericTelemetry.py, 0x8FF, cfs-sbn-hkpeersubs-tlm.txt
```
