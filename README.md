<img width="1857" height="1059" alt="image" src="https://github.com/user-attachments/assets/b0ac6283-fe8b-44a2-bbd3-8781b98f5131" />

# About
This repository documents an embedded flight-software integration project demonstrating communication between NASA's Core Flight System (cFS), PX4 Autopilot Software, Gazebo, and QGroundControl with an autonomous quadcopter.

The project integrates cFS with PX4 Software-in-the-Loop over MAVLink, enabling flight telemetry monitoring, command exchange, and fault-injection testing. A custom cFS application processes PX4 telemetry and supports GPS failure injection and restoration through cFS Stored Command (SC) sequences.

# What is NASA Core Flight System?
NASA's Core Flight System (cFS) is a reusable, platform-independent flight software framework designed for embedded real-time systems. Its architecture separates applications from the underlying operating system and hardware, making flight software more portable and reusable.

The primary components include:

| Component | Description |
|---|---|
| **cFE — core Flight Executive** | Provides the application runtime environment and common flight-software services. |
| **OSAL — Operating System Abstraction Layer** | Provides a consistent API between cFS applications and the underlying operating system. |
| **PSP — Platform Support Package** | Provides an abstraction between cFS and the target hardware platform. |

## cFS Apps Running In This Project
For this project, cFS provides the command flight software side of the system, while PX4 provides the autonomous control of the quadcopter.

| App | Description |
|---|---|
| **MAVLINK_APP** | Reads CCSDS packets from ESP32 via serial. |
| **TO_LAB** | Outlet for telemetry data from cFS to external applications. |
| **CI_LAB** | Inlet for commands for cFS from external applications. |
| **SC** | Runs stored command sequences (RTS) when triggered. |

# What is PX4 Autopilot and Gazebo?

PX4 is an open-source autopilot software platform designed for autonomous vehicles with a very active community. It provides the flight-control software responsible for processing sensor data, estimating vehicle state, controlling vehicle motion, executing flight modes, and simulating the vehicle with a virtual flight computer.

## PX4's Role in This Project
PX4 serves as the simulated flight-control system, providing vehicle telemetry and processing commands exchanged with NASA cFS over MAVLink.

The integration enables cFS to monitor selected flight data and initiate fault-injection scenarios, including GPS failure and restoration. These tests allow PX4's response to injected faults to be observed alongside cFS command execution and event reporting.

PX4 retains responsibility for its own flight-control and failsafe behavior, while cFS provides an additional software integration and test framework.

## What is Gazebo?
In this project, PX4 Software-in-the-Loop can be used with Gazebo to simulate a vehicle while PX4 runs its flight-control software. This provides an accurate environment for observing flight behavior and testing the integration with NASA cFS.

Gazebo's role is to simulate the vehicle and its environment, while PX4 runs the autopilot software that controls the simulated vehicle.

## What is QGroundControl?

QGroundControl is an open-source ground-control application for MAVLink-compatible vehicles. QGroundControl communicates with PX4 over MAVLink and provides visibility into the simulated vehicle's state and behavior. Its capabilities include:

| Feature | Description |
|---|---|
| **Vehicle telemetry** | Displays information such as position, attitude, battery status, and GPS health. |
| **Mission planning** | Allows users to create, upload, and manage supported flight missions. |
| **Vehicle control** | Provides supported commands and controls for interacting with the autopilot. |
| **Configuration** | Exposes supported PX4 parameters and vehicle settings. |
| **Status monitoring** | Displays vehicle status messages and other diagnostic information. |

QGroundControl provides a ground-control interface for observing PX4 SITL and interacting with the simulated vehicle. It can be used to monitor the vehicle during fault-injection tests and observe relevant status changes.

# 
