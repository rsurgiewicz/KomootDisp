#pragma once

#define TFTW            135     // screen width
#define TFTH            240     // screen height
#define TFTW2           67     // half screen width
#define TFTH2           120     // half screen height
#define NAV_STREET_Y    140
#define NAV_DISTANCE_Y  190

void drawBlackScreen(){
  M5.Lcd.fillScreen(TFT_BLACK);
}

void drawinitScreen(){
//   M5.Lcd.drawBitmap(0,0,13,13,ic_nav_arrow_keep_right_bits);
  M5.Lcd.setCursor(5, 139+30);
  M5.Lcd.setFreeFont(&FreeMonoBold9pt7b);
  M5.Lcd.setTextColor(TFT_BLACK);
  M5.Lcd.printf("Komoot Disp");
  M5.Lcd.setCursor(20, 139+30+30);
  M5.Lcd.printf("by morf");
};


void drawKomootNavArrow(uint8_t kd){
  switch (kd)
  {
  case 1:
    M5.Lcd.drawBitmap(0,0,135,135,arrow_1);
    break;
  case 2:
    M5.Lcd.drawBitmap(0,0,135,135,arrow_1);
    break;
 case 4:
    M5.Lcd.drawBitmap(0,0,135,135,arrow_4);
    break;    
  case 5:
    M5.Lcd.drawBitmap(0,0,135,135,arrow_5);
    break; 
  case 6:
    M5.Lcd.drawBitmap(0,0,135,135,arrow_6);
    break; 
  case 7:
    M5.Lcd.drawBitmap(0,0,135,135,arrow_7);
    break; 
  case 8:
    M5.Lcd.drawBitmap(0,0,135,135,arrow_8);
    break; 
  case 9:
    M5.Lcd.drawBitmap(0,0,135,135,arrow_9);
    break; 
  case 10:
    M5.Lcd.drawBitmap(0,0,135,135,arrow_9);
    break; 
  case 11:
    M5.Lcd.drawBitmap(0,0,135,135,arrow_4);
    break; 
  case 12:
    drawBlackScreen();
    M5.Lcd.setCursor(10, 80);
    M5.Lcd.setTextColor(TFT_RED);
    M5.Lcd.setFreeFont(&FreeMonoBold24pt7b);
    M5.Lcd.printf("TURN");
    break; 
  //   M5.Lcd.drawBitmap(0,0,135,135,arrow_12);
  //   break; 
  case 15:
    M5.Lcd.drawBitmap(0,0,135,135,arrow_5);
    break; 
  case 16:
    M5.Lcd.drawBitmap(0,0,135,135,arrow_8);
    break; 
  case 17:
    M5.Lcd.drawBitmap(0,0,135,135,arrow_1);
    break; 
  case 18:
    M5.Lcd.drawBitmap(0,0,135,135,arrow_9);
    break; 
  case 19:
    M5.Lcd.drawBitmap(0,0,135,135,arrow_8);
    break; 
  case 20:
    M5.Lcd.drawBitmap(0,0,135,135,arrow_4);
    break; 
  case 21:
    M5.Lcd.drawBitmap(0,0,135,135,arrow_1);
    break; 
  case 22:
    M5.Lcd.drawBitmap(0,0,135,135,arrow_5);
    break; 
  case 29:
    M5.Lcd.drawBitmap(0,0,135,135,arrow_1);
    break; 
  case 30:
    drawBlackScreen();
    M5.Lcd.setCursor(10, 80);
    M5.Lcd.setTextColor(TFT_RED);
    M5.Lcd.setFreeFont(&FreeMonoBold24pt7b);
    M5.Lcd.printf("OUT");
    break; 
  
  default:
    

    break;
  }
}