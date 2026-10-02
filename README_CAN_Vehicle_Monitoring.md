# CAN-Driven Vehicle Monitoring and Driver Assistance System

A three-node embedded vehicle monitoring and driver-assistance prototype built around **NXP LPC2129 (ARM7)** microcontrollers and the **Controller Area Network (CAN)** protocol.

The system monitors engine temperature and fuel level, controls left/right indicators, and provides an ultrasonic reverse-obstacle alert. A central LCD displays the vehicle's current status.

> **Note:** This is an educational hardware prototype. It is not a certified automotive safety system and should not be used as the sole means of preventing collisions.

## Project Objectives

- Monitor engine temperature using a DS18B20 temperature sensor.
- Read the fuel-gauge input through the LPC2129 ADC and report the level as a percentage.
- Exchange sensor values, mode information, and commands between three nodes over CAN.
- Control left and right indicator LED sequences in forward mode.
- Detect obstacles while reversing and report `SAFE`, `WARNING`, or `STOP` status.
- Display temperature, fuel percentage, vehicle mode, and reverse-alert information on a central LCD.

## System Architecture

The project is divided into three independently programmed nodes:

| Node | Main responsibilities | Key interfaces |
|---|---|---|
| **Main Node** | Reads engine temperature; receives fuel data; handles mode and indicator switches; displays system status | LPC2129, CAN, DS18B20, 20×4 LCD, external interrupts |
| **Indicator & Reverse Alert Node** | Runs indicator LED sequences in forward mode; measures obstacle distance and controls buzzer/alert LED in reverse mode | LPC2129, CAN, HC-SR04, LEDs, buzzer |
| **Fuel Node** | Reads the fuel-gauge signal using ADC, converts it to a percentage, and sends updates to the Main Node | LPC2129, ADC, fuel gauge input, CAN |

Each node uses a CAN transceiver (MCP2551) to connect to the shared CAN bus.

## Working Principle

### 1. Main Node
- Continuously reads the DS18B20 engine-temperature sensor.
- Receives fuel percentage messages from the Fuel Node over CAN.
- Uses external interrupts for the mode-selection, left-indicator, and right-indicator switches.
- Sends mode and indicator commands to the Indicator & Reverse Alert Node.
- Updates the LCD with temperature, fuel level, mode, and reverse-alert status.

### 2. Fuel Node
- Samples the fuel-gauge input using the LPC2129 on-chip ADC.
- Converts the ADC reading into a 0–100% value.
- Sends the fuel percentage over CAN periodically and when the value changes significantly.

### 3. Indicator & Reverse Alert Node
**Forward mode**
- Waits for indicator commands from the Main Node.
- Runs the left or right LED sequence as requested.
- Turns the indicator LEDs off when an OFF command is received.

**Reverse mode**
- Disables normal indicator operation and enables HC-SR04 distance measurement.
- Compares measured distance against the configured thresholds.
- Sends the reverse status and distance to the Main Node.
- Controls the buzzer and reverse-alert LED according to the detected range.

The current source sets the reverse thresholds to **100 cm** for the safe boundary and **40 cm** for the warning boundary. Adjust and validate these values for your test setup.

## Hardware Requirements

- 3 × NXP LPC2129 development boards (or compatible LPC2129 hardware)
- MCP2551 CAN transceivers
- 20×4 character LCD
- DS18B20 temperature sensor
- HC-SR04 ultrasonic sensor
- Fuel-gauge input / potentiometer for prototype testing
- Indicator LEDs and reverse-alert LED
- Buzzer
- Mode-selection and indicator switches
- CAN bus wiring and suitable termination
- USB-to-UART converter / programming connection
- Power supply and jumper wires

## Software and Tools

- Embedded C
- Keil µVision / ARM C toolchain
- Flash Magic for programming the target
- LPC2129 device support and the project files included in `source/`

## Repository Structure

```text
CAN_Vehicle_Monitoring_GitHub/
├── README.md
├── images/
│   ├── hardware-setup-1.jpg
│   ├── hardware-setup-2.jpg
│   ├── hardware-setup-3.jpg
│   └── system-output.jpg
└── source/
    ├── MainNode/
    ├── FuelNode/
    └── IndicatorReverseAlertNode/
```

The source folders contain the C/header files and Keil project files for each node. Generated compiler outputs and temporary files are intentionally excluded from this GitHub-ready copy.

## Build and Flash

Build and program each node separately:

1. Open the corresponding Keil project (`.uvproj`) from its node folder.
2. Confirm the target device, clock configuration, CAN settings, and pin connections match your hardware.
3. Build the project and resolve any toolchain or device-support issues.
4. Use Flash Magic (or your supported programming method) to flash the resulting firmware to the relevant LPC2129 board.
5. Repeat for the other two nodes.
6. Connect the CAN bus and peripherals, then power the nodes and test each function individually before testing the integrated system.

**Important:** Verify CAN wiring, common ground, transceiver supply, bus termination, and node configuration before powering the complete setup. Pin mappings and threshold values may need to be adapted to the specific development boards.

## Testing Workflow

1. Test the LCD with fixed text and numeric values.
2. Test ADC readings using a variable input and verify the fuel-percentage conversion.
3. Test the DS18B20 temperature reading independently.
4. Test the HC-SR04 trigger/echo measurement at multiple distances.
5. Test the CAN transmit and receive functions between nodes.
6. Test the mode-selection and indicator switches.
7. Integrate the three nodes and verify fuel and temperature display, indicator operation, and reverse alert behavior.

## Hardware Photos

### Development boards and wiring

![Hardware setup 1](images/hardware-setup-1.jpg)

![Hardware setup 2](images/hardware-setup-2.jpg)

![Hardware setup 3](images/hardware-setup-3.jpg)

### LCD output

![System output](images/system-output.jpg)

## Concepts Demonstrated

- Embedded C and ARM7 LPC2129 programming
- GPIO and peripheral interfacing
- ADC-based sensor reading
- External interrupts
- CAN communication between multiple nodes
- Sensor data conversion and status handling
- LCD-based real-time monitoring
- Hardware integration and module-level debugging

## Future Enhancements

- Add fault detection for sensor disconnection and CAN communication timeout.
- Add configurable obstacle-distance thresholds and improve distance filtering.
- Include data logging for fuel, temperature, and reverse-alert events.
- Add a richer display or dashboard interface.
- Perform systematic testing under different operating conditions and document measured accuracy and response time.

## Acknowledgment

This project was developed as an embedded-systems learning project using the LPC2129 platform and CAN communication.

---

**Repository note:** Review the pin mappings, wiring, and project settings against your actual board before using or modifying the firmware.
