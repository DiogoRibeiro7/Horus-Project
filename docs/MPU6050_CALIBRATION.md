# MPU6050 Calibration Guide

Proper calibration of the MPU6050 IMU sensor is essential for accurate attitude measurements in the Horus Project. This guide explains why calibration is needed and how to perform it.

## Table of Contents

- [Why Calibrate?](#why-calibrate)
- [What is Calibration?](#what-is-calibration)
- [Types of Calibration](#types-of-calibration)
- [Quick Calibration](#quick-calibration)
- [Advanced Calibration](#advanced-calibration)
- [Verification](#verification)
- [Troubleshooting](#troubleshooting)

## Why Calibrate?

The MPU6050 sensor may have:

- **Offset errors**: Non-zero readings when stationary
- **Scale factor errors**: Incorrect sensitivity
- **Temperature drift**: Changes with temperature
- **Manufacturing variations**: Each sensor is slightly different

Calibration corrects these errors for accurate pitch and roll measurements.

## What is Calibration?

Calibration determines:

1. **Zero offsets**: What values should be zero when level
2. **Scale factors**: How raw values map to degrees
3. **Cross-axis effects**: How axes influence each other

## Types of Calibration

### 1. Factory Calibration (Built-in)

The MPU6050 has built-in factory calibration that's usually sufficient for most applications.

**Pros:**
- No user action required
- Already programmed into the sensor

**Cons:**
- May drift over time
- Not customized to your specific setup

### 2. Zero-Point Calibration (Recommended)

Calculates offset values when the sensor is perfectly level.

**Pros:**
- Simple and quick
- Significantly improves accuracy
- Good for most use cases

**Cons:**
- Requires level surface
- Doesn't correct scale factors

### 3. Multi-Point Calibration (Advanced)

Uses multiple known orientations to calculate offsets and scale factors.

**Pros:**
- Most accurate
- Corrects both offset and scale errors

**Cons:**
- Time-consuming
- Requires precision equipment

## Quick Calibration

This is the recommended calibration method for most users.

### Requirements

- Perfectly level surface (use a spirit level)
- Stable environment (no vibrations)
- 2-5 minutes of time

### Step 1: Setup

1. Place the MPU6050 on a level surface
2. Ensure the sensor is stable and won't move
3. Connect ESP32 to your computer
4. Upload the calibration firmware (see below)

### Step 2: Calibration Firmware

Create a new Arduino sketch:

```cpp
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Calibration variables
int16_t ax, ay, az, gx, gy, gz;
int32_t ax_offset = 0, ay_offset = 0, az_offset = 0;
int32_t gx_offset = 0, gy_offset = 0, gz_offset = 0;
const int samples = 1000;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialize MPU6050
  Serial.println("Initializing MPU6050...");
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }

  Serial.println("MPU6050 connected successfully");
  Serial.println("\n=== STARTING CALIBRATION ===");
  Serial.println("Place sensor on LEVEL surface and keep it STILL");
  delay(3000);

  // Collect samples
  Serial.println("Collecting samples...");
  for (int i = 0; i < samples; i++) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    ax_offset += ax;
    ay_offset += ay;
    az_offset += az;
    gx_offset += gx;
    gy_offset += gy;
    gz_offset += gz;

    if (i % 100 == 0) {
      Serial.print(".");
    }
    delay(5);
  }
  Serial.println("\nCalculating offsets...");

  // Calculate average offsets
  ax_offset /= samples;
  ay_offset /= samples;
  az_offset /= samples;
  gx_offset /= samples;
  gy_offset /= samples;
  gz_offset /= samples;

  // Adjust for gravity (Z-axis should read +1g = ~16384)
  az_offset -= 16384;

  // Display results
  Serial.println("\n=== CALIBRATION COMPLETE ===");
  Serial.println("\nAccelerometer Offsets:");
  Serial.print("AX: "); Serial.println(ax_offset);
  Serial.print("AY: "); Serial.println(ay_offset);
  Serial.print("AZ: "); Serial.println(az_offset);

  Serial.println("\nGyroscope Offsets:");
  Serial.print("GX: "); Serial.println(gx_offset);
  Serial.print("GY: "); Serial.println(gy_offset);
  Serial.print("GZ: "); Serial.println(gz_offset);

  Serial.println("\n=== COPY THESE VALUES ===");
  Serial.println("Add to your main sketch:");
  Serial.println("mpu.setXAccelOffset(" + String(ax_offset) + ");");
  Serial.println("mpu.setYAccelOffset(" + String(ay_offset) + ");");
  Serial.println("mpu.setZAccelOffset(" + String(az_offset) + ");");
  Serial.println("mpu.setXGyroOffset(" + String(gx_offset) + ");");
  Serial.println("mpu.setYGyroOffset(" + String(gy_offset) + ");");
  Serial.println("mpu.setZGyroOffset(" + String(gz_offset) + ");");
}

void loop() {
  // Nothing to do
}
```

### Step 3: Run Calibration

1. Upload the calibration sketch to ESP32
2. Open Serial Monitor (115200 baud)
3. Wait for calibration to complete (~10 seconds)
4. Copy the offset values displayed

Example output:
```
=== CALIBRATION COMPLETE ===

Accelerometer Offsets:
AX: -2453
AY: 183
AZ: 1342

Gyroscope Offsets:
GX: 34
GY: -12
GZ: 56

=== COPY THESE VALUES ===
Add to your main sketch:
mpu.setXAccelOffset(-2453);
mpu.setYAccelOffset(183);
mpu.setZAccelOffset(1342);
mpu.setXGyroOffset(34);
mpu.setYGyroOffset(-12);
mpu.setZGyroOffset(56);
```

### Step 4: Apply Calibration

Edit `firmware/mpu6050_imu.ino` and add the offset values after `mpu.initialize()`:

```cpp
void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();

  // Apply calibration offsets
  mpu.setXAccelOffset(-2453);  // Your values here
  mpu.setYAccelOffset(183);
  mpu.setZAccelOffset(1342);
  mpu.setXGyroOffset(34);
  mpu.setYGyroOffset(-12);
  mpu.setZGyroOffset(56);

  // Rest of setup...
}
```

### Step 5: Verify

Re-upload the main firmware and verify:
- Pitch reads ~0° when level
- Roll reads ~0° when level
- Values are stable (±0.5° or less)

## Advanced Calibration

For maximum accuracy, use a 6-point calibration.

### Equipment Needed

- Precision level or spirit level
- Flat, stable surface
- Ruler or square for 90° angles
- Calibration cube (optional but helpful)

### 6-Point Method

Collect data with the sensor in 6 orientations:

1. **+X up** (sensor flat, X-axis pointing up)
2. **-X up** (sensor flat, X-axis pointing down)
3. **+Y up** (sensor on edge, Y-axis pointing up)
4. **-Y up** (sensor on edge, Y-axis pointing down)
5. **+Z up** (sensor normal, Z-axis pointing up)
6. **-Z up** (sensor upside down, Z-axis pointing down)

For each orientation:
1. Hold steady for 10 seconds
2. Record average accelerometer values
3. Calculate scale factors and offsets

### Calculation

This requires solving a system of equations. Consider using existing calibration tools:

- **MPU6050_calibration** Arduino library
- **RTIMULib** calibration utility
- **I2Cdevlib** calibration examples

## Verification

### Visual Check

In Horus Project:
- Pitch ladder should be centered when level
- Roll indicator should be at 0° when level
- Values should be stable (not drifting)

### Numerical Check

In Serial Monitor:
```
Pitch: 0.12, Roll: -0.08    ✓ Good
Pitch: 15.34, Roll: -3.21   ✗ Needs calibration
```

### Tilt Test

1. Tilt sensor +10° → Verify pitch reads +10° (±1°)
2. Tilt sensor -10° → Verify pitch reads -10° (±1°)
3. Roll sensor +10° → Verify roll reads +10° (±1°)
4. Roll sensor -10° → Verify roll reads -10° (±1°)

## Temperature Compensation

The MPU6050 has built-in temperature compensation, but for extreme environments:

1. Run calibration at operating temperature
2. Re-calibrate if temperature changes significantly
3. Consider using the MPU9250 (better temperature stability)

## Calibration Best Practices

### Do's

✅ Calibrate on a stable, level surface
✅ Wait for sensor to warm up (1-2 minutes)
✅ Perform calibration at operating temperature
✅ Re-calibrate if sensor is moved or remounted
✅ Store calibration values in firmware

### Don'ts

❌ Don't calibrate on an unstable surface
❌ Don't touch the sensor during calibration
❌ Don't calibrate in vibrating environment
❌ Don't skip verification step
❌ Don't use calibration from a different sensor

## When to Re-Calibrate

Recalibration is needed when:

- Sensor is remounted or moved
- Significant temperature change occurs
- Readings appear to drift over time
- Physical shock or drop occurs
- Switching between different sensors

## Troubleshooting

### Problem: Offsets are very large (>10,000)

**Solutions:**
- Check sensor is actually level
- Verify sensor is not damaged
- Try a different MPU6050 module

### Problem: Values still drift after calibration

**Solutions:**
- Gyroscope drift is normal (compensated by accelerometer)
- Check complementary filter settings
- Consider implementing a Kalman filter
- Verify no magnetic interference nearby

### Problem: Can't get level surface

**Solutions:**
- Use a smartphone with level app
- Use water in a clear container
- Use a marble or ball bearing (should stay centered)

### Problem: Calibration values change each time

**Solutions:**
- Increase sample count (1000 → 5000)
- Wait longer for sensor to stabilize
- Check for environmental vibrations
- Ensure consistent temperature

## Alternative: Auto-Calibration

For applications where manual calibration is impractical:

```cpp
// Auto-calibrate on startup (sensor must be level)
void autoCalibrate() {
  Serial.println("Auto-calibrating... keep sensor level and still");

  mpu.CalibrateAccel(6);  // 6 iterations
  mpu.CalibrateGyro(6);   // 6 iterations

  Serial.println("Calibration complete!");
  mpu.PrintActiveOffsets();
}
```

**Note:** This requires the sensor to be level on startup.

## Calibration Storage

### Save to EEPROM (persistent)

```cpp
#include <EEPROM.h>

// Save calibration
void saveCalibration() {
  EEPROM.begin(512);
  EEPROM.put(0, ax_offset);
  EEPROM.put(4, ay_offset);
  // ... save all offsets
  EEPROM.commit();
}

// Load calibration
void loadCalibration() {
  EEPROM.begin(512);
  EEPROM.get(0, ax_offset);
  EEPROM.get(4, ay_offset);
  // ... load all offsets
}
```

## Additional Resources

- [MPU6050 Register Map](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf)
- [I2Cdevlib MPU6050 Library](https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050)
- [Sensor Fusion Algorithms](https://github.com/xioTechnologies/Fusion)
- [Complementary Filter Explanation](https://www.pieter-jan.com/node/11)

## Next Steps

- [Return to ESP32 Setup Guide](ESP32_SETUP.md)
- [Learn about the Serial Protocol](SERIAL_PROTOCOL.md)
- [Explore Sensor Expansion options](../README.md#roadmap)

---

**Author**: João Oliveira ([@joaoliveira6704](https://github.com/joaoliveira6704))
**Last Updated**: 2025-01-17
