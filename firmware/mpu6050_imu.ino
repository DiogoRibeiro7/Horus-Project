#include <Wire.h>

// MPU6050 I2C address
int MPU6050_ADDR = 0x68;

// MPU6050 Registers
const int PWR_MGMT_1 = 0x6B;
const int ACCEL_XOUT_H = 0x3B;

// Variables to store sensor data
int16_t accelX, accelY, accelZ;
int16_t gyroX, gyroY, gyroZ;
int16_t temperature;

// Complementary filter variables
float pitch = 0.0;
float roll = 0.0;
unsigned long lastTime = 0;

// I2C pins
#define SDA_PIN 21
#define SCL_PIN 22

void setup() {
  Serial.begin(115200);  // Match your Qt app's baud rate

  // Initialize I2C
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);

  delay(1000);

  // Scan for I2C devices
  if (!findMPU6050()) {
    Serial.println("ERROR: MPU6050 not found!");
    while (1) delay(1000);
  }

  // Wake up MPU6050
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);
  Wire.endTransmission(true);

  delay(100);

  // Configure accelerometer (±2g)
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x1C);
  Wire.write(0x00);
  Wire.endTransmission(true);

  // Configure gyroscope (±250°/s)
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x1B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  lastTime = millis();

  // Calibration pause
  delay(1000);
}

void loop() {
  // Read sensor data
  readMPU6050();

  // Calculate time delta
  unsigned long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;

  // Convert to g and degrees per second
  float accelX_g = accelX / 16384.0;
  float accelY_g = accelY / 16384.0;
  float accelZ_g = accelZ / 16384.0;
  float gyroX_dps = gyroX / 131.0;
  float gyroY_dps = gyroY / 131.0;

  // Calculate angles from accelerometer
  float accelPitch = atan2(accelY_g, sqrt(accelX_g * accelX_g + accelZ_g * accelZ_g)) * 180.0 / PI;
  float accelRoll = atan2(-accelX_g, accelZ_g) * 180.0 / PI;

  // Complementary filter (98% gyro, 2% accel)
  pitch = 0.98 * (pitch + gyroX_dps * dt) + 0.02 * accelPitch;
  roll = 0.98 * (roll + gyroY_dps * dt) + 0.02 * accelRoll;

  Serial.print(pitch, 2);
  Serial.print(",");
  Serial.println(roll, 2);

  delay(20);  // 50Hz update rate
}

void readMPU6050() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 14, true);

  // Read accelerometer
  accelX = Wire.read() << 8 | Wire.read();
  accelY = Wire.read() << 8 | Wire.read();
  accelZ = Wire.read() << 8 | Wire.read();

  // Read temperature (skip)
  temperature = Wire.read() << 8 | Wire.read();

  // Read gyroscope
  gyroX = Wire.read() << 8 | Wire.read();
  gyroY = Wire.read() << 8 | Wire.read();
  gyroZ = Wire.read() << 8 | Wire.read();
}

bool findMPU6050() {
  Wire.beginTransmission(0x68);
  if (Wire.endTransmission() == 0) {
    MPU6050_ADDR = 0x68;
    return true;
  }

  Wire.beginTransmission(0x69);
  if (Wire.endTransmission() == 0) {
    MPU6050_ADDR = 0x69;
    return true;
  }

  return false;
}