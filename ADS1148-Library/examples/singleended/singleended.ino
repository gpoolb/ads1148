#include <ADS1148.h>

/**
* Single Ended Example for using the Arduino Library for the Texas Instruments ADS1148
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

  ads1148.setADCPositiveInput(POSITIVE_INPUT_AIN0);       // default values
  ads1148.setADCNegativeInput(NEGATIVE_INPUT_AIN1);       // default values
  ads1148.setOpMode(NORMAL_OPERATION);                    // default values
  ads1148.setGain(GAIN_PGA_TO_1);                         // default values
  ads1148.setDataRate(DATA_RATE_TO_5_SPS);                // default values
}


void loop(){
    
  if(ads1148.isDataReady())
  Serial.printf("Input Value:   %0.4f V\n", ads1148.readInputVoltage());

}
