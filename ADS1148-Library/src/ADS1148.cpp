#include "ADS1148.h"
#include <Arduino.h>



#if defined(__AVR__)
/**
 * Constructor of the class
 * @param io_pin_cs a byte indicating the pin to be use as the chip select pin (CS)
 */
ADS1148::ADS1148(uint8_t cs_pin, uint8_t drdy_pin, uint8_t start_pin, uint8_t reset_pin) {

  // Set pins up
  ADS1148_CS_PIN = cs_pin;
  ADS1148_DRDY_PIN = drdy_pin;
  ADS1148_START_PIN = start_pin;
  ADS1148_RESET_PIN = reset_pin;
  
  // Configure start as an output
  pinMode(ADS1148_START_PIN, OUTPUT);

  // Enable the device by setting the START pin high;
  digitalWrite(ADS1148_START_PIN, HIGH); // Set START High

  // Configure chip select as an output
  pinMode(ADS1148_CS_PIN, OUTPUT);

  // Configure DRDY as as input (mfg wants us to use interrupts)
  pinMode(ADS1148_DRDY_PIN, INPUT);
  
  // Disable the device by setting the CS pin high;
  digitalWrite(ADS1148_CS_PIN, HIGH); // Set CS HIGH

  // Configure reset as an output
  pinMode(ADS1148_RESET_PIN, OUTPUT);
  
  // Enable the device by setting the RESET pin high;
  digitalWrite(ADS1148_RESET_PIN, HIGH);  // Set reset HIGH
 	
}
						///< This method initialize the SPI port and the config register        
#elif defined(ESP32)
/**
 * Constructor of the class
 * @param io_pin_cs a byte indicating the pin to be use as the chip select pin (CS)
 */
ADS1148::ADS1148(uint8_t cs_pin, uint8_t drdy_pin, uint8_t start_pin, uint8_t reset_pin, SPIClass *spi) {
  // Set pins up
  ADS1148_CS_PIN = cs_pin;
  ADS1148_DRDY_PIN = drdy_pin;
  ADS1148_START_PIN = start_pin;
  ADS1148_RESET_PIN = reset_pin;
  pSpi = spi;
  
  // Configure start as an output
  pinMode(ADS1148_START_PIN, OUTPUT);

  // Enable the device by setting the START pin high;
  digitalWrite(ADS1148_START_PIN, HIGH); // Set START High

  // Configure chip select as an output
  pinMode(ADS1148_CS_PIN, OUTPUT);

  // Configure DRDY as as input (mfg wants us to use interrupts)
  pinMode(ADS1148_DRDY_PIN, INPUT);
  
  // Disable the device by setting the CS pin high;
  digitalWrite(ADS1148_CS_PIN, HIGH); // Set CS HIGH

  // Configure reset as an output
  pinMode(ADS1148_RESET_PIN, OUTPUT);
  
  // Enable the device by setting the RESET pin high;
  digitalWrite(ADS1148_RESET_PIN, HIGH);  // Set reset HIGH
  
}       
#endif


#if defined(__AVR__)

void ADS1148::writeRegister(uint8_t address, uint8_t value) {
  SPI.beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE1));
  digitalWrite(ADS1148_CS_PIN, LOW);
  delayMicroseconds(1);  // tcssc
  SPI.transfer(CMD_WREG|address);
  delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  SPI.transfer(0x00);  // Sólo se envía 1 byte.
  delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  SPI.transfer(value);
  delayMicroseconds(2);  // tsccs = 5 * tclk = 5 * (1 / 4096000)
  digitalWrite(ADS1148_CS_PIN, HIGH);
  delayMicroseconds(2);  // tcspw
  SPI.endTransaction();
}

uint8_t ADS1148::readRegister(uint8_t address) {
  SPI.beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE1));
  digitalWrite(ADS1148_CS_PIN,LOW);
  delayMicroseconds(1);  // tcssc
  SPI.transfer(CMD_RREG|address); 
  delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  SPI.transfer(0x00); // Sólo se recibe 1 byte
  delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  uint8_t data = SPI.transfer(SPI_MASTER_DUMMY);
  delayMicroseconds(2);  // tsccs = 5 * tclk = 5 * (1 / 4096000)
  digitalWrite(ADS1148_CS_PIN,HIGH);
  delayMicroseconds(2);  // tcspw
  SPI.endTransaction();
  return data;
}
/**
 * This method initialize the SPI port and the config register
 */

