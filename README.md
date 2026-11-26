# 4WD Smart Robotic Vehicle

An ESP32-based four-wheel-drive robotic vehicle featuring Bluetooth Classic smartphone control, dual motor drivers, PWM speed control, and a separate power architecture for the motors and logic circuit.

## Project Overview

This project involves the design and development of a Bluetooth-controlled 4WD robotic vehicle using an ESP32 NodeMCU microcontroller.

The vehicle uses two TB6612FNG H-bridge motor drivers to control four DC geared motors. Commands received from a smartphone via Bluetooth allow the vehicle to move forward, backward, turn left, turn right, and stop.

The firmware is written in Embedded C++ using the Arduino framework.

## Key Features

- **4WD Differential Drive:** Independent directional control of the left and right motor groups.
- **Bluetooth Classic Control:** Wireless smartphone control using the ESP32 Bluetooth interface.
- **Dual Motor Drivers:** Two TB6612FNG drivers control four DC geared motors.
- **PWM Speed Control:** Motor speed is controlled using PWM signals.
- **Soft-Start Mechanism:** Gradually increases motor PWM duty cycle to reduce sudden startup loads.
- **Coast Stop:** Sets motor direction pins LOW before disabling PWM output.
- **Command State Tracking:** Prevents repeated execution of the same movement command.
- **Dual-Rail Power Architecture:** Separate regulated supplies for the motors and ESP32 logic circuit.

## Hardware Components

| Component            | Specification                  | Purpose                           |
| -------------------- | ------------------------------ | --------------------------------- |
| Microcontroller      | ESP32 NodeMCU (30-pin)         | Main controller                   |
| Motor Drivers        | 2 × TB6612FNG                  | Motor direction and speed control |
| DC Motors            | 4 × TT geared motors (200 RPM) | Four-wheel drive                  |
| Battery              | 2 × 18650 Li-ion cells         | Power source                      |
| BMS                  | FlyRobo 2S 20A                 | Battery protection                |
| Motor Buck Converter | XL4016E1                       | Motor power supply                |
| Logic Buck Converter | LM2596                         | ESP32 power supply                |
| Capacitors           | 1000 µF and 100 nF             | Power supply filtering            |

## Power Supply Architecture

The vehicle uses a dual-rail power architecture to reduce the effect of motor current fluctuations on the ESP32.

| Power Rail      | Voltage | Purpose                                 |
| --------------- | ------: | --------------------------------------- |
| Motor supply    |    6.4V | Powers the motor drivers                |
| Logic supply    |   5.35V | Supplies the ESP32 VIN input            |
| Regulated logic |    3.3V | ESP32-regulated output for driver logic |

### Power Distribution

1. Two 18650 Li-ion cells are connected through a 2S 20A BMS.
2. The XL4016E1 buck converter supplies the motor power rail.
3. The LM2596 buck converter supplies the ESP32 through its VIN pin.
4. The ESP32 provides regulated 3.3V logic power to the motor drivers.
5. All grounds are connected together to establish a common ground reference.

### Power Filtering

Capacitors are used to reduce supply fluctuations:

- A 1000 µF electrolytic capacitor and a 100 nF ceramic capacitor are connected across the motor supply rail.
- An additional 1000 µF capacitor is used on the logic supply rail.

This arrangement helps reduce voltage fluctuations caused by motor startup and load variations.

## Bluetooth Control

The vehicle is controlled wirelessly using Bluetooth Classic.

**Bluetooth Device Name:** `ESP32_Smart_Car`

| Command | Function      |
| ------- | ------------- |
| F       | Move Forward  |
| B       | Move Backward |
| L       | Turn Left     |
| R       | Turn Right    |
| S       | Stop          |

Lowercase commands are also supported because the firmware converts received lowercase characters to uppercase.

## GPIO Pin Configuration

The ESP32 communicates with two TB6612FNG motor drivers using the following GPIO configuration:

| Motor Driver | Motor       | PWM Pin | Direction Pins   |
| ------------ | ----------- | ------- | ---------------- |
| Left Driver  | Front Left  | GPIO 15 | GPIO 2, GPIO 4   |
| Left Driver  | Rear Left   | GPIO 22 | GPIO 19, GPIO 21 |
| Right Driver | Front Right | GPIO 25 | GPIO 26, GPIO 27 |
| Right Driver | Rear Right  | GPIO 14 | GPIO 12, GPIO 13 |

## Motor Control Logic

The vehicle uses differential steering:

- **Forward:** Both motor groups rotate forward.
- **Backward:** Both motor groups rotate in reverse.
- **Left:** Left motor group reverses while the right motor group moves forward.
- **Right:** Left motor group moves forward while the right motor group reverses.
- **Stop:** Direction pins are set LOW before PWM outputs are disabled.

## Firmware Details

The firmware is located in:

`firmware/4WD_Smart_Car/4WD_Smart_Car.ino`

Important implementation details:

- Bluetooth communication using the ESP32 BluetoothSerial library.
- GPIO-based motor direction control.
- PWM-based motor speed control.
- Gradual PWM ramp-up from 110 to 240.
- Command state tracking to avoid repeatedly executing the same command.
- Coast-stop implementation.

## Software and Development Tools

- Arduino IDE
- ESP32 Arduino Board Support Package
- Embedded C++
- Bluetooth Classic Android controller application
- Git and GitHub for version control

## How to Run

1. Install the Arduino IDE.
2. Install ESP32 board support in the Arduino IDE.
3. Open `firmware/4WD_Smart_Car/4WD_Smart_Car.ino`.
4. Connect the ESP32 to your computer using USB.
5. Select the appropriate ESP32 board and COM port.
6. Upload the firmware.
7. Power the robotic vehicle using its battery system.
8. Pair your smartphone with the Bluetooth device named `ESP32_Smart_Car`.
9. Send the commands F, B, L, R, or S using a Bluetooth controller application.

**Note:** Ensure that the motor drivers, power rails, and common ground are connected correctly before powering the vehicle.

## Project Limitations

- The vehicle currently supports manual Bluetooth control.
- Autonomous navigation and obstacle avoidance are not implemented.
- The current firmware does not implement encoder-based closed-loop speed or position control.

## Future Improvements

- Integration of wheel encoders for closed-loop odometry.
- Wi-Fi-based web control.
- Obstacle detection and avoidance.
- Improved speed regulation using feedback control.

## Repository Structure

```text
4WD-Smart-Robotic-Vehicle/
│
├── firmware/
│   └── 4WD_Smart_Car/
│       └── 4WD_Smart_Car.ino
│
├── images/
│
├── hardware/
│
├── documentation/
│
└── README.md
```

## Acknowledgement

This project was developed as part of the Computer Architecture and Organization coursework at Symbiosis Institute of Technology.
