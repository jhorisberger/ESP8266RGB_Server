#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>       // Hardware-specific library



TFT_eSPI tft = TFT_eSPI();


void setup() {
  // start serial port
  Serial.begin(115200);
  Serial.println("");
  Serial.println("ESP8266 RGB controller server Debug");
  Serial.println("-----------------------------------");
  Serial.println("");
  Serial.println("");

  // Setup TFT Display
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);


  // Setup static Text
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.fillRect(0,0,320,16, TFT_BLUE);
  tft.drawCentreString("Pfuuser Lightshow Controller",160,0,2);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Tick");
  delay(1000);
}