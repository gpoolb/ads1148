#include <ADS1148.h>

/**
* Internal Temp Example for using the Arduino Library for the Texas Instruments ADS1148
*/

// Definition of the pins to be used
#define CS      32
#define DRDY    27
#define START   33
#define RESET   13

// Creating an ADS1148 object
ADS1148 ads1148(CS, DRDY, START, RESET, &SPI);

void setup(){
  Serial.begin(115200);
  
	// Initialize the ADS1148
  Serial.println(ads1148.begin() ? "ADS1148 is connected" : "ADS1148 not found");

  ads1148.setReadingInternalTemperature();
}


void loop(){
    
  if(ads1148.isDataReady())
      Serial.printf("Int Temp: %0.2f C\n", ads1148.readInternalTemperature());
      
}