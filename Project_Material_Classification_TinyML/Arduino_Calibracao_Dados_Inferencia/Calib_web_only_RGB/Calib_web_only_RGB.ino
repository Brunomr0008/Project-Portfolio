#include "WebServerESP.h"
#include "RGBSensor.h"

void setup() {
  Serial.begin(115200);

  startWebServer();
  setupRGB();
}

void loop() {
  ParcialStateCalibrationRGB();
  ReadRGB();
}