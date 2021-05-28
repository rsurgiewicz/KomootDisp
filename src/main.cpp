#include <Arduino.h>
#include "M5StickCPlus.h"
#include "bitmaps.h"
#include "komoot_bitmaps.h"
#include "komootble.h"
#include "helpers.h"

#define NOTIFICATION_TIME 5000
#define NOTIFICATION_TIMEOUT 10000
double vbat = 0.0;
int charge;


void KomootBLESetup (){

  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
  lastTime = millis();
}



void setup() {
  M5.begin();
  M5.Lcd.fillScreen(WHITE);
  pinMode(M5_BUTTON_HOME,INPUT_PULLUP);
  pinMode(M5_BUTTON_RST,INPUT_PULLUP);
  uint8_t i=0;
  for (i=7;i<15;i++)
  {
    M5.Axp.ScreenBreath(i);
    delay(100);
  }

  drawinitScreen();
  M5.Lcd.setTextFont(2);
  vbat = M5.Axp.GetVbatData() * 1.1 / 1000;
  M5.Lcd.setCursor(5, 10);
  M5.Lcd.printf("%.3fV\r\n", vbat);
  Serial.println("Init BLE..");
  KomootBLESetup();
  drawBlackScreen();
  lastHearbeat = millis();
  
 
}
void loop() {

  if (doScan == true)
  {
    Serial.println("STIL NOT CONNECTED");
    KomootBLESetup();
  }
  if (doConnect == true) {
    if (connectToServer()) {
      connected = true;
      Serial.println("Connected");
    } else {
      connected = false;
      Serial.println("NOT CONNECTED");
    }
    doConnect = false;
  }
  
  
  if (new_notify)
  {

    // Serial.print("* A:");
    // Serial.print(kdir,DEC);
    // Serial.print(" S: ");
    // Serial.print((char *) kstreet);
    // Serial.print(" D: ");
    // Serial.print(kdistance,DEC);
    // Serial.println("");

    drawBlackScreen();
    drawKomootNavArrow(kdir);
    // M5.Lcd.drawRect(0,NAV_STREET_Y,TFT_WIDTH,TFT_HEIGHT-NAV_STREET_Y,TFT_BLACK);
    M5.Lcd.setCursor(5, NAV_STREET_Y);
    
    //  M5.Lcd.setFreeFont(&FreeMonoBold9pt7b);
    M5.Lcd.setFreeFont(&FreeMonoBold9pt7b);
    M5.Lcd.setTextColor(TFT_WHITE);
    M5.Lcd.printf((char *) kstreet);

    M5.Lcd.setCursor(10, NAV_DISTANCE_Y);
    M5.Lcd.setTextColor(TFT_WHITE);
    M5.Lcd.setFreeFont(&FreeMonoBold24pt7b);
    if (kdistance<1000)
    {
      M5.Lcd.printf("%i",kdistance);
    }else if (kdistance>=10000)
    {
      M5.Lcd.printf("%ik",(kdistance/1000));
    }else if (kdistance>=1000)
    {
      M5.Lcd.printf("%2.1fk",(kdistance/1000.0));
    }
    M5.Axp.ScreenBreath(15);
    
    delay(NOTIFICATION_TIME);
    lastTime = millis();

    if (kdistance>50){
      uint8_t i=0;
      for (i=15;i>0;i--){
        M5.Axp.ScreenBreath(i);
        delay(50);
      }
    }

    M5.Lcd.setTextFont(1);
    vbat = M5.Axp.GetVbatData() * 1.1 / 1000;
    M5.Lcd.setCursor(5, TFT_HEIGHT-10, 1);
    M5.Lcd.printf("%.3fV\r\n", vbat);
  
    int disCharge = M5.Axp.GetIdischargeData() / 2;
    
    M5.Lcd.setCursor(TFT_WIDTH-20, TFT_HEIGHT-10, 1);
    M5.Lcd.printf("%dma\r\n", disCharge);
    delay(500);

    

    new_notify = false;
    
    
  }


  if ((millis()-lastHearbeat)>NOTIFICATION_TIMEOUT){
      Serial.println("No messages, restarting ");
      ESP.restart();

    
  }

  // Serial.println("loop");

  if (digitalRead(M5_BUTTON_RST) == LOW){
    while(digitalRead(M5_BUTTON_RST)==LOW);
    ESP.restart();
  }

  if (digitalRead(M5_BUTTON_HOME) == LOW){
    while(digitalRead(M5_BUTTON_HOME)==LOW);
    ESP.restart();
  }


  


}