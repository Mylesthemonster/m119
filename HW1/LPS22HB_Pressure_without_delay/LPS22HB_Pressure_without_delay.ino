/*
  LPS22HB_Pressure_without_delay.ino
  Arduino Nano 33 BLE Sense
*/

#include <Arduino_LPS22HB.h> // Library for LPS22HB

const unsigned long interval = 5000; // 5sec interval
unsigned long previousTime = 0;

void setup() {
  Serial.begin(9600); // Serial Monitor at a data rate of 9600 bits per second
  
  while (!Serial);
  Serial.println("Started");

  //If Pressure sensor doesnt start up have serial output failure
  if (!BARO.begin()) {
    Serial.println("Failed to initialize pressure sensor!");
    while (1);
  }
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - previousTime >= interval) {
    previousTime = currentTime; // save the last executed time
 
    /// Pass PSI, millibar or kilopascal (deafult) to readPressure to read the sensor values in respective units
    float pressure = BARO.readPressure(PSI);
  
    // print the sensor value
    Serial.print("Pressure = ");
    Serial.print(pressure);
    Serial.println(" PSI");
    Serial.println();
  }
}
