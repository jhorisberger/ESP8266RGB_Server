#include <Arduino.h>
#include "radio.h"
#include "tft.h"



EspNow32 radio;
CustomTFT tft;


struct_payload txdata;
uint8_t Counter = 0;




void setup() {
  // start serial port
  Serial.begin(115200);
  Serial.println("\n\nESP8266 RGB controller server Debug \n -----------------------------------");

  // Setup TFT Display
  tft.setup();

  // Setup Radio
  radio.setup();
}


void loop() {

  Counter ++;
  if (Counter > 3) Counter = 0;
  txdata.SubSet_Type = 1;
  txdata.Subset_Index = Counter;
  txdata.SubSet_Range = 1;

  Serial.printf("\n\nTestdata Sent. Counter: %d\n",Counter);

  radio.payloadTx(txdata);


  delay(5000);
}