bool ADS1148::begin() {
  
  bool flag = false;
  uint8_t registerValues [] = {0, 0, 0, 0};
  uint8_t i = 0;
  
  // Power up - Delay for a minimum of 16 ms to allow power supplies 
  // to settle and power-on reset to complete
  delay(16);
  
  // Configure start as an output (see constructor)

  // Configure the SPI interface (CPOL=0, CPHA=1)
  SPI.begin();
  //SPI.setDataMode(SPI_MODE1);
  SPI.beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE1));

  // Configure chip select as an output (see constructor)
  // Configure DRDY as as input (mfg wants us to use interrupts) (see constructor)

  digitalWrite(ADS1148_CS_PIN, LOW); // Set CS Low
  delayMicroseconds(1); // Wait a minimum of tcssc

  // Send reset command
  SPI.transfer(CMD_RESET);  // reset(); 
  delayMicroseconds(2);
  
  // Delay for a minimum of 0.6 ms;
  delay(1);

  //Send SDATAC command
  SPI.transfer(CMD_SDATAC);  // stopContinuosMode(); 
  delayMicroseconds(2);
  
  // Sanity check read back (optional)
  // write to wreg command (40h, 03h, 01h, 00h, 03h and 42h) values.

  SPI.transfer(0x40);           /* 40h indica que se envia el comando WREG y se va a modificar a partir del registro MUX0 (00h) -> (CMD_WREG|address) */
  delayMicroseconds(2);			// tstd = 5 * tclk = 5 * (1 / 4096000)
  
  SPI.transfer(0x03);			/* 03h parte del comando WREG que indica que se van a modificar los valores 
								*  de tres registros contiguos a MUX0: VBIAS(01h), MUX1(02h) y SYS0 (03h)*/
  delayMicroseconds(2);			// tstd = 5 * tclk = 5 * (1 / 4096000)

  SPI.transfer(0x01);  			/* 01h valor asignado al registro MUX0 que indica lo siguiente:
								 *	setBurnoutCurrentSources(BURNOUT_SOURCE_OFF); 		// default values
								 *	setADCPositiveInput(POSITIVE_INPUT_AIN0); 			// default values
								 *	setADCNegativeInput(NEGATIVE_INPUT_AIN1); 			// default values */
  delayMicroseconds(2);			// tstd = 5 * tclk = 5 * (1 / 4096000)
								
  SPI.transfer(0x00); 			/* 00h valor asignado al registro VBIAS que indica lo siguiente:
								 *	setVBIASEnableTo(VBIAS_APPLIED_OFF_ALL_PIN); 		// default values */
  delayMicroseconds(2);			// tstd = 5 * tclk = 5 * (1 / 4096000)

  SPI.transfer(0x03); 			/* 03h valor asignado al registro MUX1 que indica lo siguiente:
									setInternalRef(INTERNAL_REF_ALWAYS_OFF);			// default values
									setVoltageRef(REF_INPUT_IN_REFP0_REFN0); 			// default values
									setOpMode(TEMP_MEASUREMENT); 						// Medición de temperatura */
  delayMicroseconds(2);			// tstd = 5 * tclk = 5 * (1 / 4096000)

  SPI.transfer(0x42); 			/* 42h valor asignado al registro SYS0 que indica lo siguiente:
								 *	setGain(GAIN_PGA_TO_16); 							// Se elige Ganancia = 16
								 *	setDataRate(DATA_RATE_TO_20_SPS); 					// Se elije data rate de 20 muestras por segundo. */
  delayMicroseconds(2);			// tstd = 5 * tclk = 5 * (1 / 4096000)

  // As an optional sanity check,
  // read back all configuration registers with the RREG command (four bytes from 20h, 03h);
  SPI.transfer(0x20); 			/* 20h indica que se envia el comando RREG y se va a leer datos a partir del registro MUX0 (00h) -> (CMD_WREG|address) */ 
  delayMicroseconds(2);  		// tstd = 5 * tclk = 5 * (1 / 4096000)
  
  SPI.transfer(0x03); 			/* 03h parte del comando WREG que indica que se van a leer los valores 
								*  de tres registros contiguos a MUX0: VBIAS(01h), MUX1(02h) y SYS0 (03h)*/
  delayMicroseconds(2);  		// tstd = 5 * tclk = 5 * (1 / 4096000)
  
  while (i < 4){
    registerValues [i] = SPI.transfer(SPI_MASTER_DUMMY);  // Se recibe el dato de la primera dirección solicitada (00h) en el puerto SPI
	  delayMicroseconds(2);  		// tsccs = 5 * tclk = 5 * (1 / 4096000)
    Serial.println(registerValues [i]);
	  i++;
  }
    
  if (registerValues [0] == 0x01 && \
		registerValues [1] == 0x00 && \
		registerValues [2] == 0x03 && \
		registerValues [3] == 0x42)
	flag = true;
  
  // Send the SYNC command (04h) to start the ADC conversion;
