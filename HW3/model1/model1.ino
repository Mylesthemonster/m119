#include <Arduino_APDS9960.h>

#include <TensorFlowLite.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include <tensorflow/lite/version.h>

#include "/Users/mylesthemonster/Documents/m119/HW3/model.h"

const int total_Sample = 50;  

int sample_Num = 0;
unsigned long lastUpdate = 0;

// global variables used for TensorFlow Lite (Micro)
tflite::MicroErrorReporter tflErrorReporter;

// pull in all the TFLM ops, you can remove this line and
// only pull in the TFLM ops you need, if would like to reduce
// the compiled size of the sketch.
tflite::AllOpsResolver tflOpsResolver;

const tflite::Model* tflModel = nullptr;
tflite::MicroInterpreter* tflInterpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;

// Create a static memory buffer for TFLM, the size may need to
// be adjusted based on the model you are using
constexpr int tensorArenaSize = 8 * 1024;
byte tensorArena[tensorArenaSize] __attribute__((aligned(16)));

// array to map gesture index to a name
const char* COLORS[] = {
  "red",
  "orange",
  "yellow",
  "green",
  "blue",
  "purple"
};

#define NUM_COLORS (sizeof(COLORS) / sizeof(COLORS[0]))

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial);

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor!");
  }

  // get the TFL representation of the model byte array
  tflModel = tflite::GetModel(model);
  if (tflModel->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Model schema mismatch!");
    while (1);
  }

  // Create an interpreter to run the model
  tflInterpreter = new tflite::MicroInterpreter(tflModel, tflOpsResolver, tensorArena, tensorArenaSize, &tflErrorReporter);

  // Allocate memory for the model's input and output tensors
  tflInterpreter->AllocateTensors();

  // Get pointers for the model's input and output tensors
  tflInputTensor = tflInterpreter->input(0);
  tflOutputTensor = tflInterpreter->output(0);
}

void loop() {
  int r = 0, g = 0, b = 0;

  while (sample_Num < total_Sample ){
    if (APDS.colorAvailable()) {
      APDS.readColor(r, g, b);

      // normalize the data between 0 to 1 and store in the model's
      // input tensor
      tflInputTensor->data.f[sample_Num * 3 + 0] = (r) / 300.0;
      tflInputTensor->data.f[sample_Num * 3 + 1] = (g) / 300.0;
      tflInputTensor->data.f[sample_Num * 3 + 2] = (b) / 3000.0;
      
    
      if (millis() - lastUpdate > 500) {
        lastUpdate = millis();

        Serial.print(r);
        Serial.print(',');
        Serial.print(g);
        Serial.print(',');
        Serial.print(b);
        Serial.println();

        sample_Num++;
      }
    }
    if (sample_Num == total_Sample){
      // Run inferencing
      TfLiteStatus invokeStatus = tflInterpreter->Invoke();
      if (invokeStatus != kTfLiteOk) {
        Serial.println("Invoke failed!");
        while (1);
        return;
      }
      // Loop through the output tensor values from the model
        for (int i = 0; i < NUM_COLORS; i++) {
          Serial.print(COLORS[i]);
          Serial.print(": ");
          Serial.println(tflOutputTensor->data.f[i], 6);
        }
        Serial.println();
    }
  }
}
