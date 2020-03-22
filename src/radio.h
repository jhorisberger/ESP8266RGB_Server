#ifndef _RADIO_H
#define _RADIO_H

    #include <esp_now.h>

    // User Setup
    #define WIFI_CHANNEL 4
    #define CLIENT_ID 0
    const uint8_t broadcastMac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    


    // Structure of the Data sent on the Radio
    typedef struct struct_data {
        uint8_t Command;
        uint8_t Data0;
        uint8_t Data1;
        uint8_t Data2;
        uint8_t Data3;
        uint8_t Data4;
    } struct_data;

    //Radio Tx/Rx Payload
    typedef struct struct_payload {
        uint8_t Key;
        uint8_t SubSet_Type;
        uint8_t Subset_Index;
        uint8_t SubSet_Range;
        struct_data Data;
        uint8_t CRC;
    } struct_payload;

    // On air Radio Tx/Rx Frame 
    typedef struct struct_frame {
        uint8_t Key;
        struct_payload Payload;
        uint8_t CRC;
    } struct_frame;

    
    class EspNow32 {
        private:
        // Create a struct_message to hold incoming Frame, Payload and Data 
        
        public:
            EspNow32();
            void setup(void);
            uint8_t payloadTx(struct_payload payload);
            void printFrame(struct_frame frame);

        private:
            static void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
            static void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
            static uint8_t evalRxFrame(struct_frame Payload, uint8_t clientId);
            uint8_t CRCcalc(struct_payload payload);
	};
    

    
#endif