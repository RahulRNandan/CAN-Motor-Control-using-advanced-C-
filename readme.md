# CAN-Based Motor Control System in C++

![Project Language](https://img.shields.io/badge/Language-C++-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![Platform](https://img.shields.io/badge/Platform-Linux-orange)

This project implements an advanced CAN (Controller Area Network)-based communication system in C++, designed to control motor actions in a master-slave configuration. ASCII commands sent from the master node control the motor’s operations (start, stop, forward, reverse) and speed on the slave node.

## Project Highlights

- **Advanced C++**: Uses standard libraries (`<array>`, `<cstring>`, etc.) and modern C++ best practices.
- **CAN Bus Communication**: Implements CAN protocol with raw socket programming.
- **Embedded Motor Control**: Controls motor actions and speed through ASCII command interpretation and PWM adjustments.
- **Master-Slave Design Pattern**: Master sends control signals, and the slave interprets and executes motor actions.

## Table of Contents

- [Setup and Requirements](#setup-and-requirements)
- [Architecture](#architecture)
- [Command Protocol](#command-protocol)
- [Code Walkthrough](#code-walkthrough)
- [Building and Running](#building-and-running)
- [Testing and Debugging](#testing-and-debugging)
- [Future Improvements](#future-improvements)
- [License](#license)

---

## Setup and Requirements

### Hardware Requirements
- **CAN-enabled hardware**: Two devices (master and slave) with CAN controllers.
- **CAN Bus**: Proper CAN setup to connect the devices.
- **Motor Controller**: PWM-compatible motor controller for speed control.

### Software Requirements
- **C++ Compiler**: Preferably `g++` with C++11 or later.
- **Linux Environment**: CAN libraries are used, which are best supported on Linux.
- **CAN Utilities**: `can-utils` (for testing and debugging).

### CAN Interface Setup
To configure your CAN interface, run:

```bash
sudo ip link set can0 type can bitrate 500000
sudo ip link set up can0
```

Replace `can0` with your CAN interface name.

---

## Architecture

### Master Node (`master.cpp`)

The master node sends ASCII-based control commands over CAN to the slave node. It allows user input to control motor actions and speed.

### Slave Node (`slave.cpp`)

The slave node listens for CAN messages, interprets the ASCII commands, and adjusts the motor control parameters (direction and speed) using PWM.

### Command Protocol

| Command  | Description                                |
|----------|--------------------------------------------|
| `S1`     | Start motor                                |
| `S0`     | Stop motor                                 |
| `F####`  | Move motor forward at specified speed      |
| `R####`  | Move motor in reverse at specified speed   |

Example command `F0500` means moving forward at speed 500.

---

## Code Walkthrough

### Master Code (`master.cpp`)

1. **Setup CAN Socket**: Initializes a CAN socket and binds it to the specified interface.
2. **Send Command**: Takes user input, constructs a CAN frame, and sends it.

### Slave Code (`slave.cpp`)

1. **Setup CAN Socket**: Listens for incoming CAN messages.
2. **Parse and Process Commands**:
   - Decodes the action (`S`, `F`, `R`) and speed.
   - Uses PWM for precise motor control based on received speed values.

---

## Building and Running

### Building

Compile both master and slave programs:

```bash
g++ -o master master.cpp
g++ -o slave slave.cpp
```

### Running

1. **Run the Master**:

   ```bash
   ./master
   ```

2. **Run the Slave**:

   ```bash
   ./slave
   ```

### Example Usage

- Enter commands on the master, such as `S1` to start the motor or `F0500` to move forward at speed 500.
- The slave will receive the command, interpret it, and execute the corresponding motor action.

---

## Testing and Debugging

To verify CAN communication and troubleshoot, use `can-utils`:

- **Monitor CAN traffic**:

  ```bash
  candump can0
  ```

- **Send test CAN messages**:

  ```bash
  cansend can0 001#5345303030
  ```

This command simulates sending `"S1"` to start the motor. Adjust the data field to test other commands.

---

## Future Improvements

- **Enhanced Error Handling**: Improve robustness against malformed CAN frames.
- **Dynamic Speed Control**: Real-time speed adjustments based on sensor feedback.
- **Extended Command Set**: Additional motor functions like braking or acceleration profiles.

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

