#define RXD2 16
#define TXD2 17

#include <SoftwareSerial.h>
SoftwareSerial softSerial(17, 16); //TX, RX: Connect Arduino pin 2 to scanner TX pin. Connect Arduino pin 3 to scanner RX pin.

#include <SparkFun_DE2120_Arduino_Library.h> //Click here to get the library: http://librarymanager/All#SparkFun_DE2120
DE2120 scanner;

#define BUFFER_LEN 40
char scanBuffer[BUFFER_LEN];
char barcode[BUFFER_LEN];

void setup() {
  Serial.begin(115200);
  Serial.println("Initiating BLE");
  if (scanner.begin(softSerial) == false)
  {
    Serial.println("Scanner did not respond. Please check wiring. Did you scan the POR232 barcode? Freezing...");
    while (1)
      ;
  }
}

void loop() {
  if (scanner.readBarcode(scanBuffer, BUFFER_LEN)){
    Serial.print("Code found: ");
    String(barcode) = scanBuffer;
    Serial.println(barcode);
  }
}
