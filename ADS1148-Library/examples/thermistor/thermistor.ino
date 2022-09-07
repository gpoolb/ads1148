#include <ADS1120.h>

/**
* Thermistor Example for using the Arduino Library for the Texas Instruments ADS11120
*/

// Definition of the pins to be used
#define CS      17
#define DRDY    16

// Creating an ADS1120 object
ADS1120 ads1120;

double R1 = 10000.0;    // Voltage divider resistor value
double Beta = 3977.0;   // Beta value
double To = 298.15;     // Temperature in Kelvin for 25 degree Celsius
double Ro = 10000.0;    // Resistance of Thermistor at 25 degree Celsius

void setup(){
    Serial.begin(115200);

    // Initialize the ADS1120
    ads1120.begin(CS, DRDY);
    ads1120.setGain(1);               // 1 (default)
    ads1120.setDataRate(0x00);        // 20 SPS
    ads1120.setOpMode(0x00);          // Normal mode (default)
    ads1120.setConversionMode(0x01);  // Single shot (default)
    // Thermistor and reference must be powered by same voltage
    ads1120.setVoltageRef(1);         // External REFP0 and REFN0
    ads1120.setTemperatureMode(0);    // Disables temperature sensor (default)
}

void loop(){  
  double Rt = 0;
  double T, Tc, Tf = 0;
  
  ads1120.setMultiplexer(0x08); // AIN0 and AVSS
  uint16_t val = ads1120.readADC_Single();
  
  if(val >= 32760) {
    Serial.println("Sensor not connected");
  }
  else if (val <= 7) {
    Serial.println("Sensor shorted");
  } else {
    Rt = R1 / ((32767 / val) - 1);
    T = 1/(1/To + log(Rt/Ro)/Beta);  // Temperature in Kelvin
    Tc = T - 273.15;                 // Celsius
    Tf = Tc * 9 / 5 + 32;            // Fahrenheit
    Serial.printf("Sensor %5.0f %3.2fF %5.1fohm\n", val, Tf, Rt);
  }
  
  delay(500);
}
