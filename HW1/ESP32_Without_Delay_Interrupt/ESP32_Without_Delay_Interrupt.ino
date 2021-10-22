/*
  ESP32_without_delay_interrupt
  LED Blink for 2 second period and a 25% duty cycle
  ESP32-DEVKITC-32D 
*/
const int ledPin = 16; //External LED with 220 ohm resistor 
const int intrptPin = 17; //Interrupt Pin number

unsigned int counter = 0; //create a counter 

// On and Off Times
const unsigned int T_on = 500;
const unsigned int T_off = 1500;
 
unsigned long previousTime = 0; // Tracks the last time event fired

int interval = T_on; // Interval is how long we wait
 
boolean LED = true; // Used to track if LED should be on or off

void IRAM_ATTR isr() {
  Serial.println("Interrupt Detected");
  counter += 1;
  Serial.println(LED);
}

void setup() {
  // initialize ledPin as an output.
  pinMode(ledPin, OUTPUT);

  attachInterrupt(intrptPin, isr, HIGH); //Attach interrupt to GPIO pin 4,intrrupts when LOW
  
  Serial.begin(9600); // Serial Monitor at a data rate of 9600 bits per second
  while (!Serial);
  Serial.println("Started");

  //Formating for Serial Output
  Serial.print("Toff and Ton");
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

  //Print out T_off/T_on then a tab
    Serial.print(T_off);
    Serial.print('\t');
    Serial.print(T_on);
    Serial.print('\t');
    Serial.println(counter);
  
}
