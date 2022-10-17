#include <ADS1148.h>

/**
* Thermistor Example for using the Arduino Library for the Texas Instruments ADS1148
* NTC thermistor is connected between to AIN0 and AIN1
* IEXC1 pin is connected to AIN0 pin
* and R1 is connected between to AIN1 to GND
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

double R1 = 10000.0;    // Voltage divider resistor value
double Beta = 3977.0;   // Beta value
double To = 298.15;     // Temperature in Kelvin for 25 degree Celsius
double Ro = 10000.0;    // Resistance of Thermistor at 25 degree Celsius

void setup(){
  Serial.begin(115200);

  // Initialize the ADS1148
  Serial.println(ads1148.begin() ? "ADS1148 is connected" : "ADS1148 not found");

  ads1148.setADCPositiveInput(POSITIVE_INPUT_AIN0); // default values
  ads1148.setADCNegativeInput(NEGATIVE_INPUT_AIN1); // default values
  ads1148.setGain(GAIN_PGA_TO_1);                   // Gain 1
  ads1148.setDataRate(DATA_RATE_TO_5_SPS);          // data rate 5 sps
  ads1148.setOpMode(NORMAL_OPERATION);              // Normal mode
  // Thermistor and reference must be powered by same voltage
  ads1148.setIDACcurrent(IDAC_CURRENT_100UA);       // Set IDAC Modules to 100 uA
  ads1148.setIDAC1output(IDAC1_CURRENT_TO_IEXC1);   // Set IDAC1 Output to pin IEXC1
}

#if defined(__AVR__)
  char buffer[64];
  char str_val[12];
  char str_Tc[12];
  char str_Rt[12];
#endif

void loop(){  
  double Rt = 0;
  double T, Tc, Tf = 0;
  
  if(ads1148.isDataReady()){
  
    uint16_t val = ads1148.convertToValue(ads1148.readADC_Array());

    if(val >= 32760) {
      Serial.println("Sensor not connected");
    }
    else if (val <= 7) {
      Serial.println("Sensor shorted");
    } else {
      Rt = R1 / ((32767.0 / val) - 1.0);  // Rntc = Raux / ((Vcc/Vm) - 1) -> Vm = voltaje del NTC
      T = 1/(1/To + log(Rt/Ro)/Beta);  // Temperature in Kelvin
      Tc = T - 273.15;                 // Celsius
      Tf = Tc * 9 / 5 + 32;            // Fahrenheit
      #if defined(__AVR__)
        //dtostrf(val, 5, 0, str_val);
        dtostrf(Tc, 6, 2, str_Tc);
        dtostrf(Rt, 6, 1, str_Rt);
        sprintf (buffer, "Sensor %d, %s C, %s ohm\n", val, str_Tc, str_Rt);
        Serial.print(buffer);
      #elif defined(ESP32)
        Serial.printf("Sensor %5.0f, %3.2f C, %5.1f ohm\n", val, Tc, Rt);
      #endif
    }
  }
}
