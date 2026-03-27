#ifndef RGB_SENSOR_H
#define RGB_SENSOR_H

// ---- Calibração ----
extern unsigned long R_MinHz, R_MaxHz;
extern unsigned long G_MinHz, G_MaxHz;
extern unsigned long B_MinHz, B_MaxHz;

// ---- Valores RGB finais ----
extern int redValue, greenValue, blueValue;

// ---- Valores HSV finais ----
extern float Hue, Saturation, Value;

// ---- Flags ----
extern bool ActiveSensorRGB;
extern bool CalibratingRGB;
extern int StateCalibrationRGB;
extern unsigned long TimeStateRGB;
extern String messageCalibRGB;

// ---- Funções ----
void setupRGB();
void ReadRGB();
void ParcialStateCalibrationRGB();

#endif