//  startSync();
  // Delay for a minimum of tSCCS;
  delayMicroseconds(2); // tsccs = 5 * tclk = 5 * (1 / 4096000)
  //Clear CS to high (resets the serial interface);
  digitalWrite(ADS1148_CS_PIN, HIGH); // Clear CS to high
  SPI.endTransaction();
  
  // Por conveniencia se deja encendido y seleccionado el voltaje de referencia interna:
  setInternalRef(INTERNAL_REF_ALWAYS_ON);
  setVoltageRef(REF_INPUT_INTERNAL);
  
  // Se ajustan valores por default
  setGain(GAIN_PGA_TO_1);  			 // default
  setDataRate(DATA_RATE_TO_5_SPS);   // default
  stopContinuousMode();              // default
  
  return flag;
}

byte * ADS1148::readADC_Array(bool isContinuous) {
  SPI.beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE1));
  digitalWrite(ADS1148_CS_PIN, LOW); // Take CS low
  delayMicroseconds(1); // Minimum of td(CSSC)
  
  if (!isContinuous){
	SPI.transfer(CMD_RDATA);
	delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  }
  
  static byte dataarray[2];
  for (int x = 0; x < 2 ; x++)
  {
    dataarray[x] = SPI.transfer(CMD_NOP);
	delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  }
  
  digitalWrite(ADS1148_CS_PIN, HIGH);
  delayMicroseconds(2);  // tcspw
  SPI.endTransaction();
  return dataarray;
}

void ADS1148::sendCommand(uint8_t command) {
  
  SPI.beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE1));
  digitalWrite(ADS1148_CS_PIN, LOW);
  delayMicroseconds(1);  // tcssc
  SPI.transfer(command);
  delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  digitalWrite(ADS1148_CS_PIN, HIGH);
  delayMicroseconds(2);  // tcspw
  SPI.endTransaction();
}


#elif defined(ESP32)
/**
 *  void ADS1148::writeRegister(uint8_t address, uint8_t value)
 *  Este metodo se encarga de escibir un valor en el registro 
 *  que se proporciona en sus parametros:
 *  address: direccion al que se desea modificar (0 ~ 14) ver defs en ADS1146 Ln. 31 hasta Ln. 45
 *  value: valor que se desea escribir en la direccion del registro 
 *  apuntado por el parametro anterior (address).
 */

void ADS1148::writeRegister(uint8_t address, uint8_t value) {
  pSpi->beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE1));
  digitalWrite(ADS1148_CS_PIN, LOW);
  delayMicroseconds(1);  // tcssc
  pSpi->transfer(CMD_WREG|address);
  delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  pSpi->transfer(0x00);  // Sólo se envía 1 byte.
  delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  pSpi->transfer(value);
  delayMicroseconds(2);  // tsccs = 5 * tclk = 5 * (1 / 4096000)
  digitalWrite(ADS1148_CS_PIN, HIGH);
  delayMicroseconds(2);  // tcspw
  pSpi->endTransaction();
}
/**
 *  uint8_t ADS1148::readRegister(uint8_t address)
 *  Este metodo se encarga de leer un valor en el registro 
 *  que se proporciona en el parametro:
 *  address: direccion al que se desea leer (0 ~ 14)  ver defs en ADS1146 Ln. 31 hasta Ln. 45
 *  
 *  Esta funcion devuelve el valor almacenado en la direccion del registro
 *  apuntado por el parametro anterior (address).
 */
uint8_t ADS1148::readRegister(uint8_t address) {
  pSpi->beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE1));
  digitalWrite(ADS1148_CS_PIN,LOW);
  delayMicroseconds(1);  // tcssc
  pSpi->transfer(CMD_RREG|address); 
  delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  pSpi->transfer(0x00); // Sólo se recibe 1 byte
  delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  uint8_t data = pSpi->transfer(SPI_MASTER_DUMMY);
  delayMicroseconds(2);  // tsccs = 5 * tclk = 5 * (1 / 4096000)
  digitalWrite(ADS1148_CS_PIN,HIGH);
  delayMicroseconds(2);  // tcspw
  pSpi->endTransaction();
  return data;
}
/**
 * bool ADS1148::begin()
 * Este metodo inicializa el puerto SPI y configura los registros
 * 
 * Devuelve:
 * true si la configuracion fue exitosa
 * false si la configuracion fue fallida
 */

