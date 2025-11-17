# Horus Project - Technical Architecture

This document provides a comprehensive overview of the Horus Project's technical architecture, design decisions, and implementation details.

## Table of Contents

- [System Overview](#system-overview)
- [Architecture Diagram](#architecture-diagram)
- [Component Details](#component-details)
- [Data Flow](#data-flow)
- [Design Patterns](#design-patterns)
- [Technology Stack](#technology-stack)
- [Performance Considerations](#performance-considerations)
- [Future Architecture](#future-architecture)

## System Overview

Horus Project is a real-time Primary Flight Display (PFD) system consisting of:

1. **Hardware Layer**: ESP32 + MPU6050 IMU sensor
2. **Firmware Layer**: Arduino-based sensor data acquisition
3. **Communication Layer**: Serial protocol (UART)
4. **Application Layer**: Qt-based GUI application
5. **Presentation Layer**: OpenGL-accelerated rendering

### System Characteristics

- **Real-time**: 50Hz sensor data, 60+ FPS rendering
- **Cross-platform**: Windows, macOS, Linux
- **Modular**: Separate firmware and application components
- **Extensible**: Easy to add new sensors/instruments
- **Lightweight**: Minimal dependencies, low resource usage

## Architecture Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                     Horus Project System                     │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────┐       ┌──────────────────────────────┐
│   Hardware Layer    │       │     Application Layer         │
│                     │       │                               │
│  ┌───────────────┐  │       │  ┌─────────────────────────┐ │
│  │   MPU6050     │  │  USB  │  │      main.cpp           │ │
│  │   IMU Sensor  │◄─┼───────┼─►│  - Setup & event loop   │ │
│  │  (I2C 400kHz) │  │ Serial│  │  - Serial communication │ │
│  └───────┬───────┘  │ 115200│  │  - Window management    │ │
│          │I2C       │       │  └─────────┬───────────────┘ │
│  ┌───────▼───────┐  │       │            │                 │
│  │    ESP32      │  │       │  ┌─────────▼───────────────┐ │
│  │ - Read sensors│  │       │  │  AttitudeIndicator      │ │
│  │ - Sensor fusion│ │       │  │  (attitudeindicator.h)  │ │
│  │ - Format data │  │       │  │                         │ │
│  │ - UART TX     │  │       │  │  - Pitch/Roll display   │ │
│  └───────────────┘  │       │  │  - Instrument rendering │ │
│                     │       │  │  - QPainter graphics    │ │
└─────────────────────┘       │  │  - Real-time updates    │ │
                              │  └─────────────────────────┘ │
                              │                               │
                              │  ┌─────────────────────────┐ │
                              │  │   Qt Framework          │ │
                              │  │  - QSerialPort          │ │
                              │  │  - QWidget              │ │
                              │  │  - QPainter             │ │
                              │  │  - QTimer               │ │
                              │  └─────────────────────────┘ │
                              └───────────────────────────────┘
```

## Component Details

### 1. Hardware Layer

#### MPU6050 IMU Sensor

**Specifications:**
- 6-axis motion tracking (3-axis gyro + 3-axis accel)
- I2C interface at 400 kHz
- 16-bit ADC resolution
- ±2g to ±16g accelerometer range
- ±250°/s to ±2000°/s gyroscope range
- Built-in temperature sensor
- Power: 3.3V, ~3.9mA typical

**Sensor Fusion:**
- Complementary filter (98% gyro, 2% accel)
- 50Hz update rate
- Pitch/Roll accuracy: ±1°

#### ESP32 Microcontroller

**Specifications:**
- Xtensa dual-core 32-bit processor @ 240 MHz
- 520 KB SRAM
- WiFi & Bluetooth (not used in current version)
- 34 GPIO pins
- Hardware UART, I2C, SPI
- USB-to-Serial for programming and data

**Firmware Responsibilities:**
- I2C communication with MPU6050
- Sensor data acquisition
- Complementary filter implementation
- Data formatting (CSV)
- Serial transmission (UART)

### 2. Firmware Layer

**File**: `firmware/mpu6050_imu.ino`

**Architecture:**

```cpp
// Initialization
void setup() {
  - Initialize serial (115200 baud)
  - Initialize I2C bus
  - Detect and initialize MPU6050
  - Apply calibration offsets
  - Configure sensor ranges
}

// Main loop (50Hz)
void loop() {
  - Read raw sensor data (6 values)
  - Apply complementary filter
  - Calculate pitch and roll
  - Format as CSV string
  - Transmit via serial
  - Delay for 50Hz rate
}
```

**Key Functions:**
- `calculatePitch()`: Fuse accel + gyro for pitch
- `calculateRoll()`: Fuse accel + gyro for roll
- `readSensors()`: Read raw MPU6050 data
- `transmitData()`: Send CSV over serial

### 3. Communication Layer

**Protocol**: CSV over UART (see [SERIAL_PROTOCOL.md](SERIAL_PROTOCOL.md))

```
pitch,roll\n
```

**Characteristics:**
- Text-based (human-readable)
- Line-oriented (newline-terminated)
- Simple parsing (split on comma)
- Error-tolerant (discard malformed)

**Data Rate:**
- 50 Hz update rate
- ~15 bytes per message
- ~750 bytes/second bandwidth
- 6.5% utilization of 115200 baud

### 4. Application Layer

#### main.cpp

**Responsibilities:**
- Application setup and initialization
- Window creation and configuration
- Serial port management
- Event loop and timers
- Serial data reading and parsing
- Data forwarding to AttitudeIndicator

**Key Components:**

```cpp
// Main application class
class HorusApp : public QMainWindow {
  - QSerialPort* serialPort
  - AttitudeIndicator* attitudeWidget
  - QTimer* updateTimer
  - QTimer* simulationTimer
}

// Core functions
void setupSerial()        // Configure serial port
void readSerialData()     // Read and parse incoming data
void updateDisplay()      // Refresh display
void simulateData()       // Generate fake data (no hardware)
```

**State Machine:**

```
┌─────────┐    Serial Port   ┌───────────┐
│  Idle   ├──────Found──────►│ Connected │
└─────────┘                  └─────┬─────┘
                                   │
                              Data Received
                                   │
                             ┌─────▼─────┐
                        ┌───►│  Reading  │
                        │    └─────┬─────┘
                        │          │
                   Parse & Update  │
                        │          │
                        └──────────┘
```

#### AttitudeIndicator Widget

**File**: `attitudeindicator.h`

**Class Structure:**

```cpp
class AttitudeIndicator : public QWidget {
private:
  // State
  double m_pitch;              // -90 to +90 degrees
  double m_roll;               // -180 to +180 degrees
  double m_heading;            // 0 to 360 degrees
  double m_altitude;           // Feet
  double m_speed;              // Knots
  double m_batteryVoltage;     // Volts
  double m_batteryPercent;     // 0-100%

  // Configuration
  int m_zoomFactor;            // Pitch ladder zoom
  QColor m_skyColor;           // Sky gradient
  QColor m_groundColor;        // Ground gradient

public:
  void updatePitchRoll(double pitch, double roll);
  void paintEvent(QPaintEvent* event) override;

private:
  void drawSky(QPainter& painter);
  void drawGround(QPainter& painter);
  void drawPitchLadder(QPainter& painter);
  void drawRollIndicator(QPainter& painter);
  void drawAircraftSymbol(QPainter& painter);
  void drawAltitudeTape(QPainter& painter);
  void drawSpeedTape(QPainter& painter);
  void drawHeadingIndicator(QPainter& painter);
  void drawBatteryIndicator(QPainter& painter);
};
```

**Rendering Pipeline:**

```
paintEvent() called (60+ FPS)
     │
     ├─► drawSky()              (gradient fill)
     ├─► drawGround()           (gradient fill)
     ├─► drawPitchLadder()      (lines + numbers)
     ├─► drawRollIndicator()    (arc + marks)
     ├─► drawAircraftSymbol()   (fixed W-shape)
     ├─► drawAltitudeTape()     (right side)
     ├─► drawSpeedTape()        (left side)
     ├─► drawHeadingIndicator() (top)
     └─► drawBatteryIndicator() (corner)
```

### 5. Presentation Layer

**Graphics Engine**: Qt QPainter (OpenGL-accelerated)

**Rendering Features:**
- Anti-aliasing for smooth lines
- Gradient fills for sky/ground
- Affine transformations (rotation, translation)
- Font rendering (Nimbus Mono)
- Clipping for vertical tapes

**Coordinate System:**

```
       (0,0)
         ├────────────────► X (width)
         │
         │    Widget Space
         │
         ▼ Y (height)

Center: (width/2, height/2)
```

**Transformation Stack:**

```cpp
painter.save();
  painter.translate(centerX, centerY);
  painter.rotate(-roll);              // Bank angle
  painter.translate(0, pitch * scale); // Pitch offset
  // Draw instruments
painter.restore();
```

## Data Flow

### Sensor to Display Pipeline

```
MPU6050           ESP32           Serial          Application         Display
Sensor          Firmware         Port             (Qt)              (Screen)
   │                │              │                │                   │
   ├─ I2C Data ────►│              │                │                   │
   │                │              │                │                   │
   │                ├─ Process ───►│                │                   │
   │                │  (Filter)    │                │                   │
   │                │              │                │                   │
   │                ├─ CSV Format─►│                │                   │
   │                │              │                │                   │
   │                │              ├─ UART TX ─────►│                   │
   │                │              │   (115200)     │                   │
   │                │              │                ├─ Parse ──────────►│
   │                │              │                │   (CSV)           │
   │                │              │                │                   │
   │                │              │                ├─ Update ─────────►│
   │                │              │                │  Widget           │
   │                │              │                │                   │
   │                │              │                │                   ├─ Render
   │                │              │                │                   │  (60 FPS)
   │                │              │                │                   │
   └────────────────┴──────────────┴────────────────┴───────────────────┘
     ~20ms            ~5ms           <1ms             ~1ms              ~16ms
```

**Total Latency**: ~42ms (24 FPS equivalent for data updates)
**Display FPS**: 60+ FPS (smooth visual update)

### Timing Analysis

| Stage                | Time      | Rate   |
|----------------------|-----------|--------|
| MPU6050 sampling     | 20ms      | 50 Hz  |
| Sensor fusion        | <1ms      | -      |
| Serial transmission  | <1ms      | -      |
| Serial reception     | <1ms      | -      |
| CSV parsing          | <0.1ms    | -      |
| Widget update        | <0.5ms    | -      |
| Display rendering    | 16ms      | 60 FPS |

## Design Patterns

### 1. Model-View-Controller (MVC)

- **Model**: Sensor data (pitch, roll, heading, etc.)
- **View**: AttitudeIndicator widget (visual presentation)
- **Controller**: main.cpp (serial communication, data routing)

### 2. Observer Pattern

```cpp
// Serial port notifies on data arrival
QObject::connect(serialPort, &QSerialPort::readyRead,
                 this, &HorusApp::readSerialData);

// Timer triggers display updates
QObject::connect(updateTimer, &QTimer::timeout,
                 this, &HorusApp::updateDisplay);
```

### 3. State Pattern

```cpp
enum class AppState {
  Idle,
  Connecting,
  Connected,
  Simulating,
  Error
};
```

### 4. Strategy Pattern

Different data sources (hardware, simulation):

```cpp
if (serialPort->isOpen()) {
  // Hardware strategy
  readSerialData();
} else {
  // Simulation strategy
  generateSimulatedData();
}
```

## Technology Stack

### Languages & Standards

- **C++17**: Modern C++ features
- **Arduino C++**: ESP32 firmware
- **CMake**: Build system
- **Qt 6.x**: GUI framework

### Libraries & Frameworks

**Application:**
- Qt Core (event loop, objects)
- Qt Widgets (GUI components)
- Qt SerialPort (UART communication)
- Qt GUI (graphics, fonts)

**Firmware:**
- Arduino Core for ESP32
- Wire library (I2C)
- MPU6050 library (sensor)

### Build Tools

- CMake 3.16+
- GCC 7+ / Clang 5+ / MSVC 2017+
- Arduino IDE 1.8.13+

### Platform Support

| Platform     | Tested Versions        | Status |
|--------------|------------------------|--------|
| Windows      | 10, 11                 | ✅      |
| macOS        | 12+                    | ✅      |
| Linux        | Ubuntu 20.04+          | ✅      |
| ESP32        | Arduino Core 2.0.x     | ✅      |

## Performance Considerations

### Optimization Techniques

1. **Double Buffering**: Smooth rendering without flicker
2. **Clipping Regions**: Only redraw changed areas
3. **Pre-calculated Values**: Trigonometry cached where possible
4. **Integer Math**: Use int where possible (faster than float)
5. **Event-driven Updates**: No polling loops

### Resource Usage

**Application (typical):**
- CPU: <5% (single core)
- RAM: ~50 MB
- GPU: Minimal (2D graphics only)

**ESP32 (typical):**
- CPU: ~10% (single core)
- RAM: ~20 KB
- Flash: ~200 KB

### Performance Bottlenecks

Identified bottlenecks and mitigations:

1. **Serial Parsing**: Minimal (text processing is fast)
2. **QPainter Rendering**: Optimized with clipping
3. **Sensor Fusion**: Lightweight complementary filter

### Scalability

Current architecture supports:
- Additional sensors (just extend CSV protocol)
- Multiple instruments (add more widgets)
- Higher data rates (up to ~500Hz with binary protocol)

## Future Architecture

### Planned Enhancements

#### 1. Plugin Architecture

```cpp
class InstrumentPlugin {
  virtual void processData(const SensorData& data) = 0;
  virtual void render(QPainter& painter) = 0;
};
```

#### 2. Network Communication

```
ESP32 ─[WiFi]─► Horus App ─[TCP/IP]─► Remote Display
```

#### 3. Data Recording

```cpp
class DataRecorder {
  void record(const SensorData& data);
  void playback(double speed);
  void export(const QString& filename);
};
```

#### 4. Multi-Sensor Fusion

```
GPS + IMU + Barometer + Magnetometer
         │
         ▼
   Kalman Filter
         │
         ▼
  Fused State Estimate
```

## References

- [Qt Documentation](https://doc.qt.io/)
- [ESP32 Technical Reference](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)
- [MPU6050 Datasheet](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf)
- [Serial Protocol Specification](SERIAL_PROTOCOL.md)

---

**Author**: João Oliveira ([@joaoliveira6704](https://github.com/joaoliveira6704))
**Last Updated**: 2025-01-17
