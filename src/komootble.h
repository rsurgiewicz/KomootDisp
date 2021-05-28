#pragma once
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
extern BLEClient*  pClient;
extern BLEAdvertisedDevice* myDevice;
extern BLERemoteCharacteristic* pRemoteCharacteristic;

static BLEUUID serviceUUID("71C1E128-D92F-4FA8-A2B2-0F171DB3436C"); // navigationServiceUUID
static BLEUUID charUUID("503DD605-9BCB-4F6E-B235-270A57483026");    // navigationServiceNotifyCharacteristicUUI
static BLEUUID heartUUID("6D75DBF0-D763-4147-942A-D97B1BC700CF");   // navigationServiceHeartbeatWriteCharacteristicUUID


extern boolean doConnect;
extern boolean connected;
extern boolean doScan;

extern boolean new_notify;
extern boolean new_street;
extern int heartbeat;
extern int timeout;
extern int scan_time;

extern uint8_t old_data[20];
extern uint8_t kdir;
extern uint32_t kdistance;
extern uint8_t kstreet[11];

extern unsigned long lastTime;
extern unsigned long currentTime;
extern unsigned long lastHearbeat;


class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient);
  void onDisconnect(BLEClient* pclient);
};

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    //Called for each advertising BLE server.
    void onResult(BLEAdvertisedDevice advertisedDevice);
};

void notifyCallback(  BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData,  size_t length, bool isNotify);
void disconnectFromServer();
bool connectToServer();

