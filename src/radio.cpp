#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "radio.h"
#include "crc/crc8.h"

void evaluateRx(struct_data RxData); // Prototype



  
  EspNow8266::EspNow8266(void){
  }

  // Setup Radio
  void EspNow8266::setup(void){
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
    memcpy(peerInfo.peer_addr, broadcastMac, 6);
    peerInfo.channel = WIFI_CHANNEL;  
    peerInfo.encrypt = false;
    
    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
      Serial.println("Failed to add peer");
      return;
    }
    // Register for a callback function that will be called when data is received
    esp_now_register_recv_cb(OnDataRecv);
  };


  // Callback when data is sent
  void EspNow8266::OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //  Serial.print("\r\nLast Packet Send Status:\t");
  //  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  };


  // Callback when data is received
  void EspNow8266::OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    struct_frame frame;

    memcpy(&frame, incomingData, sizeof(frame));
    Serial.print("Bytes received: "); Serial.println(len);

    evalRxFrame(frame);
  };

  
uint8_t EspNow8266::evalRxFrame(struct_frame Payload){

}
