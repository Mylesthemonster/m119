#include <Arduino_APDS9960.h>

const int total_Sample = 50;   // total number of voice instance

int sample_Num = 0;
unsigned long lastUpdate = 0;

void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial);

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor!");
  }
  
  delay(900);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("$~~~~~Starting Color Detection~~~~~$");
  Serial.println("R,G,B");
}
void loop() {
  int r = 0, g = 0, b = 0;

  while (sample_Num < total_Sample ){
    if (APDS.colorAvailable()) {
      APDS.readColor(r, g, b);
    
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
      Serial.println();
      while (1) {
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      }        
    }
  }
}
