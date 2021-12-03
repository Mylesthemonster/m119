#include <ArduinoBLE.h> // Library for BLE connection
#include <Arduino_LSM9DS1.h> // Library for LSM9DS1 - IMU
#include <Arduino_APDS9960.h> // Library for APDS9960 -  Proximity Sensor

int accel;
int prox;
float x, y, z;

BLEService SensorService("32e6be96-b061-43e3-82fe-26dcf1126d37");
BLEUnsignedIntCharacteristic IMUChar("0d6a35ba-8d49-4894-890b-9d25a03e3ccf", BLERead | BLENotify);
BLEUnsignedIntCharacteristic ProxChar("823b3728-c42f-4d23-a9fe-4ed3442f5f23", BLERead | BLENotify);

void setup() {
  Serial.begin(115200); // Serial Monitor at a data rate of 9600 bits per second
  
  while (!Serial);
  Serial.println("Started");

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println(peripheral.address());
  
  //If Proximity Sensor doesnt start up have serial output failure
  if (!APDS.begin()) {
    Serial.println("*Failed to initialize APDS9960 sensor!");
  } 
  
  //If IMU doesnt start up have serial output failure
  if (!IMU.begin()) {
    Serial.println("*Failed to initialize IMU!");
    while (1);
  }
  
  //If BLE doesnt start up have serial output failure
  if (!BLE.begin()) {
    Serial.println("*Failed to initialize BLE module!");
    while (1);
  }

  BLE.setLocalName("Nano 33 BLE Sense");
  
  BLE.setAdvertisedService(SensorService);
  SensorService.addCharacteristic(IMUChar);
  SensorService.addCharacteristic(ProxChar);
  BLE.addService(SensorService);
  IMUChar.writeValue(accel);
  ProxChar.writeValue(prox);

  BLE.advertise();
 
  Serial.println("Bluetooth device is now active, waiting for connections...");
}

void loop() {

BLEDevice central = BLE.central();

if (central) {
  Serial.print("Connected to central: ");
  Serial.println(central.address());
  digitalWrite(LED_BUILTIN, HIGH);
  
  while (central.connected()) {
    delay(1000);
    read_Accel();
    read_Prox();

    IMUChar.writeValue(accel);
    ProxChar.writeValue(prox);

    Serial.print("Acceleration = ");
    Serial.print(accel);
    Serial.println("");
    Serial.println("");
    Serial.print("Proximity = ");
    Serial.print(prox);
    Serial.println("");
    Serial.println("");
    }
  }
  
digitalWrite(LED_BUILTIN, LOW);
Serial.print("Disconnected from central: ");
Serial.println(central.address());
delay(1000);
}

void read_Accel() {

if (IMU.accelerationAvailable()) {
  IMU.readAcceleration(x, y, z);
  accel = x*9.80665; // x in g convert to m/s^2 
  }
}

void read_Prox() {

if (APDS.proximityAvailable()) {
    prox = APDS.readProximity();
  }
}
