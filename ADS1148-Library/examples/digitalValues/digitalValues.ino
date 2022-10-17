#include <ADS1148.h>

/**
* Single Ended Example for using the Arduino Library for the Texas Instruments ADS1148
* Tested with arduino nano and NodeMCU ESP32-WROOM-32D
*/

// Definition of the pins to be used
#if defined(__AVR__)
  #define CS      4
  #define DRDY    3
  #define START   5
  #define RESET   2
#elif defined(ESP32)
  #define CS      32
  #define DRDY    27
  #define START   33
  #define RESET   13
#endif

// Creating an ADS1148 object
#if defined(__AVR__)
  ADS1148 ads1148(CS, DRDY, START, RESET);
#elif defined(ESP32)
  ADS1148 ads1148(CS, DRDY, START, RESET, &SPI);
#endif

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;           // interval at which to blink (milliseconds)
bool flag = false;

void setup(){
  Serial.begin(115200);
  
  // Initialize the ADS1148
  Serial.println(ads1148.begin() ? "ADS1148 is connected" : "ADS1148 not found");

  ads1148.pinModeGPIO(ADS1148_GPIO0_PIN, OUTPUT);
}


void loop(){
    
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    flag = !flag;
    if (flag)
      ads1148.digitalWriteGPIO(ADS1148_GPIO0_PIN, HIGH);
    else
      ads1148.digitalWriteGPIO(ADS1148_GPIO0_PIN, LOW);
  }

}
