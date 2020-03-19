#include <Arduino.h>
#include <esp_now.h>
#include "radio.h"

void evaluateRx(struct_data RxData); // Prototype
void evaluateTx(void);


// Create a struct_message to hold incoming Payload
struct_payload RxPayload;
struct_data RxData;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//  Serial.print("\r\nLast Packet Send Status:\t");
//  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
evaluateTx();
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&RxPayload, incomingData, sizeof(RxPayload));
  //Serial.print("Bytes received: "); Serial.println(len);
  RxData = RxPayload.Data;

  evaluateRx(RxData);
}