bool ADS1148::begin() {
  
  bool flag = false;
  uint8_t registerValues [] = {0, 0, 0, 0};
  uint8_t i = 0;
  
  // Power up - Delay for a minimum of 16 ms to allow power supplies 
  // to settle and power-on reset to complete
  delay(16);
  
  // Configure start as an output (see constructor)

  // Configure the SPI interface (CPOL=0, CPHA=1)
  pSpi->begin();
  //pSpi->setDataMode(SPI_MODE1);
  pSpi->beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE1));

  // Configure chip select as an output (see constructor)
  // Configure DRDY as as input (mfg wants us to use interrupts) (see constructor)

  digitalWrite(ADS1148_CS_PIN, LOW); // Set CS Low
  delayMicroseconds(1); // Wait a minimum of tcssc

  // Send reset command
  pSpi->transfer(CMD_RESET);  // reset(); 
  delayMicroseconds(2);
  
  // Delay for a minimum of 0.6 ms;
  delay(1);

  //Send SDATAC command
  pSpi->transfer(CMD_SDATAC);  // stopContinuosMode(); 
  delayMicroseconds(2);
  
  // Sanity check read back (optional)
  // write to wreg command (40h, 03h, 01h, 00h, 03h and 42h) values.

  pSpi->transfer(0x40);           /* 40h indica que se envia el comando WREG y se va a modificar a partir del registro MUX0 (00h) -> (CMD_WREG|address) */
  delayMicroseconds(2);			// tstd = 5 * tclk = 5 * (1 / 4096000)
  
  pSpi->transfer(0x03);			/* 03h parte del comando WREG que indica que se van a modificar los valores 
								*  de tres registros contiguos a MUX0: VBIAS(01h), MUX1(02h) y SYS0 (03h)*/
  delayMicroseconds(2);			// tstd = 5 * tclk = 5 * (1 / 4096000)

  pSpi->transfer(0x01);  			/* 01h valor asignado al registro MUX0 que indica lo siguiente:
								 *	setBurnoutCurrentSources(BURNOUT_SOURCE_OFF); 		// default values
								 *	setADCPositiveInput(POSITIVE_INPUT_AIN0); 			// default values
								 *	setADCNegativeInput(NEGATIVE_INPUT_AIN1); 			// default values */
  delayMicroseconds(2);			// tstd = 5 * tclk = 5 * (1 / 4096000)
								
  pSpi->transfer(0x00); 			/* 00h valor asignado al registro VBIAS que indica lo siguiente:
								 *	setVBIASEnableTo(VBIAS_APPLIED_OFF_ALL_PIN); 		// default values */
  delayMicroseconds(2);			// tstd = 5 * tclk = 5 * (1 / 4096000)

  pSpi->transfer(0x03); 			/* 03h valor asignado al registro MUX1 que indica lo siguiente:
									setInternalRef(INTERNAL_REF_ALWAYS_OFF);			// default values
									setVoltageRef(REF_INPUT_IN_REFP0_REFN0); 			// default values
									setOpMode(TEMP_MEASUREMENT); 						// Medición de temperatura */
  delayMicroseconds(2);			// tstd = 5 * tclk = 5 * (1 / 4096000)

  pSpi->transfer(0x42); 			/* 42h valor asignado al registro SYS0 que indica lo siguiente:
								 *	setGain(GAIN_PGA_TO_16); 							// Se elige Ganancia = 16
								 *	setDataRate(DATA_RATE_TO_20_SPS); 					// Se elije data rate de 20 muestras por segundo. */
  delayMicroseconds(2);			// tstd = 5 * tclk = 5 * (1 / 4096000)

  // As an optional sanity check,
  // read back all configuration registers with the RREG command (four bytes from 20h, 03h);
  pSpi->transfer(0x20); 			/* 20h indica que se envia el comando RREG y se va a leer datos a partir del registro MUX0 (00h) -> (CMD_WREG|address) */ 
  delayMicroseconds(2);  		// tstd = 5 * tclk = 5 * (1 / 4096000)
  
  pSpi->transfer(0x03); 			/* 03h parte del comando WREG que indica que se van a leer los valores 
								*  de tres registros contiguos a MUX0: VBIAS(01h), MUX1(02h) y SYS0 (03h)*/
  delayMicroseconds(2);  		// tstd = 5 * tclk = 5 * (1 / 4096000)
  
  while (i < 4){
    registerValues [i] = pSpi->transfer(SPI_MASTER_DUMMY);  // Se recibe el dato de la primera dirección solicitada (00h) en el puerto SPI
	delayMicroseconds(2);  		// tsccs = 5 * tclk = 5 * (1 / 4096000)
	i++;
  }
    
  if (registerValues [0] == 0x01 && \
		registerValues [1] == 0x00 && \
		registerValues [2] == 0x03 && \
		registerValues [3] == 0x42)
	flag = true;
  
  // Send the SYNC command (04h) to start the ADC conversion;
