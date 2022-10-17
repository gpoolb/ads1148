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

void setup(){
  Serial.begin(115200);
  
  // Initialize the ADS1148
  Serial.println(ads1148.begin() ? "ADS1148 is connected" : "ADS1148 not found");

  ads1148.setADCPositiveInput(POSITIVE_INPUT_AIN0);       // default values
  ads1148.setADCNegativeInput(NEGATIVE_INPUT_AIN1);       // default values
  ads1148.setOpMode(NORMAL_OPERATION);                    // default values
  ads1148.setGain(GAIN_PGA_TO_1);                         // default values
  ads1148.setDataRate(DATA_RATE_TO_5_SPS);                // default values
}

#if defined(__AVR__)
  char buffer[32];
  char str_voltage[12];
#endif

void loop(){
    
  if(ads1148.isDataReady()){
      #if defined(__AVR__)
      dtostrf(ads1148.readInputVoltage(), 6, 4, str_voltage);
      sprintf (buffer, "Input Value:   %s V\n", str_voltage);
      Serial.print(buffer);
    #elif defined(ESP32)
      Serial.printf("Input Value:   %0.4f V\n", ads1148.readInputVoltage());
    #endif
  }
}
