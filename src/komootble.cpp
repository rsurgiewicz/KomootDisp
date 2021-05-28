#include "komootble.h"

BLEClient*  pClient;
BLEAdvertisedDevice* myDevice;
BLERemoteCharacteristic* pRemoteCharacteristic;

// static BLEUUID serviceUUID("71C1E128-D92F-4FA8-A2B2-0F171DB3436C"); // navigationServiceUUID
// static BLEUUID charUUID("503DD605-9BCB-4F6E-B235-270A57483026");    // navigationServiceNotifyCharacteristicUUI
// static BLEUUID heartUUID("6D75DBF0-D763-4147-942A-D97B1BC700CF");   // navigationServiceHeartbeatWriteCharacteristicUUID


boolean doConnect = false;
boolean connected = false;
boolean doScan = false;

boolean new_notify = false;
boolean new_street = false;
int heartbeat = 0;
int timeout = 0;
int scan_time = 0;
uint8_t old_data[20];
uint8_t kdir;
uint32_t kdistance;
uint8_t kstreet[11];


unsigned long lastTime;
unsigned long lastHearbeat;
unsigned long currentTime;


void MyClientCallback::onConnect(BLEClient* pclient) {
    connected = true;
    doConnect = false;
  }

void MyClientCallback::onDisconnect(BLEClient* pclient) {
    connected = false;
    doConnect = true;
    Serial.println("onDisconnect");
  }


void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    lastHearbeat = millis();

    new_notify = false;

    memcpy(old_data,pData,20);

    // DIRECTION
    if (kdir != old_data[4]){
      new_notify = true;
      new_street = true;
    }
    
    kdir = old_data[4];
    
    // STREET
    // Serial.printf((char *) pData);
    Serial.printf(" Length %d",length);
    Serial.println("");

    int i = 0;
    for (i=9; i<length; i++){
      // kstreet[i-9]='\0';
      if (kstreet[i-9]!=old_data[i])
      {
        new_notify = true;
        new_street = true;
        break;
      };
    };

    for (i=0; i<(length-9);i++){
      kstreet[i]=old_data[9+i];
    };

    for(i=(length-9);i<11;i++){
      kstreet[i]=' ';
    }
    
    // DISTANCE
    
    uint32_t kdist = old_data[5] | (old_data[6] << 8) | (old_data[7] << 16) | (old_data[8] << 24);



    kdistance = kdist;


    // if (kdistance != kdist)
    if (kdistance<50)
      new_notify = true;

    if (kdistance>100 )
      new_notify = true;

    if (((millis()-lastTime)>60000) && (kdistance>1000))
      new_notify = true;

    if (((millis()-lastTime)>30000) && (kdistance>800))
      new_notify = true;

    if (((millis()-lastTime)>10000) && (kdistance<500))
      new_notify = true;

    if (new_notify)
      lastTime = millis();
}


void disconnectFromServer(){
  if (connected == true){
    pClient->disconnect();
  }
}


bool connectToServer() {
  Serial.print("Forming a connection to ");
  Serial.println(myDevice->getAddress().toString().c_str());
  
  // BLEClient*  pClient  = BLEDevice::createClient();
  pClient = BLEDevice::createClient();
  Serial.println(" - Created client");

  pClient->setClientCallbacks(new MyClientCallback());
  
  pClient->connect(myDevice);
  Serial.println(" - Connected to server");
  // Obtain a reference to the service we are after in the remote BLE server.
    // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our service");


  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found Komoot characteristic!");

  // Read the value of the characteristic.
  // std::string value = pRemoteCharacteristic->readValue();
  // pRemoteCharacteristic->registerForNotify(notifyCallback);
  // Display that BLE has been connected
  // show_message("BLE connected",34);

  // delay(1000); // 1s delay (reduced clock)
  // if (pRemoteCharacteristic->canRead()) {
    // Serial.println (" - can read");
    // std::string value = pRemoteCharacteristic->readValue();
    // Serial.print("The characteristic value was: ");
    // Serial.println(value.c_str());
  // } else {
    // Serial.println (" - can NOT read");
  // }

  delay(1000); // 1s delay (reduced clock)
  if (pRemoteCharacteristic->canNotify()) {
    Serial.println (" - can notify");
    pRemoteCharacteristic->registerForNotify(notifyCallback);
  } else {
    Serial.println (" - can NOT notify");
  }

  connected = true;
  doConnect = false;



  Serial.println ("Connected to desired service on BLE server");
  return true;
}



// Scan for BLE servers and find the first one that advertises the service we are looking for.
void MyAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice) {
      
      // Serial.println(advertisedDevice.getServiceUUID().toString().c_str());
      // We have found a device, let us now see if it contains the service we are looking for.
      if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
        BLEDevice::getScan()->stop();
        Serial.print("BLE Advertised Device found: ");
        Serial.println(advertisedDevice.toString().c_str());
        myDevice = new BLEAdvertisedDevice(advertisedDevice);
        doConnect = true;
        doScan = false;
        return ;
      } // Found our server

      //Server sadly not found
      doConnect = false;
      doScan = true;
   
}; // MyAdvertisedDeviceCallbacks