//  startSync();
  // Delay for a minimum of tSCCS;
  delayMicroseconds(2); // tsccs = 5 * tclk = 5 * (1 / 4096000)
  //Clear CS to high (resets the serial interface);
  digitalWrite(ADS1148_CS_PIN, HIGH); // Clear CS to high
  pSpi->endTransaction();
  
  // Por conveniencia se deja encendido y seleccionado el voltaje de referencia interna:
  setInternalRef(INTERNAL_REF_ALWAYS_ON);
  setVoltageRef(REF_INPUT_INTERNAL);
  
  // Se ajustan valores por default
  setGain(GAIN_PGA_TO_1);  			 // default
  setDataRate(DATA_RATE_TO_5_SPS);   // default
  stopContinuousMode();              // default
  
  return flag;
}
/*  byte * ADS1148::readADC_Array(bool isContinuous)
*   
*   Se requiere la configuracion previa donde se seleccione la entrada para leer.
*   Este metodo se encarga de digitalizar la entrada seleccionada anteriormente.
*   
*   El parámetro bool isContinuous es:
*   true si está usando previamente el método void ADS1148::runContinuousMode()
*   false si está usando previamente el método void ADS1148::stopContinuousMode()
*   ... en ambos casos se requiere utilizar el método bool ADS1148::isDataReady()
*   para obtener la lectura del adc.
*
*   Devuelve el valor digitalizado del adc.
*
*   ver implementacion en thermistor.ino
*/
byte * ADS1148::readADC_Array(bool isContinuous) {
  pSpi->beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE1));
  digitalWrite(ADS1148_CS_PIN, LOW); // Take CS low
  delayMicroseconds(1); // Minimum of td(CSSC)
  
  if (!isContinuous){
	pSpi->transfer(CMD_RDATA);
	delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  }
  
  static byte dataarray[2];
  for (int x = 0; x < 2 ; x++)
  {
    dataarray[x] = pSpi->transfer(CMD_NOP);
	delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  }
  
  digitalWrite(ADS1148_CS_PIN, HIGH);
  delayMicroseconds(2);  // tcspw
  pSpi->endTransaction();
  return dataarray;
}
/*
 * void ADS1148::sendCommand(uint8_t command)
 * Esta funcion envia al ads1148 los comandos que el fabricante 
 * establecio para este dispositivo consulte el datasheet para 
 * mayor detalle. 
 * Las definiciones que usa este metodo lo puede consultar
 * en el archivo ADS1148.h Ln. 10 hasta Ln. 28
 */
void ADS1148::sendCommand(uint8_t command) {
  
  pSpi->beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE1));
  digitalWrite(ADS1148_CS_PIN, LOW);
  delayMicroseconds(1);  // tcssc
  pSpi->transfer(command);
  delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  digitalWrite(ADS1148_CS_PIN, HIGH);
  delayMicroseconds(2);  // tcspw
  pSpi->endTransaction();
}


#endif
/*
 * bool ADS1148::isDataReady()
 * Esta funcion devuelve el estado del pin !DRDY
 *
 * La funcion devuelve:
 * true (si el pin esta en bajo) e indica que la conversion de adc esta completada
 * false (si el pin esta en alto) e indica que el convertidor aun no termina con la operacion
 *
 * Esta funcion es obligatoria utilizarla para capturar el valor mas reciente de la conversion
 * 
 * Vea la implementacion en los ejemplos.
 */
bool ADS1148::isDataReady() {
  if (digitalRead(ADS1148_DRDY_PIN) == HIGH) {
    return false;
  }
  return true;
}
/*
 * void ADS1148::setReadingInternalTemperature()
 * Esta funcion configura al ads1148 para que el adc pueda leer el sensor de temperatura interno.
 * 
 * vea la implementacion del metodo en el ejemplo inttemp.ino
 */
void ADS1148::setReadingInternalTemperature(){
	
  stopContinuousMode();
  setOpMode(TEMP_MEASUREMENT); 	// Medición de temperatura interna */
  startSync();
}
/*
 * void ADS1148::setReadingThermocouple(uint8_t channel)
 * Esta funcion configura al ads1148 para que el adc pueda leer un termopar.
 * 
 * El parametro requerido channel especifica el canal de entrada que desea leer,
 * las definiciones para este metodo las encuentra en ADS1146 Ln. 234 hasta Ln. 237
 * 
 * vea la implementacion del metodo en el ejemplo thermocouple.ino
 */
void ADS1148::setReadingThermocouple(uint8_t channel){
	
  stopContinuousMode();
  setOpMode(NORMAL_OPERATION); 	   // Medición de canales externos */
  setGain(GAIN_PGA_TO_32);         // default values
  setDataRate(DATA_RATE_TO_5_SPS); // default values

  switch (channel) {
    case ANALOG_CHANNEL_0: 
      setADCPositiveInput(POSITIVE_INPUT_AIN0); // default values
      setADCNegativeInput(NEGATIVE_INPUT_AIN1); // default values
      break;
    case ANALOG_CHANNEL_1: 
      setADCPositiveInput(POSITIVE_INPUT_AIN2); 
      setADCNegativeInput(NEGATIVE_INPUT_AIN3); 
      break;
    case ANALOG_CHANNEL_2: 
      setADCPositiveInput(POSITIVE_INPUT_AIN4); 
      setADCNegativeInput(NEGATIVE_INPUT_AIN5); 
      break;
    case ANALOG_CHANNEL_3: 
      setADCPositiveInput(POSITIVE_INPUT_AIN6); 
      setADCNegativeInput(NEGATIVE_INPUT_AIN7); 
      break;
  }
  startSync();
}

