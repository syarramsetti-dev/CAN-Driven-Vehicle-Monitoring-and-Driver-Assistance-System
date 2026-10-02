# 🚗 CAN-Driven Vehicle Monitoring and Driver Assistance System

**A Three-Node Embedded Automotive Monitoring System Using LPC2129 and CAN Communication**

![Embedded Systems](https://img.shields.io/badge/Domain-Embedded%20Systems-blue)
![Microcontroller](https://img.shields.io/badge/Controller-LPC2129-orange)
![Language](https://img.shields.io/badge/Language-Embedded%20C-green)
![Communication](https://img.shields.io/badge/Protocol-CAN-red)

## 📌 Project Overview

The **CAN-Driven Vehicle Monitoring and Driver Assistance System** is an embedded automotive prototype designed to monitor important vehicle parameters, manage indicator operations, and assist the driver during reverse parking.

The system is developed using **three LPC2129 ARM7 microcontrollers** connected through a CAN (Controller Area Network) bus.

It monitors engine temperature and fuel level, controls left and right indicators, and detects nearby obstacles during reverse operation using an ultrasonic sensor. The main controller displays the received information on a 20×4 LCD.

### Key Features

* 🌡️ Engine temperature monitoring using DS18B20.
* ⛽ Fuel level measurement using an ADC.
* 🔗 Communication between three controllers using CAN.
* 🚦 Left and right indicator control.
* 📡 Ultrasonic-based reverse obstacle detection.
* 🔊 Buzzer-based distance warning.
* 💡 Visual reverse-alert indication.
* 🖥️ Real-time monitoring using a 20×4 LCD.

---

## 🏗️ 1. Overall System Architecture

The system is divided into three independent embedded nodes. Each node performs a specific task and exchanges relevant information through a shared CAN bus.

```mermaid
flowchart TB
    subgraph MAIN["MAIN NODE"]
        direction TB
        T["DS18B20<br/>Temperature Sensor"]
        SW["Mode / Left / Right<br/>Switches"]
        MCU1["LPC2129<br/>ARM7 Controller"]
        LCD["20×4 LCD"]
        CAN1["MCP2551<br/>CAN Transceiver"]
        T --> MCU1
        SW --> MCU1
        MCU1 --> LCD
        MCU1 <--> CAN1
    end

    subgraph FUEL["FUEL MONITORING NODE"]
        direction TB
        FG["Fuel Gauge /<br/>Analog Input"]
        ADC["LPC2129<br/>ADC"]
        MCU2["LPC2129<br/>Fuel Processing"]
        CAN2["MCP2551<br/>CAN Transceiver"]
        FG --> ADC
        ADC --> MCU2
        MCU2 <--> CAN2
    end

    subgraph ALERT["INDICATOR AND REVERSE ALERT NODE"]
        direction TB
        US["HC-SR04<br/>Ultrasonic Sensor"]
        MCU3["LPC2129<br/>Alert Controller"]
        LED["Left / Right<br/>Indicator LEDs"]
        BUZ["Buzzer"]
        RLED["Reverse Alert LED"]
        CAN3["MCP2551<br/>CAN Transceiver"]
        US --> MCU3
        MCU3 --> LED
        MCU3 --> BUZ
        MCU3 --> RLED
        MCU3 <--> CAN3
    end

    CAN1 <-->|"CANH / CANL"| BUS(("CAN BUS"))
    CAN2 <-->|"CANH / CANL"| BUS
    CAN3 <-->|"CANH / CANL"| BUS

    classDef controller fill:#dcecff,stroke:#2865a5,color:#143454,stroke-width:1.5px
    classDef interface fill:#fff0d5,stroke:#b77b15,color:#513600
    classDef network fill:#dff3e4,stroke:#30834b,color:#164b29
    class MCU1,MCU2,MCU3 controller
    class CAN1,CAN2,CAN3 interface
    class BUS network
```

### Architecture Explanation

| Node                             | Hardware                                 | Main responsibility                                                  |
| -------------------------------- | ---------------------------------------- | -------------------------------------------------------------------- |
| Main Node                        | LPC2129, DS18B20, switches, LCD, MCP2551 | Central monitoring, mode control, indicator commands and LCD display |
| Fuel Node                        | LPC2129, analog fuel input, MCP2551      | Fuel measurement, ADC conversion and CAN transmission                |
| Indicator and Reverse Alert Node | LPC2129, HC-SR04, LEDs, buzzer, MCP2551  | Indicator control and reverse obstacle detection                     |

**CAN connection:** Each LPC2129's internal CAN controller communicates with an external MCP2551 transceiver. The transceivers connect the three nodes to the shared CANH and CANL bus.

---

## 🔌 2. Main Node Hardware Block Diagram

The Main Node acts as the central controller and display unit.

```mermaid
flowchart TB
    subgraph INPUT["INPUT SECTION"]
        DS["DS18B20<br/>Temperature Sensor"]
        MODE["Mode Selection Switch"]
        LEFT["Left Indicator Switch"]
        RIGHT["Right Indicator Switch"]
    end

    subgraph PROCESS["MAIN CONTROLLER"]
        MCU["LPC2129 ARM7"]
        GPIO["GPIO / External Interrupts"]
        TEMP["Temperature Interface"]
        CANCTL["Internal CAN Controller"]
    end

    subgraph OUTPUT["OUTPUT AND COMMUNICATION"]
        LCD["20×4 LCD"]
        TR["MCP2551<br/>CAN Transceiver"]
        BUS[("Shared CAN Bus")]
    end

    DS --> TEMP
    MODE --> GPIO
    LEFT --> GPIO
    RIGHT --> GPIO
    TEMP --> MCU
    GPIO --> MCU
    MCU <--> CANCTL
    MCU --> LCD
    CANCTL <--> TR
    TR <--> BUS

    classDef hw fill:#e7f0ff,stroke:#4b78ae,color:#183c66
    classDef proc fill:#e1f3e7,stroke:#398455,color:#205132
    class DS,MODE,LEFT,RIGHT,LCD,TR hw
    class MCU,GPIO,TEMP,CANCTL proc
```

### Main Node Working

1. The microcontroller initializes GPIO, CAN, LCD and sensor interfaces.
2. The DS18B20 provides engine-temperature readings.
3. Switch inputs are monitored to identify vehicle mode and indicator requests.
4. The Main Node receives fuel information from the Fuel Node.
5. It sends mode and indicator commands to the Indicator and Reverse Alert Node.
6. During reverse mode, it receives obstacle-alert status from the other node.
7. The LCD is refreshed with the current temperature, fuel percentage, mode and reverse status.

---

## ⛽ 3. Fuel Node Hardware Block Diagram

The Fuel Node is responsible for measuring the fuel level and sending the calculated percentage to the Main Node.

```mermaid
flowchart LR
    A["Fuel Gauge /<br/>Analog Voltage"] --> B["ADC Input<br/>LPC2129"]
    B --> C["ADC Conversion"]
    C --> D["Fuel Percentage<br/>Calculation"]
    D --> E["LPC2129<br/>CAN Controller"]
    E <--> F["MCP2551<br/>Transceiver"]
    F <--> G[("CAN Bus")]
```

### Fuel Node Working

1. Initialize the ADC and CAN interfaces.
2. Read the analog fuel-level input.
3. Convert the ADC value into a corresponding fuel percentage using the programmed mapping.
4. Send the fuel percentage over CAN.
5. Repeat the measurement and transmission according to the firmware's update logic.

**Important:** The accuracy of the fuel reading depends on the actual sensor characteristics and calibration mapping.

---

## 🚦 4. Indicator and Reverse Alert Node Hardware Block Diagram

This node manages the indicator LEDs and reverse obstacle detection.

```mermaid
flowchart TB
    BUS[("Shared CAN Bus")]
    TR["MCP2551<br/>CAN Transceiver"]
    CAN["Internal CAN<br/>Controller"]
    MCU["LPC2129<br/>ARM7 Controller"]

    subgraph SENSORS["SENSOR INPUT"]
        US["HC-SR04"]
        TRIG["Trigger Output"]
        ECHO["Echo Input"]
    end

    subgraph OUTPUTS["OUTPUT SECTION"]
        L["Left Indicator LEDs"]
        R["Right Indicator LEDs"]
        B["Buzzer"]
        AL["Reverse Alert LED"]
    end

    BUS <--> TR
    TR <--> CAN
    CAN <--> MCU
    MCU --> TRIG
    TRIG --> US
    US --> ECHO
    ECHO --> MCU
    MCU --> L
    MCU --> R
    MCU --> B
    MCU --> AL
```

### Forward Mode

* The node receives indicator commands over CAN.
* It activates the requested left or right indicator sequence.
* The LEDs blink according to the programmed timing.
* When an OFF command is received, the indicator outputs are switched off.

### Reverse Mode

* The node activates the ultrasonic distance-measurement process.
* The HC-SR04 measures the time between the transmitted ultrasonic pulse and the received echo.
* The controller calculates obstacle distance.
* It determines the alert state from the configured distance thresholds.
* The buzzer and reverse-alert LED are controlled accordingly.
* The alert status is transmitted to the Main Node through CAN.

### Distance Alert Logic

```mermaid
flowchart TD
    A([Reverse Mode Active]) --> B["Trigger HC-SR04"]
    B --> C["Measure Echo Duration"]
    C --> D["Calculate Distance"]
    D --> E{"Distance Classification"}
    E -->|"Safe range"| F["SAFE"]
    E -->|"Warning range"| G["WARNING"]
    E -->|"Critical range"| H["STOP"]
    F --> I["Buzzer OFF"]
    G --> J["Intermittent Buzzer"]
    H --> K["Continuous Buzzer<br/>Alert LED ON"]
    I --> L["Send Alert Status via CAN"]
    J --> L
    K --> L
    L --> M["Repeat Measurement"]
    M --> B
```

The source description identifies 100 cm and 40 cm as the configured distance boundaries. The exact inequalities and boundary handling should be verified in the source code.

---

## 🔄 5. Complete Project Workflow

The following diagram represents the overall system sequence from initialization to monitoring.

```mermaid
flowchart TD
    START([Power ON]) --> INIT["Initialize All Three LPC2129 Nodes"]
    INIT --> CAN["Initialize CAN Communication"]
    CAN --> SENSOR["Initialize Sensors, Switches and LCD"]
    SENSOR --> LOOP["Start Continuous Monitoring"]

    LOOP --> TEMP["Read Temperature"]
    TEMP --> FUEL["Receive Fuel Data"]
    FUEL --> MODE{"Check Vehicle Mode"}

    MODE -->|"Forward"| IND["Read Indicator Switches"]
    IND --> CMD["Send Indicator Command via CAN"]
    CMD --> INDNODE["Indicator Node Controls LEDs"]

    MODE -->|"Reverse"| DIST["Measure Obstacle Distance"]
    DIST --> STATUS["Generate SAFE / WARNING / STOP"]
    STATUS --> ALERT["Control Buzzer and Alert LED"]
    ALERT --> TX["Transmit Alert Status via CAN"]

    INDNODE --> DISPLAY["Update LCD"]
    TX --> DISPLAY
    DISPLAY --> LOOP
```

### Communication Flow Between Nodes

| Sender                           | Receiver                         | Information                 |
| -------------------------------- | -------------------------------- | --------------------------- |
| Fuel Node                        | Main Node                        | Fuel percentage             |
| Main Node                        | Indicator and Reverse Alert Node | Mode and indicator commands |
| Indicator and Reverse Alert Node | Main Node                        | Reverse-alert status        |

CAN communication allows each node to exchange information using CAN identifiers and message frames.

---

## 🧰 6. Hardware Requirements

| Component                   |    Quantity | Purpose                        |
| --------------------------- | ----------: | ------------------------------ |
| LPC2129 Development Board   |           3 | Processing for the three nodes |
| MCP2551 CAN Transceiver     |           3 | CAN physical-layer interface   |
| 20×4 LCD                    |           1 | Central monitoring display     |
| DS18B20 Temperature Sensor  |           1 | Temperature measurement        |
| HC-SR04 Ultrasonic Sensor   |           1 | Obstacle detection             |
| Fuel Gauge / Analog Input   |           1 | Fuel-level measurement         |
| Indicator LEDs              | As required | Left and right indicators      |
| Reverse Alert LED           |           1 | Reverse warning output         |
| Buzzer                      |           1 | Audible alert                  |
| Mode and Indicator Switches |           3 | Mode and indicator inputs      |
| CAN Bus Wiring              | As required | Network connection             |
| Regulated Power Supply      | As required | Board and sensor power         |
| Jumper Wires                | As required | Peripheral connections         |

The MCP2551 interfaces to the CAN controller and bus. Check the transceiver voltage, MCU logic compatibility and board-specific wiring before connecting the system.

## 💻 7. Software Requirements

* **Programming Language:** Embedded C
* **Microcontroller:** LPC2129 (ARM7)
* **IDE:** Keil µVision
* **Compiler:** Compatible ARM toolchain
* **Programming Tool:** Flash Magic
* **Communication Protocol:** CAN
* **Peripheral Interfaces:** GPIO, ADC, external interrupts, sensor interfaces and LCD

## 🛠️ 8. Implementation Procedure

1. Create three separate firmware projects.
2. Configure the LPC2129 clock and peripheral interfaces.
3. Test the LCD display independently.
4. Test temperature measurement using DS18B20.
5. Test ADC conversion and fuel-level mapping.
6. Configure and test external interrupts for switch inputs.
7. Test HC-SR04 distance measurement.
8. Implement CAN frame transmission and reception.
9. Integrate the Fuel Node with the Main Node.
10. Integrate the Indicator and Reverse Alert Node.
11. Verify the forward and reverse operating modes.
12. Test the complete three-node system and document the results.

## 🧪 9. Testing and Validation

| Test                    | Expected behavior                                 |
| ----------------------- | ------------------------------------------------- |
| LCD test                | Displays text and numeric values                  |
| Temperature sensor test | Provides a readable temperature measurement       |
| ADC test                | ADC reading changes with analog input             |
| Fuel calculation test   | Converts ADC input into the programmed percentage |
| CAN communication test  | Correct data is received by the intended node     |
| Switch test             | Mode and indicator requests are detected          |
| Forward mode test       | Requested indicator LEDs operate                  |
| Reverse mode test       | Ultrasonic distance is measured                   |
| Warning test            | Buzzer and alert state respond to obstacle range  |
| Integrated system test  | Dashboard and alert information update correctly  |

The expected behaviors are functional targets. Actual accuracy, timing and reliability should be established through measurements.

## 📷 10. Hardware Implementation

The physical prototype consists of three LPC2129 development boards interconnected using jumper wires and CAN transceivers.

The hardware setup includes:

* A central LCD for vehicle information.
* A temperature sensor interface.
* A fuel-level measurement input.
* Indicator LED connections.
* An ultrasonic obstacle-detection module.
* A buzzer and visual alert output.

To display your actual prototype photos, upload them into an `images` folder in your repository and add the following section, changing the filenames to match your uploaded images.

```markdown
## Hardware Setup

![Three-node hardware setup](images/hardware-setup.jpg)

## LCD Output

![LCD display](images/lcd-output.jpg)
```

## 🚀 11. Future Enhancements

* CAN bus error monitoring and node timeout detection.
* Sensor disconnection detection.
* Fuel-level calibration and improved measurement accuracy.
* Ultrasonic reading filtering.
* Data logging for fuel and temperature.
* Additional vehicle diagnostics.
* Wireless monitoring and IoT dashboard integration.
* More extensive hardware validation under different operating conditions.

## 📚 12. Learning Outcomes

* ARM7 LPC2129 microcontroller programming.
* Embedded C development.
* ADC and GPIO interfacing.
* External interrupt handling.
* CAN bus communication.
* Temperature and ultrasonic sensor interfacing.
* Multi-node embedded architecture.
* Real-time data monitoring.
* Hardware integration and debugging.

---

## Project Information

| Category               | Details                                                    |
| ---------------------- | ---------------------------------------------------------- |
| Project                | CAN-Driven Vehicle Monitoring and Driver Assistance System |
| Domain                 | Embedded Systems / Automotive Electronics                  |
| Microcontroller        | LPC2129 ARM7                                               |
| Programming Language   | Embedded C                                                 |
| Communication Protocol | CAN                                                        |
| CAN Transceiver        | MCP2551                                                    |
| Development IDE        | Keil µVision                                               |
| Programming Tool       | Flash Magic                                                |
| Project Type           | Academic Embedded Systems Prototype                        |

**Disclaimer:** This is an educational prototype. It is not a certified automotive control or collision-prevention system and must not be used as a replacement for production vehicle safety systems.
