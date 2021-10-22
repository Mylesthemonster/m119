/*
  ESP32_with_delay
  LED Blink for 2 second period and a 25% duty cycle
  ESP32-DEVKITC-32D 
*/

const int ledPin = 16; //Used external LED with 220 ohm resistor 

void setup() {
  // initialize ledPin as an output.
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                   // wait for a second (T_on = 0.5sec)
  digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
  delay(1500);                  // wait for a second (T_off = 1.5sec)
  /*
   *  Period = T = T_on + T_off = 0.5sec + 1.5sec = 2sec
   *  Frequency = F = 1/T = 1/2sec = 0.5 hz
   *  Duty Cycle = D = T_on/T * 100 = 0.5sec/2sec * 100 = 25% 
   */
}