/*  double ADS1148::readInternalTemperature(bool isContinuous)
 *   
 *   Requiere el uso previo del método void ADS1148::setReadingInternalTemperature()
 *   que se encarga de configurar el ADS1148 para leer el sensor interno de temperatura
 *   
 *   El parámetro bool isContinuous es:
 *   true si está usando previamente el método void ADS1148::runContinuousMode()
 *   false si está usando previamente el método void ADS1148::stopContinuousMode()
 *   ... en ambos casos se requiere utilizar el método bool ADS1148::isDataReady()
 *   para la lectura del sensor de temperatura.
 *
 *   Devuelve el valor de la temperatura interna en grados centígrados.
 * 
 * vea la implementacion del metodo en el ejemplo inttemp.ino
 */
double ADS1148::readInternalTemperature(bool isContinuous){
	
  double adcValue = 0;
	
  adcValue = (double)convertToValue(readADC_Array(isContinuous));
	
  /* (+-)2.048V -> 65535 full scale --> 
  *  -2.048V -> 0xFFFF ... 0V -> 0x0000 ... 2.048V -> 2^15 = 32768
  *  Ambient Temperature Monitor temperature coefficient of 405 μV/°C.
  *  0.108V ->  0°C -> 1726
  *  0.118V -> 25°C -> 1888
  *  0.132V -> 60°C -> 2115
  *  
  *  0°C ~ 60°C => 0.108 ~ 0.132 Volts
  *  ValueADC => 1726 ~ 2115
  *  m = ((60 - 0) / (2115 - 1726)) = (60 / 389) = 0.1542;
  *  y = mx + b de donde y = temperatura, m = pendiente de la recta, b = valor de y cuando x = 0 (offset)
  *  0 = ((60 / 389) * 1726) + b , por lo que: b = -266 + (86 / 389) = -266.2210;
  *  
  *  Temp = ((60 * ValueADC) / 389) - (266 + (86 / 389)) = (((60 * ValueADC) - 86) / 389) - 266 =(0.1542 * ValueADC) - 266.2210
  */

  return ((double) (((adcValue * 60.0) - 86.0) / 389.0) - 266.0);
  
}
/*  double ADS1148::readInputVoltage(bool isContinuous)
 *   
 *   Se requiere la configuracion previa donde se seleccione la entrada para leer.
 *   Este metodo se encarga de digitalizar la entrada seleccionada anteriormente
 *   y devuelve el valor leido en volts
 *   
 *   El parámetro bool isContinuous es:
 *   true si está usando previamente el método void ADS1148::runContinuousMode()
 *   false si está usando previamente el método void ADS1148::stopContinuousMode()
 *   ... en ambos casos se requiere utilizar el método bool ADS1148::isDataReady()
 *   para la lectura del voltaje.
 *
 *   Devuelve el valor del voltage en volts.
 * 
 * vea la implementacion del metodo en el ejemplo singleended.ino
 */
double ADS1148::readInputVoltage(bool isContinuous){

  uint16_t adcValue = 0;
	
  adcValue = (double)convertToValue(readADC_Array(isContinuous));
  /* (+-)2.048V -> 65535 full scale --> 
  *  -2.048V -> 0xFFFF ... 0V -> 0x0000 ... 2.048V -> 2^15 = 32768
  */
  
  if (adcValue > 0x7FFF) {  // Valores negativos
	adcValue = (~(adcValue - 1));
	return ((double) (((double)adcValue * -2.048) / 32768.0));
  }
	
  return ((double) (((double)adcValue * 2.048) / 32768.0));
  //return (double) adcValue;
}

/*  double ADS1148::readInputVoltageIn_mV(bool isContinuous)
 *   
 *   Se requiere la configuracion previa donde se seleccione la entrada para leer.
 *   Este metodo se encarga de digitalizar la entrada seleccionada anteriormente
 *   y devuelve el valor leido en milivolts
 *   
 *   El parámetro bool isContinuous es:
 *   true si está usando previamente el método void ADS1148::runContinuousMode()
 *   false si está usando previamente el método void ADS1148::stopContinuousMode()
 *   ... en ambos casos se requiere utilizar el método bool ADS1148::isDataReady()
 *   para la lectura del voltaje.
 *
 *   Devuelve el valor del voltage en milivoltsvolts.
 */
