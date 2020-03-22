#include <esp_now.h>
#include <WiFi.h>
#include "radio.h"
#include "tft.h"



EspNow8266 radio;
CustomTFT tft;


struct_payload txdata;
uint8_t Counter = 0;
unsigned long LastSend;


void evaluateRx(struct_data RxData){
  float roundtripTime = (micros() - float(LastSend))/1000;
  if (RxData.Command == Counter && roundtripTime < 1000){
      Serial.print("Ping Time: "); Serial.print(roundtripTime, 3); Serial.println(" ms");
  }
  else{
     Serial.println("Packet lost!");
  }
}



void setup() {
  // start serial port
  Serial.begin(115200);
  Serial.println("");
  Serial.println("ESP8266 RGB controller server Debug");
  Serial.println("-----------------------------------");

  // Setup TFT Display
  tft.setup();

  // Setup Radio
  radio.setup();
  }

void loop() {

  Serial.println("Tick");

  Counter ++;
  txdata.Data.Command = Counter;
  LastSend = micros();

  // Send message via ESP-NOW
  esp_now_send(broadcastMac, (uint8_t *) &txdata, sizeof(txdata));

  delay(5000);
}