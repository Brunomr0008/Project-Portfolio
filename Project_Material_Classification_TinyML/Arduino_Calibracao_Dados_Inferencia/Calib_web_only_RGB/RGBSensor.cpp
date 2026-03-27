#include <Arduino.h>
#include "RGBSensor.h"

// ---- PINOS ----
#define S2 0
#define S3 4
#define sensorOut 16

// ---- Funções ----
void calibrateSample();
unsigned long getAverageFrequency(uint8_t s2state, uint8_t s3state);
void RGBtoHSV();

// ---- VARIÁVEIS ----
unsigned long R_MinHz = 9185, R_MaxHz = 37571;
unsigned long G_MinHz = 8430, G_MaxHz = 28887;
unsigned long B_MinHz = 9652, B_MaxHz = 36541;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

bool ActiveSensorRGB = false;
bool CalibratingRGB = false;
int StateCalibrationRGB = 0;
unsigned long TimeStateRGB = 0;
String messageCalibRGB = "Ready to calibrate";

float Hue, Saturation, Value;

// ---- SETUP DO SENSOR ----
void setupRGB() {
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);
}

// ---- LEITURA RGB NORMAL ----
void ReadRGB() {
    if (!ActiveSensorRGB || CalibratingRGB) return;

    unsigned long rHz = getAverageFrequency(LOW, LOW);
    unsigned long gHz = getAverageFrequency(HIGH, HIGH);
    unsigned long bHz = getAverageFrequency(LOW, HIGH);

    unsigned long rMin = min(R_MinHz, R_MaxHz);
    unsigned long rMax = max(R_MinHz, R_MaxHz);
    unsigned long gMin = min(G_MinHz, G_MaxHz);
    unsigned long gMax = max(G_MinHz, G_MaxHz);
    unsigned long bMin = min(B_MinHz, B_MaxHz);
    unsigned long bMax = max(B_MinHz, B_MaxHz);

    if (rMin == rMax) { 
        rMin = 1; 
        rMax = rHz + 1; 
    }
    if (gMin == gMax) { 
        gMin = 1; 
        gMax = gHz + 1; 
    }
    if (bMin == bMax) { 
        bMin = 1; 
        bMax = bHz + 1; 
    }

    redValue = constrain(map(rHz, rMin, rMax, 0, 255), 0, 255);
    greenValue = constrain(map(gHz, gMin, gMax, 0, 255), 0, 255);
    blueValue = constrain(map(bHz, bMin, bMax, 0, 255), 0, 255);

    RGBtoHSV();
}

// ---- ESTADOS DE CALIBRAÇÃO ----
void ParcialStateCalibrationRGB() {
    static bool onetime = true;

    if (!CalibratingRGB) return;

    if (onetime) {
        R_MinHz = G_MinHz = B_MinHz = 999999;
        R_MaxHz = G_MaxHz = B_MaxHz = 0;
        onetime = false;
    }
    // Branco
    if (StateCalibrationRGB == 2) {
        if (millis() - TimeStateRGB < 2000) {
            calibrateSample();
        }
        else {
            StateCalibrationRGB = 3;
            messageCalibRGB = "Now place the BLACK OBJECT and click again";
        }
    }
    // Preto
    if (StateCalibrationRGB == 4) {
        if (millis() - TimeStateRGB < 2000) {
            calibrateSample();
        }
        else {
            onetime = true;
            StateCalibrationRGB = 5;
            CalibratingRGB = false;
            messageCalibRGB = "Calibration successful";
        }
    }
}

// ---- Calibração ----
void calibrateSample() {
    unsigned long r = getAverageFrequency(LOW, LOW);
    unsigned long g = getAverageFrequency(HIGH, HIGH);
    unsigned long b = getAverageFrequency(LOW, HIGH);

    if (r > 0) { 
        R_MinHz = min(R_MinHz, r); 
        R_MaxHz = max(R_MaxHz, r); 
    }
    if (g > 0) { 
        G_MinHz = min(G_MinHz, g); 
        G_MaxHz = max(G_MaxHz, g); 
    }
    if (b > 0) { 
        B_MinHz = min(B_MinHz, b); 
        B_MaxHz = max(B_MaxHz, b); 
    }
}

// ---- Média das frequências ----
unsigned long getAverageFrequency(uint8_t s2state, uint8_t s3state) { 
    const int samples = 8; 
    digitalWrite(S2, s2state); 
    digitalWrite(S3, s3state); 
    delay(10); 
    
    double sumHz = 0; 
    int valid = 0; 
    
    for (int i = 0; i < samples; i++) { 
        unsigned long period = pulseIn(sensorOut, LOW, 20000); 
        if (period > 0) { 
            sumHz += 1000000.0 / period; 
            valid++; 
        } 
    } 
    
    if (valid == 0) return 0; 
    return (unsigned long)(sumHz / valid + 0.5); 
}

// ---- RGB → HSV ----
void RGBtoHSV() {
    float R = redValue / 255.0f;
    float G = greenValue / 255.0f;
    float B = blueValue / 255.0f;

    float cmax = max(R, max(G, B));
    float cmin = min(R, min(G, B));
    float diff = cmax - cmin;

    if (diff < 1e-6){
        Hue = 0;
    } else if (cmax == R) {
        Hue = 60 * fmod((G - B) / diff, 6);
    } else if (cmax == G) {
        Hue = 60 * ((B - R) / diff + 2);
    } else {
        Hue = 60 * ((R - G) / diff + 4);
    }

    if (Hue < 0) { 
        Hue += 360;
    }

    Saturation = (cmax < 1e-6) ? 0 : (diff / cmax) * 100;
    Value = cmax * 100;
}