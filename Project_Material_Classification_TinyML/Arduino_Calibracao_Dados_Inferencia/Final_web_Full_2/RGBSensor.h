#ifndef RGB_SENSOR_H
#define RGB_SENSOR_H

// ---- Valores RGB finais ----
extern int redValue, greenValue, blueValue;

// ---- Valores HSV finais ----
extern float Hue, Saturation, Value;

// ---- Flags ----
extern bool ActiveSensorRGB;

// ---- Funções ----
void setupRGB();
void ReadRGB();

#endif