double ADS1148::readInputVoltageIn_mV(bool isContinuous){

  uint16_t adcValue = 0;
	
  /* (+-)2.048V -> 65535 negative full scale --> 32767 positive full scale
  *  -2.048V -> 0xFFFF ... 0V -> 0x0000 ... 2.048V -> 2^15 = 0x7FFF = 32768
  */
  adcValue = (double)convertToValue(readADC_Array(isContinuous));
  
  if (adcValue > 0x7FFF) {  // Valores negativos
	adcValue = (~(adcValue - 1));
	return ((double) (((double)adcValue * (-1.0)) / 512.0));
  }
	
  return ((double) (((double)adcValue) / 512.0));
  //return (double) adcValue;
}
/* 
* uint16_t ADS1148::convertToValue(byte * data)
*  Funcion de apoyo para convertir un array de 2 bytes
* a un numero de 16 bits
*/
uint16_t ADS1148::convertToValue(byte * data) {
  return (data[0])<<8 | (data[1]); //Moving MSB and LSB to 16 bit
}
/**
 *  void ADS1148::pinModeGPIO(uint8_t pin, uint8_t mode)
 *  Este metodo se encarga de configurar en el registro del ADS1148
 *  el modo en que se desea los pines GPIO
 *  y los parametros requeridos son:
 *  pin: el pin que se desea configurar (ver defs en ads1148.h Ln 224)
 *  mode: modo que se desea configurar ya sea como entrada (INPUT) o salida digital (OUTPUT) 
 * 
 */
void ADS1148::pinModeGPIO(uint8_t pin, uint8_t mode){
	
  uint8_t mask = pin;
  
  if (mode == OUTPUT)
	mode = 0;
  else
	mode = pin;
	
  /* Adjust GPIOCFG register for enable pin*/
  writeRegisterMasked(pin, mask, CONFIG_GPIOCFG_ADDRESS);

  /* Adjust GPIODIR register for enable pin*/
  writeRegisterMasked(mode, mask, CONFIG_GPIODIR_ADDRESS);

}

/**
 * Esta funcion requiere de la configuracion previa del pin como
 * entrada o salida. 
 * 
 * void ADS1148::digitalWriteGPIO(uint8_t pin, uint8_t value)
 *  Este metodo se encarga de configurar en el registro del ADS1148
 *  la salida que se desea los pines GPIO del ADS1148 (HIGH or LOW)
 *  y los parametros requeridos son:
 *  pin: el pin que se desea configurar (ver defs en ads1148.h Ln 224 ~ 231)
 *  value: modo que se desea configurar HIGH or LOW
 * 
 */
void ADS1148::digitalWriteGPIO(uint8_t pin, uint8_t value){
	
  uint8_t mask = pin;
	
  if (value == HIGH)
	value = pin;
  else
	value = 0;

  /* Adjust GPIODAT register for enable pin*/
  writeRegisterMasked(value, mask, CONFIG_GPIODAT_ADDRESS);
}

/**
 *  Esta funcion requiere de la configuracion previa del pin como
 *  entrada o salida. 
 * 
 *  bool ADS1148::digitalReadGPIO(uint8_t pin)
 *  Este metodo se encarga de configurar en el registro del ADS1148
 *  la captura del valor de entrada en el pin GPIO del ADS1148 (HIGH or LOW)
 *  y los parametros requeridos son:
 *  pin: el pin que se desea leer (ver defs en ads1148.h Ln 224 ~ 231)
 * 
 * Devuelve el valor del estado de entrada del pin (HIGH or LOW)
 * 
 */
bool ADS1148::digitalReadGPIO(uint8_t pin){
	
  uint8_t value;
	

  /* Read GPIODAT register*/
  value = readRegister(CONFIG_GPIODAT_ADDRESS);
  
  return (value & pin);
	
}

/*   Write the value to a register using the mask to leave the rest of the
*    register untouched. This does not shift the value, so it shoudl be provided
*    shifted to the appropriate positions.
*/
void ADS1148::writeRegisterMasked(uint8_t value, uint8_t mask, uint8_t address) {

  // Read what's in the register now
  uint8_t register_contents = readRegister(address);

  // Flip the mask so that it's zeros where we'll put data and zero out that
  // part of the register's contents
  register_contents = register_contents & ~mask;

  // OR in the value to be written
  register_contents = register_contents | value;

  // Write it back out
  writeRegister(address, register_contents);
}

/*
* Consultar valores en ads1148.h línea 53
* para valores de Burn-out Detect Current source
*/
void ADS1148::setBurnoutCurrentSources(uint8_t value) {
  /* Adjust burn-out current sources */
  writeRegisterMasked(value, REG_MASK_BCS, CONFIG_MUX0_ADDRESS);
}

