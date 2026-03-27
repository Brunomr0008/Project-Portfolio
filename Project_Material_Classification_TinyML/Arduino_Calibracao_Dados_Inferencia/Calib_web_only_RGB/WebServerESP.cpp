#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "WebServerESP.h"
#include "RGBSensor.h"

/*
// ===== WIFI =====
const char* ssid = "MEO-485D60";
const char* password = "584b3b96ad";
*/

// ===== WIFI =====
const char* ssid = "hmmmm";
const char* password = "123456789";

void RootGeneral();
void webpage();

AsyncWebServer server(80);

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

  server.on("/calibstatusRGB", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", messageCalibRGB);
  });

  server.on("/minmaxRGB", HTTP_GET, [](AsyncWebServerRequest *request){
    String txt = String(R_MinHz) + "," + String(R_MaxHz) + "," +
                 String(G_MinHz) + "," + String(G_MaxHz) + "," +
                 String(B_MinHz) + "," + String(B_MaxHz);      
    request->send(200, "text/plain", txt);
  });

  server.on("/calibrate_RGB", HTTP_GET, [](AsyncWebServerRequest *request){
    // CLIQUE 1 → pedir branco
    if (ActiveSensorRGB && StateCalibrationRGB == 0) {
      StateCalibrationRGB = 1;
      CalibratingRGB = true;
      messageCalibRGB = "Place the WHITE OBJECT and click again";
      return;
    }
    // CLIQUE 2 → começa a calibrar branco (por tempo)
    if (ActiveSensorRGB && StateCalibrationRGB == 1) {
      StateCalibrationRGB = 2;
      TimeStateRGB = millis();
      messageCalibRGB = "Calibrating WHITE...";
      return;
    }
    // CLIQUE 3 → começa a calibrar preto (por tempo)
    if (ActiveSensorRGB && StateCalibrationRGB == 3) {
      StateCalibrationRGB = 4;
      TimeStateRGB = millis();
      messageCalibRGB = "Calibrating BLACK...";
      return;
    }
    // CLIQUE FINAL → reinicia tudo
    if (ActiveSensorRGB && StateCalibrationRGB == 5) {
      StateCalibrationRGB = 0;
      CalibratingRGB = false;
      messageCalibRGB = "Ready to recalibrate";
      return;
    }
    // CLIQUE Emergencia → reinicia tudo
    if (!ActiveSensorRGB) {
      StateCalibrationRGB = 0;
      messageCalibRGB = "!!Activate the sensor to calibrate!!";
    }
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
              "label:after{content:'';width:14px;height:14px;position:absolute;top:3px;left:3px;"
              "background:linear-gradient(180deg,#ccc,#888);border-radius:50%;transition:.3s;}" 
              "input:checked + label{background:#f5f5f5;}" 
              "input:checked + label:after{left:57px;transform:translateX(-100%);background:linear-gradient(180deg,#444,#000);}" 

            /* Container das caixas de cor e min/max */
            "#colorContainer{display:flex;gap:5px;margin-top:20px;}" 
              /* Caixa RGB */
              "#colorbox{border-radius:10px;width:100px;height:100px;border:2px solid black;}" 

              /* Caixa min/max */
              "#minmaxBox{width:90px;border:2px solid black;padding:5px;border-radius:10px;background:#f5f5f5;}" 
              "#minmaxBox p{font-size:13px;margin:2.5px 0;line-height:12px;}" 
              ".rval{color:red}" 
              ".gval{color:green}" 
              ".bval{color:blue}" 

            /* Botão calibrar */
            "button{margin-top:10px;padding:5px 5px;border:2px solid black;border-radius:15px;}" 

            /* Terminal / box de mensagens */
            "#calibRGB{margin-top:10px;border:2px solid black;padding:10px;border-radius:10px;"
            "font-size:16px;background:#f5f5f5;text-align:center;min-width:190px;}"
          "</style>"
        "</head>"

        "<body>"
          /* Título */
          "<h1>ESP32 Calibration</h1>"

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

          /* -- Container do quadrado de cor e min/max -- */
          "<div id='colorContainer'>"
          "  <div id='colorbox'></div>"
          "  <div id='minmaxBox'>"
          "    <p class='rval'>R Min: <span id='rmin'>999999</span></p>"
          "    <p class='rval'>R Max: <span id='rmax'>0</span></p>"
          "    <p class='gval'>G Min: <span id='gmin'>999999</span></p>"
          "    <p class='gval'>G Max: <span id='gmax'>0</span></p>"
          "    <p class='bval'>B Min: <span id='bmin'>999999</span></p>"
          "    <p class='bval'>B Max: <span id='bmax'>0</span></p>"
          "  </div>"
          "</div>"

          /* -- Botão calibrar -- */
          "<button onclick='calibrateRGB()'>Calibrate Sensor</button>"

          /* -- Terminal -- */
          "<p id='calibRGB'>---</p>"

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
            "}, 500);"

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
            "}, 300);"

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
            "}, 300);"

            /* ---- Calibração Botão ---- */
            "function calibrateRGB(){"
            " fetch('/calibrate_RGB')"
            "}"

            /* ---- Calibração menssagem ---- */
            "setInterval(function(){"
            " fetch('/calibstatusRGB')"
            "   .then(r => r.text())"
            "   .then(t => {"
            "       document.getElementById('calibRGB').innerHTML = t;"
                    /*Se a calibração terminou, atualiza os valores min/max*/
            "       if(t.includes('successful')) {"
            "         fetch('/minmaxRGB')"
            "          .then(r => r.text())"
            "          .then(txt => {"
            "            let v = txt.split(',');"
            "            document.getElementById('rmin').innerHTML = v[0];"
            "            document.getElementById('rmax').innerHTML = v[1];"
            "            document.getElementById('gmin').innerHTML = v[2];"
            "            document.getElementById('gmax').innerHTML = v[3];"
            "            document.getElementById('bmin').innerHTML = v[4];"
            "            document.getElementById('bmax').innerHTML = v[5];"
            "           });"
            "       }"
            "   });"
            "}, 500);"

          "</script>"
        "</body>"
      "</html>";

    request->send(200, "text/html", html);
  });
  
  server.begin();
}