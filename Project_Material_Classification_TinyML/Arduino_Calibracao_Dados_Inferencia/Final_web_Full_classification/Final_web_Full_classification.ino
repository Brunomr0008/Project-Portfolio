#include "WebServerESP.h"
#include "RGBSensor.h"
#include "MMA845X_Accelerometer.h"
#include "Metal_Sensor.h"

#include "model_data.h"
#include <tflm_esp32.h>
#include <eloquent_tinyml.h>

#define ARENA_SIZE 2000 // this is trial-and-error process

Eloquent::TF::Sequential<TF_NUM_OPS, ARENA_SIZE> tf;

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("TENSORFLOW IRIS");
  
  // configure input/output
  tf.setNumInputs(TF_NUM_INPUTS);
  tf.setNumOutputs(TF_NUM_OUTPUTS);

  // add required ops
  tf.resolver.AddFullyConnected();
  tf.resolver.AddSoftmax();

  while (!tf.begin(g_model).isOk()) {
    Serial.println(tf.exception.toString());
    delay(1000);
  }

  Serial.println("Modelo carregado com sucesso!");

  // Sensores
  //setupMMA8452();
  //setupRGB();
  //setupMetalSensor();

  // Web Server
  //startWebServer();
}

void loop() {
  //ReadRGB();
  //ReadMMA8452();
  //ReadMetalSensor();


  // classify class 0
  if (!tf.predict(x0).isOk()) {
    Serial.println(tf.exception.toString());
    return;
  }
  Serial.print("expected class 0, predicted class ");
  Serial.print(tf.classification);
  Serial.print(" ");
  // how long does it take to run a single prediction?
  Serial.print(tf.benchmark.microseconds());
  Serial.println("us");
  
  // classify class 1
  if (!tf.predict(x1).isOk()) {
    Serial.println(tf.exception.toString());
    return;
  }
  Serial.print("expected class 1, predicted class ");
  Serial.print(tf.classification);
  Serial.print(" ");
  // how long does it take to run a single prediction?
  Serial.print(tf.benchmark.microseconds());
  Serial.println("us");

// classify class 2
  if (!tf.predict(x2).isOk()) {
    Serial.println(tf.exception.toString());
    return;
  }
  Serial.print("expected class 2, predicted class ");
  Serial.print(tf.classification);
  Serial.print(" ");
  // how long does it take to run a single prediction?
  Serial.print(tf.benchmark.microseconds());
  Serial.println("us");

  delay(5000);
}
