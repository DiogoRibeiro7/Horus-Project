# ESP32 Setup Guide

This guide will help you set up your ESP32 development board for use with the Horus Project Primary Flight Display.

## Table of Contents

- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Wiring Instructions](#wiring-instructions)
- [Arduino IDE Setup](#arduino-ide-setup)
- [Uploading the Firmware](#uploading-the-firmware)
- [Troubleshooting](#troubleshooting)

## Hardware Requirements

### Required Components

1. **ESP32 Development Board** (any variant)
   - ESP32-DevKitC
   - ESP32-WROOM-32
   - ESP32-WROVER
   - NodeMCU-32S
   - Or any other ESP32 board

2. **MPU6050 IMU Sensor Module**
   - 6-axis (3-axis gyroscope + 3-axis accelerometer)
   - GY-521 module (common and inexpensive)
   - Supports I2C communication

3. **Connection Cables**
   - 4x Female-to-Female jumper wires
   - USB cable (USB-A to Micro-USB or USB-C, depending on your ESP32)

### Optional Components

- **Breadboard** - For prototyping and testing
- **Power Supply** - If not powering via USB
- **Case/Enclosure** - To protect the electronics

## Software Requirements

### Required Software

1. **Arduino IDE** (v1.8.13 or newer, or v2.x)
   - Download: https://www.arduino.cc/en/software

2. **ESP32 Board Support**
   - Installed via Arduino IDE Board Manager

3. **MPU6050 Library**
   - Installed via Arduino IDE Library Manager

### Installation Instructions

#### 1. Install Arduino IDE

Download and install the Arduino IDE for your operating system from the official website.

#### 2. Add ESP32 Board Support

1. Open Arduino IDE
2. Go to **File → Preferences**
3. In "Additional Board Manager URLs", add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Click **OK**
5. Go to **Tools → Board → Boards Manager**
6. Search for "ESP32"
7. Install "ESP32 by Espressif Systems"
8. Wait for installation to complete

#### 3. Install MPU6050 Library

1. Go to **Sketch → Include Library → Manage Libraries**
2. Search for "MPU6050"
3. Install "MPU6050 by Electronic Cats" or "Adafruit MPU6050"
4. Also install dependencies if prompted (e.g., Adafruit Sensor, Adafruit BusIO)

## Wiring Instructions

### Pin Connections

Connect the MPU6050 to the ESP32 as follows:

| MPU6050 Pin | ESP32 Pin | Wire Color (Suggested) |
|-------------|-----------|------------------------|
| VCC         | 3.3V      | Red                    |
| GND         | GND       | Black                  |
| SCL         | GPIO 22   | Yellow                 |
| SDA         | GPIO 21   | Green                  |

### Wiring Diagram

```
ESP32                    MPU6050
┌─────────────┐         ┌──────────┐
│             │         │          │
│   3.3V ●────┼─────────┼─● VCC   │
│             │         │          │
│    GND ●────┼─────────┼─● GND   │
│             │         │          │
│ GPIO22 ●────┼─────────┼─● SCL   │
│             │         │          │
│ GPIO21 ●────┼─────────┼─● SDA   │
│             │         │          │
│      USB ●  │         │          │
└─────────────┘         └──────────┘
```

### Important Notes

- **Use 3.3V**, not 5V! The MPU6050 operates at 3.3V
- Double-check connections before powering on
- Keep wires short to reduce noise
- Ensure the MPU6050 is mounted securely and flat

## Arduino IDE Setup

### 1. Select the Board

1. Go to **Tools → Board → ESP32 Arduino**
2. Select your ESP32 board variant:
   - **ESP32 Dev Module** (most common)
   - Or your specific board model

### 2. Configure Board Settings

Set the following in **Tools** menu:

- **Upload Speed**: 115200
- **Flash Frequency**: 80MHz
- **Flash Mode**: QIO
- **Flash Size**: 4MB (32Mb)
- **Partition Scheme**: Default 4MB with spiffs
- **Core Debug Level**: None (or Info for debugging)

### 3. Select the Port

1. Connect your ESP32 to your computer via USB
2. Go to **Tools → Port**
3. Select the port that appears when you connect the ESP32:
   - **Windows**: COMx (e.g., COM3)
   - **macOS**: /dev/cu.usbserial-xxxx
   - **Linux**: /dev/ttyUSB0 or /dev/ttyACM0

## Uploading the Firmware

### 1. Open the Firmware Sketch

1. In Arduino IDE, go to **File → Open**
2. Navigate to the Horus Project directory
3. Open `firmware/mpu6050_imu.ino`

### 2. Verify the Code

Click the **Verify** button (checkmark icon) to compile the code and check for errors.

### 3. Upload to ESP32

1. Ensure ESP32 is connected via USB
2. Click the **Upload** button (right arrow icon)
3. Wait for the upload to complete
4. You should see "Hard resetting via RTS pin..." when successful

### Upload Troubleshooting

If upload fails:

1. **Hold the BOOT button** on the ESP32 during upload
2. Check the selected port is correct
3. Try a different USB cable (data cable, not charge-only)
4. Reduce upload speed to 115200
5. Restart Arduino IDE and try again

## Testing the Setup

### 1. Open Serial Monitor

1. Go to **Tools → Serial Monitor**
2. Set baud rate to **115200**
3. You should see sensor data being printed:
   ```
   Pitch: 0.45, Roll: -1.23
   Pitch: 0.48, Roll: -1.19
   ...
   ```

### 2. Test Sensor Response

- **Tilt the MPU6050** forward/backward → Pitch should change
- **Tilt the MPU6050** left/right → Roll should change
- Verify the values are reasonable (-90° to +90° for pitch, -180° to +180° for roll)

### 3. Connect to Horus Project

1. Close the Serial Monitor (important!)
2. Launch the Horus Project application
3. The PFD should now display real-time attitude data

## I2C Address Configuration

The MPU6050 can have two I2C addresses:

- **0x68** (default, AD0 pin low)
- **0x69** (if AD0 pin connected to 3.3V)

The firmware automatically detects the correct address. If you have multiple I2C devices, you may need to configure addresses manually.

## Troubleshooting

### Problem: "Could not find MPU6050"

**Solutions:**
- Check wiring connections
- Verify power supply (3.3V, not 5V)
- Try the other I2C address (0x69)
- Test with an I2C scanner sketch
- Replace the MPU6050 module (could be faulty)

### Problem: "Serial port not found"

**Solutions:**
- Install USB-to-Serial drivers (CP210x or CH340)
- Check USB cable (must be data cable)
- Try different USB port
- Check device manager (Windows) or dmesg (Linux)

### Problem: "Upload failed"

**Solutions:**
- Hold BOOT button during upload
- Reduce upload speed
- Check board selection is correct
- Restart Arduino IDE

### Problem: "Erratic sensor readings"

**Solutions:**
- Calibrate the sensor (see [MPU6050_CALIBRATION.md](MPU6050_CALIBRATION.md))
- Reduce wire length
- Add decoupling capacitors (10µF + 0.1µF on VCC)
- Check for loose connections
- Keep away from magnetic/electrical interference

### Problem: "Data not appearing in Horus Project"

**Solutions:**
- Ensure Serial Monitor is closed (only one program can use serial port)
- Verify baud rate is 115200 in both firmware and Horus Project
- Check the correct COM port is selected in Horus Project
- Restart both ESP32 and Horus Project application

## Advanced Configuration

### Changing the Update Rate

Edit the firmware to change the data rate (default: 50Hz):

```cpp
delay(20);  // 50Hz update rate (1000ms / 50 = 20ms)
```

### Adjusting Complementary Filter

Edit the filter coefficients for more/less smoothness:

```cpp
pitch = 0.98 * pitchGyro + 0.02 * pitchAccel;  // 98% gyro, 2% accel
roll = 0.98 * rollGyro + 0.02 * rollAccel;
```

More gyro weight = faster response, more drift
More accel weight = slower response, less drift

### Using Different GPIO Pins

If GPIO 21/22 are occupied, you can use other I2C-capable pins:

```cpp
Wire.begin(SDA_PIN, SCL_PIN);  // Specify custom pins
```

Valid I2C pins on ESP32: 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27

## Additional Resources

- [ESP32 Pinout Reference](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)
- [MPU6050 Datasheet](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf)
- [Arduino ESP32 Documentation](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
- [Horus Project README](../README.md)
- [Serial Protocol Specification](SERIAL_PROTOCOL.md)

## Next Steps

- [Calibrate your MPU6050](MPU6050_CALIBRATION.md) for accurate readings
- [Understand the Serial Protocol](SERIAL_PROTOCOL.md) for custom integrations
- Explore adding additional sensors (GPS, barometer, magnetometer)

---

**Author**: João Oliveira ([@joaoliveira6704](https://github.com/joaoliveira6704))
**Last Updated**: 2025-01-17
