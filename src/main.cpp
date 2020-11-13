#include <Arduino.h>
#include "radio.h"
#include "tft.h"



EspNow32 radio;
CustomTFT tft;


struct_payload txdata;
const byte numChars = 16;
char receivedChars[numChars]; // an array to store the received data
int testdata = 0;
boolean newData = false;





void setup() {
  // start serial port
  Serial.begin(115200);
  Serial.println("\n\nESP8266 RGB controller server Debug \n -----------------------------------");

  // Setup TFT Display
  tft.setup();

  // Setup Radio
  radio.setup();
}

void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\r';
  char rc;
 
  //if (Serial.available() > 0) {
  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
      else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void showNewData() {
 if (newData == true) {
  Serial.print("\n This just in ... ");
  Serial.println(receivedChars);
  Serial.println("Sending Testdata...");
  testdata = atoi(receivedChars);

  txdata.SubSet_Type = 1;       // Select range
  txdata.Subset_Index = 1;      // ID 1
  txdata.SubSet_Range = 1;      // Range 1

  txdata.Data.Command = 0xF0;   // Set Color Direct

  txdata.Data.Data0 = testdata;
  txdata.Data.Data1 = testdata/2;
  txdata.Data.Data2 = testdata/10;
  txdata.Data.Data3 = 0;

  
  radio.payloadTx(txdata);

  newData = false;
  }
};

void loop() {

  recvWithEndMarker();
  showNewData();

}

