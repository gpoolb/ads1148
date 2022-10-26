#include <ADS1148.h>

/**
* Internal Temp Example for using the Arduino Library for the Texas Instruments ADS1148
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
  //ADS1148 ads1148(CS, DRDY, START, RESET);
  ADS1148 ads1148(CS);
#elif defined(ESP32)
  //ADS1148 ads1148(CS, DRDY, START, RESET, &SPI);
  ADS1148 ads1148(CS, &SPI);
#endif

void setup(){
  Serial.begin(115200);
  
	// Initialize the ADS1148
  Serial.println(ads1148.begin() ? "ADS1148 is connected" : "ADS1148 not found");

  ads1148.setReadingInternalTemperature();
}

#if defined(__AVR__)
  char buffer[32];
  char str_temp[12];
#endif

uint8_t counter = 0;

void loop(){
    
  if(ads1148.isDataReady()){
    #if defined(__AVR__)
      dtostrf(ads1148.readInternalTemperature(), 6, 2, str_temp);
      sprintf (buffer, "Int Temp: %s C, %d\n", str_temp, counter++);
      Serial.print(buffer);
    #elif defined(ESP32)
      Serial.printf("Int Temp: %0.2f C, %d\n", ads1148.readInternalTemperature(), counter++);
    #endif
  }
}