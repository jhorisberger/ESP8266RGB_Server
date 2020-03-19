#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>       // Hardware-specific library
#include <esp_now.h>
#include <WiFi.h>

#include "radio.h"


TFT_eSPI tft = TFT_eSPI();


#define WIFI_CHANNEL 4
uint8_t broadcastMac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};


struct_payload txdata;
uint8_t Counter = 0;
unsigned long LastSend;


void evaluateTx(void){
  float sendtime = (micros() - float(LastSend))/1000;
  Serial.print("Sendtime: "); Serial.print(sendtime, 3); Serial.println(" ms");
}



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


  // Setup Radio
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    Serial.print("MAC: "); Serial.println(WiFi.macAddress());
    Serial.println("");
    Serial.println("");

    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
    }

    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Trasnmitted packet
    esp_now_register_send_cb(OnDataSent);
    
    // Register peer
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, broadcastMac, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
    
    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
      Serial.println("Failed to add peer");
      return;
    }
    // Register for a callback function that will be called when data is received
    esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  Serial.println("Tick");

  Counter ++;
  txdata.Data.Command = Counter;
  LastSend = micros();

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastMac, (uint8_t *) &txdata, sizeof(txdata));

  delay(1000);
}