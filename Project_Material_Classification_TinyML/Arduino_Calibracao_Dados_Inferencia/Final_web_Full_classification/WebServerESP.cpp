#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "WebServerESP.h"
#include "RGBSensor.h"
#include "MMA845X_Accelerometer.h"
#include "Metal_Sensor.h"


// ===== WIFI =====
const char* ssid = "MEO-485D60";
const char* password = "584b3b96ad";

/*
// ===== WIFI =====
const char* ssid = "hmmmm";
const char* password = "123456789";
*/

void RootGeneral();
void webpage();

AsyncWebServer server(80);

void stopWebServer() {
  server.end();                 // encerra o servidor
  WiFi.disconnect(true);        // desconecta do WiFi
  WiFi.mode(WIFI_OFF);          // desliga WiFi
}

void startWebServer() {
  WiFi.mode(WIFI_STA);          // modo estação
  WiFi.begin(ssid, password);   // conecta ao WiFi

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 5000) {
    Serial.print(".");
    delay(300);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected!");
    Serial.print("Access via phone or PC: http://");
    Serial.println(WiFi.localIP());
    
    RootGeneral();
    webpage();
  } else {
    Serial.println("Failed to connect to WiFi (timeout 5s)");
  }
}

void RootGeneral(){
  // ===== ROTA PARA ENVIAR RGB =====
  server.on("/rgb", HTTP_GET, [](AsyncWebServerRequest *request){
    String txt = String(redValue) + "," + String(greenValue) + "," + String(blueValue);
    request->send(200, "text/plain", txt);
  });
 
  // ===== ROTA PARA ENVIAR HSV =====
  server.on("/hsv", HTTP_GET, [](AsyncWebServerRequest *request){
    String txt = String(Hue) + "," + String(Saturation) + "," + String(Value);
    request->send(200, "text/plain", txt);
  });

  // ===== ROTA PARA ATIVAR/DESATIVAR A LEITURA DE DADOS DO SENSOR DE COR =====
  server.on("/toggleRGB", HTTP_GET, [](AsyncWebServerRequest *request){
    ActiveSensorRGB = !ActiveSensorRGB; // inverte o estado
  });

  // Rota para saber se o sensor está ativo
  server.on("/sensorstateRGB", HTTP_GET, [](AsyncWebServerRequest *request){
    if(ActiveSensorRGB)
      request->send(200, "text/plain", "1"); // ligado
    else
      request->send(200, "text/plain", "0"); // desligado
  });


  // ===== ROTA PARA ENVIAR vib =====
  server.on("/vibration", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(vib, 2));
  });

  // ===== ROTA PARA ATIVAR/DESATIVAR A LEITURA DE DADOS DO SENSOR DE COR =====
  server.on("/toggleMMA845X", HTTP_GET, [](AsyncWebServerRequest *request){
    ActiveSensorMMA845X = !ActiveSensorMMA845X;
  });

  // Rota para saber se o sensor está ativo
  server.on("/sensorstateMMA845X", HTTP_GET, [](AsyncWebServerRequest *request){
    if(ActiveSensorMMA845X)
      request->send(200, "text/plain", "1"); // ligado
    else
      request->send(200, "text/plain", "0"); // desligado
  });


  // ===== ROTA PARA ENVIAR Metal =====
  server.on("/metal", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(MetalsensorValue));
  });

  // ===== ROTA PARA ATIVAR/DESATIVAR A LEITURA DE DADOS DO SENSOR DE COR =====
  server.on("/toggleMetal", HTTP_GET, [](AsyncWebServerRequest *request){
    ActiveMetalSensor = !ActiveMetalSensor;
  });

  // Rota para saber se o sensor está ativo
  server.on("/sensorstateMetal", HTTP_GET, [](AsyncWebServerRequest *request){
    if(ActiveMetalSensor)
      request->send(200, "text/plain", "1"); // ligado
    else
      request->send(200, "text/plain", "0"); // desligado
  });
}

