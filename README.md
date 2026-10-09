


https://github.com/user-attachments/assets/2fa3ce7b-194a-40c1-85f4-a32222d3e443






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
| **MAVLINK_APP** | Reads PX4 SITL data and publishes it to the cFS Software Bus (SB). |
| **TO_LAB** | Outlet for telemetry data from cFS to external applications. |
| **CI_LAB** | Inlet for commands for cFS from external applications. |
| **SC** | Runs stored command sequences (RTS) when triggered. |

# What is PX4 Autopilot and Gazebo?
## PX4's Role in This Project
<img width="30%" alt="PX4 Autopilot logo" src="https://github.com/user-attachments/assets/1daaf3ec-afe6-4d7e-bdcb-ff199b52d341" />
<br/> 
<br/>

PX4 is an open-source autopilot software platform designed for autonomous vehicles with a very active community. It provides the flight-control software responsible for processing sensor data, estimating vehicle state, controlling vehicle motion, executing flight modes, and simulating the vehicle with a virtual flight computer.

The integration enables cFS to monitor selected flight data and initiate fault-injection scenarios, including GPS failure and restoration. These tests allow PX4's response to injected faults to be observed alongside cFS command execution and event reporting.

PX4 retains responsibility for its own flight-control and failsafe behavior, while cFS provides an additional software integration and test framework.

## Gazebo Role in This Project
<img width="30%" alt="Gazebo simulation" src="https://github.com/user-attachments/assets/67d40374-1930-400c-9e09-7c6d428ce975" />
<br/>
<br/>

In this project, PX4 Software-in-the-Loop can be used with Gazebo to simulate a vehicle while PX4 runs its flight-control software. This provides an accurate environment for observing flight behavior and testing the integration with NASA cFS.

Gazebo's role is to simulate the vehicle and its environment, while PX4 runs the autopilot software that controls the simulated vehicle.

# What is QGroundControl?
<img width="30%" alt="image" src="https://github.com/user-attachments/assets/b0d610b2-f992-4c30-8dbb-58c4e8cce8ad" />

<br/>
<br/>
QGroundControl is an open-source ground-control application for MAVLink-compatible vehicles. QGroundControl communicates with PX4 over MAVLink and provides visibility into the simulated vehicle's state and behavior. Its capabilities include:
<br/>
<br/>

| Feature | Description |
|---|---|
| **Vehicle telemetry** | Displays information such as position, attitude, battery status, and GPS health. |
| **Mission planning** | Allows users to create, upload, and manage supported flight missions. |
| **Vehicle control** | Provides supported commands and controls for interacting with the autopilot. |
| **Configuration** | Exposes supported PX4 parameters and vehicle settings. |
| **Status monitoring** | Displays vehicle status messages and other diagnostic information. |

QGroundControl provides a ground-control interface for observing PX4 SITL and interacting with the simulated vehicle. It can be used to monitor the vehicle during fault-injection tests and observe relevant status changes.

# How to Build and Run
## Prerequisite
In order to run the entire system, you will need to install the programs below, each has their respective installation instructions linked.

- Core Flight System framework cloned from this repo
- [PX4 Autopilot Software](https://docs.px4.io/main/en/dev_setup/building_px4)
- [QGroundControl](https://docs.qgroundcontrol.com/master/en/qgc-user-guide/getting_started/download_and_install.html) or a similar ground station
- [Gazebo simulation software](https://gazebosim.org/docs/latest/getstarted/)

## Building
Once all the prereqs are installed, the only thing that will need to be built is this repository that contains Core Flight System. 

``` bash
make native_std.prep    # Sets up the build tree
make native_std.install # Compiles the software and stages it to the exe directory
```

## Running
Once cFS is built, you will be ready to boot up the demo.

### Start PX4 SITL and Gazebo
Open a separate terminal and navigate to your PX4-Autopilot source directory.

``` bash
cd ~/PX4-Autopilot
```

Then, you can choose a vehicle to model in PX4. In my case I used a quadcopter for the demo as shown below.
``` bash
make px4_sitl gz_x500
```

### Start QGroundControl
Since I am running QGroundControl on Ubuntu 22.04, I build this from source. In my case, I used the command below to start QGroundControl, but feel free to use what your specific version requires.

``` bash
~/qgroundcontrol/build/Release/QGroundControl
```

Once this is running, make sure to enable **MAVLINK Mirror** on port **localhost:14540** in the telemetry menu so that all telemetry gets forwarded to cFS via Mavlink.

### Start cFS
To boot cFS, run the command below once built.

``` bash
cd ~/cFS/build-native_std/exe/cpu1/
./core-cpu1
```

Messages should start populating your terminal now from PX4 and cFS.
<br/>
<br/>
Included in this as well is a dashboard that connects to TO_LAB of cFS and displays simulation data as well as has command buttons to start an uploaded flight path and inject failure commands to see how the quadcopter reacts to signal losses.

``` bash
cd ~/cFS/tools/Mavlink_Test
python3 mavlink_tolab_viewer.py
```
TO_LAB will automatically be enabled when the GUI starts up, so an external command is not needed to start receiving telemetry.
<br/>
<br/>
**Note** Start Loaded Mission will only work if there is a flight plan loaded into the vehicle through PX4 and QGroundControl. 

# Conclusion

While PX4 does not require NASA's Core Flight System (cFS) to operate, this project demonstrates how cFS can complement an existing autopilot by providing an additional framework for command sequencing, telemetry distribution, and fault-injection testing. Integrating cFS with PX4 SITL provided hands-on experience with flight-software architecture, MAVLink communication, real-time telemetry processing, and automated test sequences. 

Feel free to clone the repository, experiment with the implementation, and add your own features! 

