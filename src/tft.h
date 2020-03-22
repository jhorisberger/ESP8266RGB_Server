#ifndef _TFT_H
#define _TFT_H

#include <TFT_eSPI.h>
// User Setup



class CustomTFT {
        private:
        TFT_eSPI tft = TFT_eSPI();
        
        public:
        CustomTFT();
        void setup();
	};


#endif