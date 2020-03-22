#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>       // Hardware-specific library
#include "tft.h"



CustomTFT::CustomTFT(void){ 
}

void CustomTFT::setup(void){
    tft.init();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(0, 0);

    pinMode(4, OUTPUT);       // Backlight Enable
    digitalWrite(4, HIGH);


    // Setup static Text
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    tft.fillRect(0,0,320,16, TFT_BLUE);
    tft.drawCentreString("Pfuuser Lightshow Controller",160,0,2);
}