/*
* Consultar valores en ads1148.h línea 59
* para valores de Multiplexer Selection - ADC Positive Input
*/
void ADS1148::setADCPositiveInput(uint8_t value){
  /* Adjust ADC Positive Input */
  writeRegisterMasked(value, REG_MASK_MUX_SP, CONFIG_MUX0_ADDRESS);	
}

/*
* Consultar valores en ads1148.h línea 69
* para valores de Multiplexer Selection - ADC Negative Input
*/
void ADS1148::setADCNegativeInput(uint8_t value){
  /* Adjust ADC Negative Input */
  writeRegisterMasked(value, REG_MASK_MUX_SN, CONFIG_MUX0_ADDRESS);	
}

/*
* Consultar valores en ads1148.h línea 82
* para valores de VBIAS—Bias Voltage Register output selection
*/
void ADS1148::setVBIASEnableTo(uint8_t value){
  /* Adjust VBIAS—Bias Voltage */
  writeRegisterMasked(value, REG_MASK_VBIAS, CONFIG_VBIAS_ADDRESS);	
}

/*
* Consultar valores en ads1148.h línea 100
* para valores de Internal Reference Control
*/
void ADS1148::setInternalRef(uint8_t value){
  /* Adjust Internal Reference Control */
  writeRegisterMasked(value, REG_MASK_VREFCON, CONFIG_MUX1_ADDRESS);	
}

/*
* Consultar valores en ads1148.h línea 105
* para valores de Reference Control Select
*/
void ADS1148::setVoltageRef(uint8_t value){
  /* Adjust Reference Control Select*/
  writeRegisterMasked(value, REG_MASK_REFSELT, CONFIG_MUX1_ADDRESS);	
}

/*
* Consultar valores en ads1148.h línea 111
* para valores de System Monitor Control
*/
void ADS1148::setOpMode(uint8_t value) {
	/* Adjust System Monitor Control*/
  writeRegisterMasked(value, REG_MASK_MUXCAL, CONFIG_MUX1_ADDRESS);
}

/*
* Consultar valores en ads1148.h línea 126
* para valores de Gain Setting for PGA
*/
void ADS1148::setGain(uint8_t value) {
  /* Sets ADC gain. */
  writeRegisterMasked(value, REG_MASK_PGA, CONFIG_SYS0_ADDRESS);
}

/*
* Consultar valores en ads1148.h línea 136
* para valores de Data Output Rate
*/
void ADS1148::setDataRate(uint8_t value) {
  /* Sets the data rate for the ADC. */
  writeRegisterMasked(value, REG_MASK_DR, CONFIG_SYS0_ADDRESS);
}

/*
* Consultar valores en ads1148.h línea 171
* para valores de Data Ready Mode Setting
*/
void ADS1148::setDRDYmode(uint8_t value) {
  /* Controls the behavior of the DOUT/DRDY pin when new data are ready. */
  writeRegisterMasked(value, REG_MASK_DRDY_MODE, CONFIG_IDAC0_ADDRESS);
}

/*
* Consultar valores en ads1148.h línea 175
* para valores de IDAC Excitation Current Magnitude
*/
void ADS1148::setIDACcurrent(uint8_t value) {
  /* Set current for IDAC0 excitation sources */
  writeRegisterMasked(value, REG_MASK_IMAG, CONFIG_IDAC0_ADDRESS);
}

/*
* Consultar valores en ads1148.h línea 189
* para valores de IDAC Excitation Current Output 1
*/
void ADS1148::setIDAC1output(uint8_t value) {
  /* Selects where IDAC1 is routed */
  writeRegisterMasked(value, REG_MASK_I1DIR, CONFIG_IDAC1_ADDRESS);
}

/*
* Consultar valores en ads1148.h línea 202
* para valores de IDAC Excitation Current Output 2
*/
void ADS1148::setIDAC2output(uint8_t value) {
  /* Selects where IDAC2 is routed */
  // Make sure the value is in the valid range. Otherwise set to 0x00
  writeRegisterMasked(value, REG_MASK_I2DIR, CONFIG_IDAC1_ADDRESS);
}

void ADS1148::reset() {
  sendCommand(CMD_RESET);
}

void ADS1148::stopContinuousMode() {
  sendCommand(CMD_SDATAC);
}

void ADS1148::startSync() {
  sendCommand(CMD_SYNC);
  sendCommand(CMD_SYNC);  // Se requiere enviar el comando 2 veces.
}

void ADS1148::runContinuousMode() {
  sendCommand(CMD_RDATAC);
}
/*
void ADS1148::sleep() {
  sendCommand(CMD_SLEEP);
}

void ADS1148::wakeUp() {
  sendCommand(CMD_WAKEUP);
}*/

void ADS1148::rdata() {
  sendCommand(CMD_RDATA);
}
