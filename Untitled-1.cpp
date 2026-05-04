#include <Wire.h>

// MPU6050 I2C address
const int MPU_ADDR = 0x68;

// Raw data variables
int16_t AccX, AccY, AccZ;
int16_t GyroX, GyroY, GyroZ;

// Step detection variables
int stepCount = 0;
float prevAccZ = 0;
float threshold = 1.2;   // Adjust for sensitivity

void setup() {
  Serial.begin(115200);

  Wire.begin();

  // Wake up MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  Serial.println("MPU6050 Initialized...");
}

// -------- READ MPU6050 --------
void readMPU() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);  // Start register
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);

  AccX = Wire.read() << 8 | Wire.read();
  AccY = Wire.read() << 8 | Wire.read();
  AccZ = Wire.read() << 8 | Wire.read();

  GyroX = Wire.read() << 8 | Wire.read();
  GyroY = Wire.read() << 8 | Wire.read();
  GyroZ = Wire.read() << 8 | Wire.read();
}

// -------- STEP DETECTION --------
void detectStep() {
  float accZ_g = AccZ / 16384.0;  // Convert to g

  if (accZ_g > threshold && prevAccZ <= threshold) {
    stepCount++;
    Serial.print("Step Detected! Count: ");
    Serial.println(stepCount);
  }

  prevAccZ = accZ_g;
}

// -------- DIRECTION DETECTION --------
void detectDirection() {
  float accX_g = AccX / 16384.0;

  if (accX_g > 0.5) {
    Serial.println("Tilt Right");
  }
  else if (accX_g < -0.5) {
    Serial.println("Tilt Left");
  }
}

// -------- MAIN LOOP --------
void loop() {

  readMPU();

  detectStep();
  detectDirection();

  delay(200);
}