#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "radio.h"
#include "crc/crc8.h"

esp_now_peer_info_t peerInfo;
EspNow32 _radio;
CRC8 crc8;


EspNow32::EspNow32(void){
  crc8.begin();
}

// Setup Radio
void EspNow32::setup(void){
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  Serial.print("MAC: "); Serial.println(WiFi.macAddress());
  Serial.println("\n");


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
void EspNow32::OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
};

// Callback when data is received
void EspNow32::OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  struct_frame frame;
  memcpy(&frame, incomingData, sizeof(frame));
  Serial.println(" ----------| Rx frame |----------");
  _radio.printFrame(frame);
  evalRxFrame(frame, CLIENT_ID);
};

// Check key, address and CRC of received frame
uint8_t EspNow32::evalRxFrame(struct_frame frame, uint8_t clientId){
  //Key
  if(frame.Key != 0xA8){
    Serial.println("Frame received, wrong key");
    return 1;  // Wrong Key
  }
  //CRC
  uint8_t crc = _radio.CRCcalc(frame.Payload);
  if(frame.CRC != crc) {
    Serial.printf("Frame received, wrong CRC: %d \n", crc);
    return 2;
  }
    //Address
   if(frame.Payload.SubSet_Type == 0 ||                                             // Broadcast
     (frame.Payload.SubSet_Type == 1 && frame.Payload.Subset_Index == CLIENT_ID)){  // Range
      Serial.println("Valid frame received");
      return 0;
     }
    Serial.println("Frame received, wrong address");
    return 3;
}

// transmit a payload after adding key and CRC
uint8_t EspNow32::payloadTx(struct_payload payload){
  struct_frame txFrame;
  txFrame.Key = 0xA8;
  txFrame.Payload = payload;
  
  txFrame.CRC =_radio.CRCcalc(payload);

  Serial.println(" ----------| Tx frame | ----------");
  printFrame(txFrame);
  return esp_now_send(broadcastMac, (uint8_t *) &txFrame, sizeof(txFrame));
}

// Calculate CRC of a payload
uint8_t EspNow32::CRCcalc(struct_payload payload){
  unsigned char frame_array[sizeof(payload)];
  memcpy(&frame_array, &payload, sizeof(payload));
  return crc8.get_crc8(frame_array, sizeof(payload));
}

// debug print frame 
void EspNow32::printFrame(struct_frame frame){
  Serial.printf("Key: %#x , CRC %d \n" , frame.Key, frame.CRC);
  Serial.printf("Subset: %#x %#x %#x , Data %#x | %#x %#x %#x %#x %#x \n\n" , frame.Payload.SubSet_Type, frame.Payload.Subset_Index, frame.Payload.SubSet_Range, frame.Payload.Data.Command, frame.Payload.Data.Data0, frame.Payload.Data.Data1, frame.Payload.Data.Data2, frame.Payload.Data.Data3, frame.Payload.Data.Data4);
}

