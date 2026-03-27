#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include "MMA845X_Accelerometer.h"

#define MMA_ADDR 0x1C

// -------- CONFIG --------
float alpha = 0.95;   // HP filter
float env_k = 0.2;    // envelope smoothing

// -------- ESTADO --------
bool ActiveSensorMMA845X = true;
float vib = 0;

// -------- I2C --------
void writeReg(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(MMA_ADDR);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

bool readRaw(int16_t &x, int16_t &y, int16_t &z) {
  Wire.beginTransmission(MMA_ADDR);
  Wire.write(0x01);
  Wire.endTransmission(false);
  Wire.requestFrom(MMA_ADDR, 6);

  if (Wire.available() == 6) {
    x = (Wire.read()<<8 | Wire.read()) >> 4;
    y = (Wire.read()<<8 | Wire.read()) >> 4;
    z = (Wire.read()<<8 | Wire.read()) >> 4;
    return true;
  }
  return false;
}

// -------- SETUP SENSOR --------
void setupMMA8452() {
  Wire.begin(21, 22);

  writeReg(0x2A, 0x00);          // standby
  writeReg(0x0E, 0x00);          // ±2g
  writeReg(0x0F, 0b00010000);    // HPF enable
  writeReg(0x2A, 0b00101001);    // Active | High-res | 200Hz
}

// -------- LOOP --------
void ReadMMA8452() {
  // filtros
  static float hp_x=0, hp_y=0, hp_z=0;
  static float prev_x=4077, prev_y=4045, prev_z=1032;
  static float envelope = 0;

  if (!ActiveSensorMMA845X) return;

  int16_t x,y,z;
  if (!readRaw(x,y,z)) return;

  // HP
  hp_x = alpha * (hp_x + x - prev_x);
  hp_y = alpha * (hp_y + y - prev_y);
  hp_z = alpha * (hp_z + z - prev_z);

  prev_x = x; prev_y = y; prev_z = z;

  float mag = sqrt(hp_x*hp_x + hp_y*hp_y + hp_z*hp_z);
  envelope += env_k * (fabs(mag) - envelope);

  vib = envelope;
  if (vib < 0) vib = 0;

  delay(25);
}