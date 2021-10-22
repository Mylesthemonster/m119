/*
  LSM9DS1_Gyroscope_without_delay.ino
  Arduino Nano 33 BLE Sense
*/

#include <Arduino_LSM9DS1.h> // Library for LSM9DS1

const unsigned long interval = 1000; // 1sec interval
unsigned long previousTime = 0;


void setup() {
  Serial.begin(9600); // Serial Monitor at a data rate of 9600 bits per second
  
  while (!Serial);
  Serial.println("Started");

  //If IMU doesnt start up have serial output failure
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  //Formating for Serial Output
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate()); // States smapling rate of gyroscope = 120hz
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
  Serial.println("X\tY\tZ"); // Label the axis' being recorded
}

void loop() {
  float x, y, z; // use float to use deciamal values vs whole int

  unsigned long currentTime = millis();

  if (currentTime - previousTime >= interval) {
    previousTime = currentTime; // save the last executed time

    //Assigns gyscope data to float varibles 
    if (IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(x, y, z);
      
  
      //Print out each axis data point then a tab
      Serial.print(x);
      Serial.print('\t');
      Serial.print(y);
      Serial.print('\t');
      Serial.println(z);
    }
  }
}
