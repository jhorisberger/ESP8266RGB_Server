#ifndef _RADIO_H
#define _RADIO_H

    #include <Arduino.h>

    typedef struct struct_data {
        uint8_t Command;
        uint8_t Data0;
        uint8_t Data1;
        uint8_t Data2;
        uint8_t Data3;
        uint8_t Data4;
    } struct_data;

    //Structure Radio Tx/Rx Payload
    //Must match the receiver structure
    typedef struct struct_payload {
        uint8_t SubSet_Type;
        uint8_t Subset_Index;
        uint8_t SubSet_Range;
        struct_data Data;
    } struct_payload;



    void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
    void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
#endif