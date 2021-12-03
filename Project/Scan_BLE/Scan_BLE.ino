#define RXD2 16
#define TXD2 17

#include "SoftwareSerial.h"
SoftwareSerial softSerial(17, 16); //TX, RX: Connect Arduino pin 2 to scanner TX pin. Connect Arduino pin 3 to scanner RX pin.

#include "SparkFun_DE2120_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_DE2120
DE2120 scanner;

#define BUFFER_LEN 40
char scanBuffer[BUFFER_LEN];
char barcode[BUFFER_LEN];

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

static BLEUUID SERVICE_UUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
static BLEUUID CHARACTERISTIC_UUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

BLEServer *pServer = BLEDevice::createServer();
BLEService *pService = pServer->createService(SERVICE_UUID);
BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_NOTIFY
                                       );

static boolean connected = false;

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEClient* pServer) {
    connected = true;
  }

  void onDisconnect(BLEClient* pServer) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Initiating BLE");
  if (scanner.begin(softSerial) == false)
  {
    Serial.println("Scanner did not respond. Please check wiring. Did you scan the POR232 barcode? Freezing...");
    while (1)
      ;
  }

  BLEDevice::init("ESP32+Scanner");
  pServer->setCallbacks(new MyServerCallbacks());
  pCharacteristic->setValue(barcode);
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("Scanner online!");
  Serial.println("Scan Product");
}

void loop() {
  if (connected == true) {
    Serial.println("We are now connected to the Food Databae");
    if (scanner.readBarcode(scanBuffer, BUFFER_LEN)){
      Serial.print("Code found: ");
      String(barcode) = scanBuffer;
      Serial.println(barcode);

      pCharacteristic->setValue(barcode.c_str());
      pCharacteristic->notify();

      Serial.println("Sent barcode : " + barcode);
    }
  } else {
      Serial.println("We have failed to connect to the Food Database; there is nothin more we will do.");
    }

  delay(1000);
}