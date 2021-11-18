#include "BLEDevice.h"

// The remote service we wish to connect to.
static BLEUUID serviceUUID("32e6be96-b061-43e3-82fe-26dcf1126d37");
// The characteristics of the remote service we are interested in.
static BLEUUID charUUID_IMU("0d6a35ba-8d49-4894-890b-9d25a03e3ccf");
static BLEUUID charUUID_Prox("823b3728-c42f-4d23-a9fe-4ed3442f5f23");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic_imu;
static BLERemoteCharacteristic* pRemoteCharacteristic_prox;
static BLEAdvertisedDevice* myDevice;

static void notifyCallback_imu(
  BLERemoteCharacteristic* pBLERemoteCharacteristic_imu,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.println("Current IMU reading is: ");
    int val = *(int*)pData;
    Serial.print(val);
    Serial.println("");
}

static void notifyCallback_prox(
  BLERemoteCharacteristic* pBLERemoteCharacteristic_prox,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.println("Current Proximity reading is: ");
    int val = *(int*)pData;
    Serial.print(val);
    Serial.println("");
 }

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    pClient->connect(myDevice);
    Serial.println(" - Connected to server");

    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    
    Serial.println(" - Found our service");

    // Obtain a reference to the IMU characteristic in the service of the remote Nano 33 BLE Sense server.
    pRemoteCharacteristic_imu = pRemoteService->getCharacteristic(charUUID_IMU);
    
    // Obtain a reference to the Prox characteristic in the service of the remote Nano 33 BLE Sense server.
    pRemoteCharacteristic_prox = pRemoteService->getCharacteristic(charUUID_Prox);
    
    Serial.println(" - Found our characteristics");

    if(pRemoteCharacteristic_imu->canNotify())
        pRemoteCharacteristic_imu->registerForNotify(notifyCallback_imu);

    if(pRemoteCharacteristic_prox->canNotify())
        pRemoteCharacteristic_prox->registerForNotify(notifyCallback_prox);

}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;
    }
  } 
}; 

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");
  
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
}


void loop() {
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the Nano 33 BLE Sense Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }
  delay(1000);
}
