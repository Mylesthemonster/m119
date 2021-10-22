/*
  ESP32_without_delay
  LED Blink for 2 second period and a 25% duty cycle
  ESP32-DEVKITC-32D 
*/
const int ledPin = 16; //External LED with 220 ohm resistor 

// On and Off Times
const unsigned int T_on = 500;
const unsigned int T_off = 1500;
 
unsigned long previousTime = 0; // Tracks the last time event fired

int interval = T_on; // Interval is how long we wait
 
boolean LED = true; // Used to track if LED should be on or off

void setup() {
  // initialize ledPin as an output.
  pinMode(ledPin, OUTPUT);
}

void loop() {
  
  digitalWrite(ledPin, LED);
 
  unsigned long currentTime = millis();
 
  if ((currentTime - previousTime) >= interval) {
    
    // Change wait interval, based on current LED state
    if (LED) {
      interval = T_off; // If the LED on set time to stay off
    } else {
      interval = T_on; // Otherwise LED is off so set time to stay on
    }
    
    LED = !(LED); //Flips LED bool value like a clk 
 
    previousTime = currentTime; // Save the currentTime for next loop
  }
}
