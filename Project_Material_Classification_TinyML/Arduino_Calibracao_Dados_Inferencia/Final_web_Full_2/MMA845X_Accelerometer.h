#ifndef MMA8452_ACCELEROMETER_H
#define MMA8452_ACCELEROMETER_H

// ---- Funções ----
void ReadMMA8452();
void setupMMA8452();

extern bool ActiveSensorMMA845X;
extern float vib;

#endif