void webpage(){
  // ===== PÁGINA WEB SIMPLES =====
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html =
      "<html>"
        "<head>"
          "<meta name='viewport' content='width=device-width, initial-scale=1'>"
          "<style>"
            "body{margin:0;width:100vw;height:100vh;font-family:Arial;display:flex;"
            "flex-direction:column;align-items:center;}" 

            /* Título */
            "h1{margin:0;text-align:center;margin-top:20px;}" 

            /* Subtítulo */
            "h2{margin:0;text-align:center;margin-top: 10px;}" 

            /* -- Container RGB | SWITCH | HSV -- */
            "#rgbhsvContainer{display:grid;grid-template-rows:auto auto auto;justify-items:center;margin-top:20px;}" 
              /* Linhas RGB e HSV: elementos lado a lado */
              ".rowRGB p,.rowHSV p{display:inline-block;font-size:14px;font-weight:bold;text-align:center;}"
              ".rowRGB p{margin: 5px 10px;}"
              ".rowHSV p{margin: 5px 5px;}"
              
              /* Linha do switch */
              ".rowSwitch{margin-top:10px;display:flex;justify-content:center;align-items:center;}"
              /* Switch botão */
              "input[type='checkbox']{width:0;height:0;visibility:hidden;}" 
              "label{width:60px;height:20px;position:relative;background:#242424;border-radius:10px;"
              "cursor:pointer;transition:.3s;box-shadow:inset 0 2px 10px rgba(0,0,0,.4);margin-top:0px;}" 
              "label:after{content:'';width:14px;height:14px;position:absolute;top:3px;left:43px;"
              "background:linear-gradient(180deg,#ccc,#888);border-radius:50%;transition:.3s;}" 
              "input:checked + label{background:#f5f5f5;}" 
              "input:checked + label:after{left:17px;transform:translateX(-100%);background:linear-gradient(180deg,#444,#000);}" 

            /* Caixa RGB */
            "#colorbox{border-radius:10px;width:100px;height:100px;border:2px solid black;margin-top:15px;}" 

            /* Bloco único do switch + valor */
            "#vibBlock{margin-top:20px;display:flex;align-items:center;gap:0px;}" 
            "#vibBlock p{margin:0px 0px 0px 50px;padding:0px;font-size:14px;font-weight:bold;}" 

            /* Bloco único do switch + valor */
            "#MetalBlock{margin-top:20px;display:flex;align-items:center;gap:0px;}" 
            "#MetalBlock p{margin:0px 0px 0px 50px;padding:0px;font-size:14px;font-weight:bold;}" 
          "</style>"
        "</head>"

        "<body>"
          /* Título */
          "<h1>ESP32 Data Fusion</h1>"

          /* Subtítulo do sensor */
          "<h2>Color Sensor (RGB)</h2>"

          /* -- Valores R G B -- */
          "<div id='rgbhsvContainer'>"
          "  <div class='rowRGB'>"
          "    <p>R: <span id='r'>0</span> </p>" 
          "    <p>G: <span id='g'>0</span> </p>"
          "    <p>B: <span id='b'>0</span> </p>"
          "  </div>"

          /* -- Valores H S V -- */
          "  <div class='rowHSV'>"
          "    <p>H: <span id='h'>0</span> </p>"
          "    <p>S: <span id='s'>0</span> </p>"
          "    <p>V: <span id='v'>0</span> </p>"
          "  </div>"

          /* -- Switch (botão Alavanca) -- */
          "  <div class='rowSwitch'>"
          "    <input type='checkbox' id='sw1' onclick='toggleSensorRGB()'>"
          "    <label for='sw1'></label>"
          "  </div>"
          "</div>"

          "<div id='colorbox'></div>"


          /* Subtítulo do acelerómetro */
          "<h2>MMA8452 Accelerometer</h2>"

          /* Bloco Vib + Switch */
          "<div id='vibBlock'>"
          "  <input type='checkbox' id='sw2' onclick='toggleSensorMMA845X()'>"
          "  <label for='sw2'></label>"
          "  <p>Vib: <span id='vib'>0</span></p>"
          "</div>"

           /* Subtítulo do acelerómetro */
          "<h2>Metal Sensor</h2>"

          /* Bloco Metal + Switch */
          "<div id='MetalBlock'>"
          "  <input type='checkbox' id='sw3' onclick='toggleSensorMetal()'>"
          "  <label for='sw3'></label>"
          "  <p>Metal: <span id='metal_'>0</span></p>"
          "</div>"

          "<script>"
            /* ---- BOTÃO LIGA/DESLIGA LEITURA RGB ---- */
            "function toggleSensorRGB(){"
            " fetch('/toggleRGB');"
            "}"

            // Atualiza o estado inicial e evita inversão errada
            "setInterval(function(){"
            "    fetch('/sensorstateRGB')"
            "      .then(r => r.text())"
            "      .then(t => {"
            "          document.getElementById('sw1').checked = (t === '1');"
            "      });"
            "}, 1000);"

            /* ---- LEITURA RGB ---- */
            "setInterval(function(){"
            " fetch('/rgb')"
            "  .then(r => r.text())"
            "  .then(t => {"
            "    let v = t.split(',');"
            "    document.getElementById('r').innerHTML = v[0];"
            "    document.getElementById('g').innerHTML = v[1];"
            "    document.getElementById('b').innerHTML = v[2];"
            "    document.getElementById('colorbox').style.backgroundColor = "
            "    'rgb('+v[0]+','+v[1]+','+v[2]+')';"
            "  });"
            "}, 500);"

            /* ---- LEITURA HSV ---- */
            "setInterval(function(){"
            " fetch('/hsv')"
            "  .then(r => r.text())"
            "  .then(t => {"
            "    let v = t.split(',');"
            "    document.getElementById('h').innerHTML = v[0];"
            "    document.getElementById('s').innerHTML = v[1];"
            "    document.getElementById('v').innerHTML = v[2];"
            "  });"
            "}, 500);"

            /* ---- BOTÃO LIGA/DESLIGA LEITURA ADXL335 ---- */
            "function toggleSensorMMA845X(){"
            " fetch('/toggleMMA845X');"
            "}"

            // Atualiza o estado inicial e evita inversão errada
            "setInterval(function(){"
            "    fetch('/sensorstateMMA845X')"
            "      .then(r => r.text())"
            "      .then(t => {"
            "          document.getElementById('sw2').checked = (t === '1');"
            "      });"
            "}, 1000);"

            /* ---- LEITURA vib ---- */
            "setInterval(function(){"
            " fetch('/vibration')"
            "  .then(r => r.text())"
            "  .then(t => {"
            "    document.getElementById('vib').innerHTML = t;"
            "  });"
            "}, 500);"


             /* ---- BOTÃO LIGA/DESLIGA LEITURA Metal sensor ---- */
            "function toggleSensorMetal(){"
            " fetch('/toggleMetal');"
            "}"

            // Atualiza o estado inicial e evita inversão errada
            "setInterval(function(){"
            "    fetch('/sensorstateMetal')"
            "      .then(r => r.text())"
            "      .then(t => {"
            "          document.getElementById('sw3').checked = (t === '1');"
            "      });"
            "}, 1000);"

            /* ---- LEITURA vib ---- */
            "setInterval(function(){"
            " fetch('/metal')"
            "  .then(r => r.text())"
            "  .then(t => {"
            "    document.getElementById('metal_').innerHTML = t;"
            "  });"
            "}, 500);"
          "</script>"
        "</body>"
      "</html>";

    request->send(200, "text/html", html);
  });
  
  server.begin();
}