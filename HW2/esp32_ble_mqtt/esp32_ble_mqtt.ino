
#include "BLEDevice.h"
#include "UbidotsEsp32Mqtt.h"

unsigned long timer;

int imu_val;
int prox_val;

Ubidots ubidots("BBFF-QkJYCsh8Do8GJPi6phv9sKhYf33AfY");

static BLEUUID serviceUUID("32e6be96-b061-43e3-82fe-26dcf1126d37");
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
    imu_val = *(int*)pData;
    Serial.print(imu_val);
    Serial.println("");
}

static void notifyCallback_prox(
  BLERemoteCharacteristic* pBLERemoteCharacteristic_prox,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.println("Current Proximity reading is: ");
    prox_val = *(int*)pData;
    Serial.print(prox_val);
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
  
  ubidots.connectToWifi("UCLA_WEB", "");
  ubidots.setup();
  ubidots.reconnect();

  timer = millis();
  
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
  
  if (!ubidots.connected())
    ubidots.reconnect();
  if (abs(millis() - timer) > 1000)
  {
    float value = 0;
    ubidots.add("IMU Data", imu_val);
    ubidots.add("PROX Data", prox_val);
    ubidots.publish("ESP32");
    timer = millis();
  }
  ubidots.loop(); 
}
