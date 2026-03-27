#include <Arduino.h>
#include "Metal_Sensor.h"

#define METAL_SENSOR_PIN 34

int MetalsensorValue = 0;
bool ActiveMetalSensor = true;


void setupMetalSensor() {
  pinMode(METAL_SENSOR_PIN, INPUT);
}

void ReadMetalSensor() {
  if (!ActiveMetalSensor) return;

  int invertedValue = analogRead(METAL_SENSOR_PIN);
  MetalsensorValue = abs(4095 - invertedValue);

  delay(25);
}