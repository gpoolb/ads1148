#include <ADS1148.h>

/**
* thermocouple Example for using the Arduino Library for the Texas Instruments ADS1148
* Tested with arduino nano and NodeMCU ESP32-WROOM-32D
*/

// Defina el typo de termopar que utilice
#define THERMOCOUPLE_TYPE_K
#include "Thermocouple.h" * 

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

  ads1148.setReadingThermocouple(ANALOG_CHANNEL_0); 
}

double TR = 0.0; // Temperatura remota (valor leido del termopar) en °C
double TRP = 0.0; // Temperatura remota (valor leido del termopar) en °C
double TAMB = 0.0; // Temperatura interna (cold Junction) en °C
char action = 0; // Indica la accion a realizar
#if defined(__AVR__)
  char buffer[64];
  char str_TR[12];
#endif

void loop(){
    
  
  if(ads1148.isDataReady()){
      // Para leer el valor del adc, se requiere esperar la senal DRDY despues de cada cambio.
    switch (action) {
      case 0:     
        TR = convertInputThermocoupleToTemperature(ads1148.readInputVoltageIn_mV());
        ads1148.setReadingInternalTemperature(); // Se ajusta la entrada para la siguiente lectura 
        break;
      case 1: 
        TAMB = ads1148.readInternalTemperature(); // lee el valor de la punta fria (temperatura ambiente)
        TR = TR + TAMB; // se compensa la temperatura de la punta fria
        #if defined(__AVR__)
          dtostrf(TR, 6, 2, str_TR);
          sprintf (buffer, "Input Value Thermocuple: %s °C\n", str_TR);
          Serial.print(buffer);
        #elif defined(ESP32)
          Serial.printf("Input Value Thermocuple:   %4.2f °C \n", TR);
        #endif
        ads1148.setReadingThermocouple(ANALOG_CHANNEL_0); // se ajusta la entrada para la siguiente lectura
        break;
    }
    action++;
    action &= 1;
  }
}
