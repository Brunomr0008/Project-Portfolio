#include "WebServerESP.h"
#include "RGBSensor.h"
#include "MMA845X_Accelerometer.h"
#include "Metal_Sensor.h"

void SendPython();

void setup() {
  Serial.begin(115200);
  
  // Sensores
  setupMMA8452();
  setupRGB();
  setupMetalSensor();

  // Web Server
  startWebServer();
}

void loop() {
  ReadRGB();
  ReadMMA8452();
  ReadMetalSensor();

  SendPython();
}

void SendPython(){
  static bool dataCollectionModeHSV = false;
  static bool dataCollectionModeVib = false;
  static bool dataCollectionModeMetal = false;

  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim(); // remove \r\n e espaços

    if (cmd == "START_HSV") {
      dataCollectionModeHSV = true;
      stopWebServer(); // função que desliga WiFi e server
    }
    if (cmd == "START_VIB") {
      dataCollectionModeVib = true;
      stopWebServer(); // função que desliga WiFi e server
    }
    if (cmd == "START_METAL") {
      dataCollectionModeMetal = true;
      stopWebServer(); // função que desliga WiFi e server
    }
    if (cmd == "STOP") {
      dataCollectionModeHSV = false;
      dataCollectionModeVib = false;
      dataCollectionModeMetal = false;
      startWebServer(); // função que liga WiFi e server
    }
  }

  // Se estiver em modo coleta, envia dados Serial
  if (dataCollectionModeHSV) {
    Serial.print("?");
    Serial.print(Hue);
    Serial.print(",");
    Serial.print(Saturation);
    Serial.print(",");
    Serial.println(Value);
    delay(25); // pequeno delay para estabilizar
  }

  if (dataCollectionModeVib) {
    Serial.print("?");
    Serial.println(vib, 2);
    delay(25); 
  }

  if (dataCollectionModeMetal) {
    Serial.print("?");
    Serial.println(MetalsensorValue);
    delay(25); 
  }
}