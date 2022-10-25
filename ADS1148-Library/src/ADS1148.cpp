#include "ADS1148.h"
#include <Arduino.h>



#if defined(__AVR__)
/**
 * @brief  Constructor for SPI-interfaced ADS1148 adc
 * @param  cs_pin
 *         a byte indicating the pin to be use as the chip select pin
 * @param  drdy_pin
 *         a byte indicating the pin to be use as the drdy pin
 * @param  start_pin
 *         a byte indicating the pin to be use as the start pin
 * @param  reset_pin
 *         a byte indicating the pin to be use as the reset pin
 * @return ADS1148 object.
 * @note   Call the object's begin() function before use -- buffer
 *         allocation is performed there!
 */
ADS1148::ADS1148(uint8_t cs_pin, uint8_t drdy_pin, uint8_t start_pin, uint8_t reset_pin) {

  // Set pins up
  ADS1148_CS_PIN = cs_pin;
  ADS1148_DRDY_PIN = drdy_pin;
  ADS1148_START_PIN = start_pin;
  ADS1148_RESET_PIN = reset_pin;
  ADS1148_MODE = DRDY_PIN_ONLY_DOUT;
  
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
/**
 * @brief  Constructor for SPI-interfaced ADS1148 adc
 * @param  cs_pin
 *         a byte indicating the pin to be use as the chip select pin
 * @return ADS1148 object.
 * @note   1. Call the object's begin() function before use -- buffer
 *         allocation is performed there!
 *         2. MOSI, MISO, SCK pins are connected to the microcontroller (SPI Hardware)
 *         3. RESET and START pins are connected to vcc
 *         4. !DRDY pin is not connected
 */
ADS1148::ADS1148(uint8_t cs_pin) {

  // Set pins up
  ADS1148_CS_PIN = cs_pin;
  ADS1148_DRDY_PIN = -1;
  ADS1148_START_PIN = -1;
  ADS1148_RESET_PIN = -1;
  ADS1148_MODE = DRDY_PIN_DOUT_DRDY;    // MOSI pin has both functions as Data Out and Data Ready
                                        // Cannot be used in SDATAC mode datasheet ads1148 Pg. 54
  // Configure chip select as an output
  pinMode(ADS1148_CS_PIN, OUTPUT);
  
  // Disable the device by setting the CS pin high;
  digitalWrite(ADS1148_CS_PIN, HIGH); // Set CS HIGH

  // Configure MISO as as input (DOUT/DRDY pin signals)
  pinMode(MISO, INPUT);
}
						///< This method initialize the SPI port and the config register        
#elif defined(ESP32)
/**
 * @brief  Constructor for SPI-interfaced ADS1148 adc
 * @param  cs_pin
 *         a byte indicating the pin to be use as the chip select pin
 * @param  drdy_pin
 *         a byte indicating the pin to be use as the drdy pin
 * @param  start_pin
 *         a byte indicating the pin to be use as the start pin
 * @param  reset_pin
 *         a byte indicating the pin to be use as the reset pin
 * @param  spi
 *         Pointer to an existing SPIClass instance (e.g. &SPI, the
 *         microcontroller's primary SPI bus).
 * @return ADS1148 object.
 * @note   Call the object's begin() function before use -- buffer
 *         allocation is performed there!
 */
ADS1148::ADS1148(uint8_t cs_pin, uint8_t drdy_pin, uint8_t start_pin, uint8_t reset_pin, SPIClass *spi) {
  // Set pins up
  ADS1148_CS_PIN = cs_pin;
  ADS1148_DRDY_PIN = drdy_pin;
  ADS1148_START_PIN = start_pin;
  ADS1148_RESET_PIN = reset_pin;
  pSpi = spi;
  ADS1148_MODE = DRDY_PIN_ONLY_DOUT;
  
  // Configure start as an output
  pinMode(ADS1148_START_PIN, OUTPUT);

  // Enable the device by setting the START pin high;
  digitalWrite(ADS1148_START_PIN, HIGH); // Set START High

  // Configure DRDY as as input (mfg wants us to use interrupts)
  pinMode(ADS1148_DRDY_PIN, INPUT);

  // Configure reset as an output
  pinMode(ADS1148_RESET_PIN, OUTPUT);
  
  // Enable the device by setting the RESET pin high;
  digitalWrite(ADS1148_RESET_PIN, HIGH);  // Set reset HIGH

  // Configure chip select as an output
  pinMode(ADS1148_CS_PIN, OUTPUT);
  
  // Disable the device by setting the CS pin high;
  digitalWrite(ADS1148_CS_PIN, HIGH); // Set CS HIGH

    // Configure MISO as as input (DOUT/DRDY pin signals)
  pinMode(MISO, INPUT);
  
}       

/**
 * @brief  Constructor for SPI-interfaced ADS1148 adc
 * @param  cs_pin
 *         a byte indicating the pin to be use as the chip select pin
 * @return ADS1148 object.
 * @note   1. Call the object's begin() function before use -- buffer
 *         allocation is performed there!
 *         2. MOSI, MISO, SCK pins are connected to the microcontroller (SPI Hardware)
 *         3. RESET and START pins are connected to vcc
 *         4. !DRDY pin is not connected
 */
ADS1148::ADS1148(uint8_t cs_pin, SPIClass *spi) {

  // Set pins up
  ADS1148_CS_PIN = cs_pin;
  ADS1148_DRDY_PIN = -1;
  ADS1148_START_PIN = -1;
  ADS1148_RESET_PIN = -1;
  pSpi = spi;
  ADS1148_MODE = DRDY_PIN_DOUT_DRDY;    // MOSI pin has both functions as Data Out and Data Ready
                                        // Cannot be used in SDATAC mode datasheet ads1148 Pg. 54
  // Configure chip select as an output
  pinMode(ADS1148_CS_PIN, OUTPUT);
  
  // Disable the device by setting the CS pin high;
  digitalWrite(ADS1148_CS_PIN, HIGH); // Set CS HIGH

  // Configure MISO as as input (DOUT/DRDY pin signals)
  pinMode(MISO, INPUT);
}
#endif


#if defined(__AVR__)
/**
 * @brief  Este metodo se encarga de modificar un byte en el registro 
 *         que se proporciona en sus parametros
 * @param  address
 *         direccion al que se desea modificar (0 ~ 14) 
 *         ver seccion "Configuration registers" en ADS1148.h Ln. 29
 *         y ver defs en ADS1148.h Ln. 30 hasta Ln. 44
 * @param  value
 *         byte que se desea escribir en la direccion del registro
 * @return Nothing.
 * @note   Esta es una funcion de apoyo para el metodo writeRegisterMasked(),
 *         ya que por lo general, no se requiere modificar todo el byte en el
 *         cual esta incluido multiples funciones del adc.
 */
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
/**
 * @brief  Este metodo se encarga de leer un byte en el registro 
 *         que se proporciona en sus parametros
 * @param  address
 *         direccion al que se desea leer (0 ~ 14) 
 *         ver seccion "Configuration registers" en ADS1148.h Ln. 29
 *         y ver defs en ADS1148.h Ln. 30 hasta Ln. 44
 * @return El byte leido de la direccion proporcionada.
 * @note   Nothing.
 */
uint8_t ADS1148::readRegister(uint8_t address) {
  SPI.beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE1));
  digitalWrite(ADS1148_CS_PIN,LOW);
  delayMicroseconds(1);  // tcssc
  SPI.transfer(CMD_RREG|address); 
  delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  SPI.transfer(0x00); // Sólo se recibe 1 byte
  delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  uint8_t data = SPI.transfer(CMD_NOP);
  delayMicroseconds(2);  // tsccs = 5 * tclk = 5 * (1 / 4096000)
  SPI.transfer(CMD_NOP);
  delayMicroseconds(2);  // tsccs = 5 * tclk = 5 * (1 / 4096000)
  digitalWrite(ADS1148_CS_PIN,HIGH);
  delayMicroseconds(2);  // tcspw
  SPI.endTransaction();
  return data;
}
/**
 * @brief  This method initialize the SPI port and the config register 
 * @return true on successful allocation/init, false otherwise.
 * @note   call this function before any reading data!
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
    registerValues [i] = SPI.transfer(CMD_NOP);  // Se recibe el dato de la primera dirección solicitada (00h) en el puerto SPI
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
  stopContinuousMode();

  if (ADS1148_MODE == DRDY_PIN_DOUT_DRDY){
    // In mode (DRDY_PIN_DOUT_DRDY) only the SPI port pins
    // (CS, MOSI, MISO, SCK) are connected to the microcontroller
    // RESET and START pins are connected to vcc
    setDRDYmode(DRDY_PIN_DOUT_DRDY);  // MOSI pin has both functions as Data Out and Data Ready
                                      // Cannot be used in SDATAC mode datasheet ads1148 Pg. 54

  } else {
    // In mode (DRDY_PIN_ONLY_DOUT) all pins are connected to microcontroller
    setDRDYmode(DRDY_PIN_ONLY_DOUT);  // MOSI pin only function as Data Out
  }

  return flag;
}
/**
 * @brief  This method read the conversion result 
 * @return Pointer to an existing instance (vector with 4 bytes included).
 * @note   drdy pin indicates that the conversion is ready
 */
byte * ADS1148::readADC_Array() {
  SPI.beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE1));
  digitalWrite(ADS1148_CS_PIN, LOW); // Take CS low
  delayMicroseconds(1); // Minimum of td(CSSC)
  
  if (!isContinuous){
	SPI.transfer(CMD_RDATA);
	delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  }
  
  static byte dataarray[4];
  for (int x = 0; x < 4 ; x++)
  {
    dataarray[x] = SPI.transfer(CMD_NOP);
	delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  }
  
  digitalWrite(ADS1148_CS_PIN, HIGH);
  delayMicroseconds(2);  // tcspw
  SPI.endTransaction();
  return dataarray;
}
/**
 * @brief  Este metodo se encarga de enviar un comando definido por el fabricante 
 * @param  command
 *         ver seccion "Commands for the ADC" en ADS1148.h Ln. 7
 *         y ver defs en ADS1148.h Ln. 8 hasta Ln. 27
 * @return Nothing.
 * @note   Nothing.
 */
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
 * @brief  Este metodo se encarga de modificar un valor en el registro 
 *         que se proporciona en sus parametros
 * @param  address
 *         direccion al que se desea modificar (0 ~ 14) 
 *         ver seccion "Configuration registers" en ADS1148.h Ln. 29
 *         y ver defs en ADS1148.h Ln. 30 hasta Ln. 44
 * @param  value
 *         byte que se desea escribir en la direccion del registro
 * @return Nothing.
 * @note   Esta es una funcion de apoyo para el metodo writeRegisterMasked(),
 *         ya que por lo general, no se requiere modificar todo el byte en el
 *         cual esta incluido multiples funciones del adc.
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
 * @brief  Este metodo se encarga de leer un byte en el registro 
 *         que se proporciona en sus parametros
 * @param  address
 *         direccion al que se desea leer (0 ~ 14) 
 *         ver seccion "Configuration registers" en ADS1148.h Ln. 29
 *         y ver defs en ADS1148.h Ln. 30 hasta Ln. 44
 * @return El byte leido de la direccion proporcionada.
 * @note   Nothing.
 */
uint8_t ADS1148::readRegister(uint8_t address) {
  pSpi->beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE1));
  digitalWrite(ADS1148_CS_PIN,LOW);
  delayMicroseconds(1);  // tcssc
  pSpi->transfer(CMD_RREG|address); 
  delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  pSpi->transfer(0x00); // Sólo se recibe 1 byte
  delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  uint8_t data = pSpi->transfer(CMD_NOP);
  delayMicroseconds(2);  // tsccs = 5 * tclk = 5 * (1 / 4096000)
  pSpi->transfer(CMD_NOP);
  delayMicroseconds(2);  // tsccs = 5 * tclk = 5 * (1 / 4096000)
  digitalWrite(ADS1148_CS_PIN,HIGH);
  delayMicroseconds(2);  // tcspw
  pSpi->endTransaction();
  return data;
}
/**
 * @brief  This method initialize the SPI port and the config register 
 * @return true on successful allocation/init, false otherwise.
 * @note   call this function before any reading data!
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
    registerValues [i] = pSpi->transfer(CMD_NOP);  // Se recibe el dato de la primera dirección solicitada (00h) en el puerto SPI
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
  stopContinuousMode();

  if (ADS1148_MODE == DRDY_PIN_DOUT_DRDY){
    // In mode (DRDY_PIN_DOUT_DRDY) only the SPI port pins
    // (CS, MOSI, MISO, SCK) are connected to the microcontroller
    // RESET and START pins are connected to vcc
    setDRDYmode(DRDY_PIN_DOUT_DRDY);  // MOSI pin has both functions as Data Out and Data Ready
                                      // Cannot be used in SDATAC mode datasheet ads1148 Pg. 54

  } else {
    // In mode (DRDY_PIN_ONLY_DOUT) all pins are connected to microcontroller
    setDRDYmode(DRDY_PIN_ONLY_DOUT);  // MOSI pin only function as Data Out
  }
  
  return flag;
}
/**
 * @brief  This method read the conversion result 
 * @return Pointer to an existing instance (vector with 4 bytes included).
 * @note   drdy pin indicates that the conversion is ready
 */
byte * ADS1148::readADC_Array() {
  pSpi->beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE1));
  digitalWrite(ADS1148_CS_PIN, LOW); // Take CS low
  delayMicroseconds(1); // Minimum of td(CSSC)
  
  if (!isContinuous){
	pSpi->transfer(CMD_RDATA);
	delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  }
  
  static byte dataarray[4];     // To force DOUT/DRDY high, a NOP can be sent to force DOUT/DRDY high (ver datasheet Pag. 35)
  for (int x = 0; x < 4 ; x++)  // twice NOP to get the conversion and an additional twice to force DOUT/DRDY high
  {
    dataarray[x] = pSpi->transfer(CMD_NOP);
	  delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  }
  
  digitalWrite(ADS1148_CS_PIN, HIGH);
  delayMicroseconds(2);  // tcspw
  pSpi->endTransaction();
  return dataarray;
}
/**
 * @brief  Este metodo se encarga de enviar un comando definido por el fabricante 
 * @param  command
 *         ver seccion "Commands for the ADC" en ADS1148.h Ln. 7
 *         y ver defs en ADS1148.h Ln. 8 hasta Ln. 27
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::sendCommand(uint8_t command) {
  
  pSpi->beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE1));
  digitalWrite(ADS1148_CS_PIN, LOW);
  delayMicroseconds(1);  // tcssc
  pSpi->transfer(command);
  delayMicroseconds(2);  // tstd = 5 * tclk = 5 * (1 / 4096000)
  if (command == CMD_SLEEP){  // !CS must be held low for the duration of the power-down mode
    digitalWrite(ADS1148_CS_PIN, HIGH);
    delayMicroseconds(2);  // tcspw
  }
  pSpi->endTransaction();
}
#endif
/**
 * @brief  Esta funcion devuelve el estado del pin !DRDY
 * @return true (si el pin esta en bajo) e indica que la conversion de adc esta completada
 *         false (si el pin esta en alto) e indica que el convertidor aun no termina con la operacion
 * @note   Esta funcion es obligatoria utilizarla para capturar el valor mas reciente de la conversion
 */
bool ADS1148::isDataReady() {
  bool ready = false;
  
  if (ADS1148_MODE == DRDY_PIN_DOUT_DRDY){

    digitalWrite(ADS1148_CS_PIN, LOW); // Set CS Low
    delayMicroseconds(1); // Wait a minimum of tcssc

    if (digitalRead(MISO) == LOW)
      ready = true;
    
    //Clear CS to high (resets the serial interface);
    digitalWrite(ADS1148_CS_PIN, HIGH); // Clear CS to high
  
  } else {
      if (digitalRead(ADS1148_DRDY_PIN) == LOW)
        ready = true;
  }
  return ready;
}
/**
 * @brief  Esta funcion configura al ads1148 para que el adc pueda leer el sensor de temperatura interno.
 * @return Nothing
 * @note   Nothing
 */
void ADS1148::setReadingInternalTemperature(){
	
  setOpMode(TEMP_MEASUREMENT); 	// Medición de temperatura interna */
  startSync();
}
/**
 * @brief  Esta funcion configura al ads1148 para que el adc pueda leer un termopar.
 * @param  channel
 *         especifica el canal de entrada que desea leer
 *         ver seccion "Analog input channel" en ADS1148.h Ln. 232
 *         y ver defs en ADS1148.h Ln. 233 hasta Ln. 236
 * @return Nothing
 * @note   Nothing
 */
void ADS1148::setReadingThermocouple(uint8_t channel){
	
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
/**
 * @brief  Esta funcion lee la temperatura interna del ads1148.
 * @return Devuelve el valor de la temperatura interna en grados centígrados.
 * @note   Se requiere la configuracion previa del ADS1148 para leer el sensor interno de temperatura
 */
double ADS1148::readInternalTemperature(){
	
  double adcValue = 0;
	
  adcValue = (double)convertToValue(readADC_Array());
	
  /* (+-)2.048V 
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
/**
 * @brief  Esta funcion lee el voltaje de entrada.
 * @return Devuelve el valor del voltaje en volts
 * @note   Se requiere la configuracion previa del ADS1148 para leer el voltaje de entrada
 */
double ADS1148::readInputVoltage(){

  uint16_t adcValue = 0;
	
  adcValue = (double)convertToValue(readADC_Array());
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
/**
 * @brief  Esta funcion lee el voltaje de entrada.
 * @return Devuelve el valor del voltaje en milivolts
 * @note   Se requiere la configuracion previa del ADS1148 para leer el voltaje de entrada
 */
double ADS1148::readInputVoltageIn_mV(){

  uint16_t adcValue = 0;
	
  /* (+-)2.048V -> 65535 negative full scale --> 32767 positive full scale
  *  -2.048V -> 0xFFFF ... 0V -> 0x0000 ... 2.048V -> 2^15 = 0x7FFF = 32768
  */
  adcValue = (double)convertToValue(readADC_Array());
  
  if (adcValue > 0x7FFF) {  // Valores negativos
	adcValue = (~(adcValue - 1));
	return ((double) (((double)adcValue * (-1.0)) / 512.0));
  }
	
  return ((double) (((double)adcValue) / 512.0));
}
/**
 * @brief  Funcion de apoyo para convertir un array de 2 bytes
 *         a un numero de 16 bits
 * @return Devuelve el valor en formato de 16 bits
 * @note   Nothing
 */
uint16_t ADS1148::convertToValue(byte * data) {
  return (data[0])<<8 | (data[1]); //Moving MSB and LSB to 16 bit
}
/**
 * @brief  Este metodo se encarga de configurar en el registro del ADS1148 
 *         el modo en que se desea los pines GPIO
 * @param  pin
 *         el pin que se desea configurar en modo digital (INPUT o OUTPUT)
 *         ver seccion "GPIO Pins" en ADS1148.h Ln. 222
 *         y ver defs en ADS1148.h Ln. 223 hasta Ln. 230
 * @param  mode
 *         modo que se desea configurar ya sea como 
 *         entrada: INPUT
 *         salida:  OUTPUT
 * @return Nothing.
 * @note   Nothing.
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
 * @brief  Este metodo se encarga de configurar en el registro del ADS1148 
 *         la salida digital que se desea los pines GPIO del ADS1148 (HIGH or LOW)
 * @param  pin
 *         el pin que se desea configurar
 *         ver seccion "GPIO Pins" en ADS1148.h Ln. 222
 *         y ver defs en ADS1148.h Ln. 223 hasta Ln. 230
 * @param  value
 *         salida que se desea en el pin 
 *         alto: HIGH
 *         bajo:  LOW
 * @return Nothing.
 * @note   Se requiere la configuracion previa del pin como salida.
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
 * @brief  Este metodo se encarga de configurar en el registro del ADS1148 
 *         la captura del valor de entrada en el pin GPIO del ADS1148 (HIGH or LOW)
 * @param  pin
 *         el pin que se desea configurar
 *         ver seccion "GPIO Pins" en ADS1148.h Ln. 222
 *         y ver defs en ADS1148.h Ln. 223 hasta Ln. 230
 * @return El valor del estado de entrada del pin (HIGH or LOW).
 * @note   Se requiere la configuracion previa del pin como entrada.
 */
bool ADS1148::digitalReadGPIO(uint8_t pin){
	
  uint8_t value;
	

  /* Read GPIODAT register*/
  value = readRegister(CONFIG_GPIODAT_ADDRESS);
  
  return (value & pin);
	
}
/**
 * @brief  Write the value to a register using the mask to leave the rest of the
 *         register untouched. This does not shift the value, so it shoudl be provided
 *         shifted to the appropriate positions.
 * @param  value
 *         Opcion que se desea configurar
 *         ver seccion "REG_MASK_NAME mask values" en ADS1148.h
 * @param  mask
 *         Bits del registro que se desean modificar 
 *         ver seccion "Register mask - for CONFIG_NAME_ADDRESS" en ADS1148.h
 * @param  address
 *         Direccion del registro que se desea modificar
 *         ver seccion "Configuration registers" en ADS1148.h
 * @return Nothing.
 * @note   Nothing.
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
/**
 * @brief  Esta funcion selecciona los valores de Burn-out Detect Current source
 * @param  value
 *         Opcion que se desea configurar
 *         ver seccion "REG_MASK_BCS mask values" en ADS1148.h Ln 52
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::setBurnoutCurrentSources(uint8_t value) {
  /* Adjust burn-out current sources */
  writeRegisterMasked(value, REG_MASK_BCS, CONFIG_MUX0_ADDRESS);
}
/**
 * @brief  Esta funcion selecciona los valores de Multiplexer Selection - ADC Positive Input
 * @param  value
 *         Opcion que se desea configurar
 *         ver seccion "REG_MASK_MUX_SP mask values" en ADS1148.h Ln 58
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::setADCPositiveInput(uint8_t value){
  /* Adjust ADC Positive Input */
  writeRegisterMasked(value, REG_MASK_MUX_SP, CONFIG_MUX0_ADDRESS);	
}
/**
 * @brief  Esta funcion selecciona los valores de Multiplexer Selection - ADC Negative Input
 * @param  value
 *         Opcion que se desea configurar
 *         ver seccion "REG_MASK_MUX_SN mask values" en ADS1148.h Ln 68
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::setADCNegativeInput(uint8_t value){
  /* Adjust ADC Negative Input */
  writeRegisterMasked(value, REG_MASK_MUX_SN, CONFIG_MUX0_ADDRESS);	
}
/**
 * @brief  Esta funcion selecciona los valores de VBIAS—Bias Voltage Register output selection
 * @param  value
 *         Opcion que se desea configurar
 *         ver seccion "REG_MASK_VBIAS mask values" en ADS1148.h Ln 81
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::setVBIASEnableTo(uint8_t value){
  /* Adjust VBIAS—Bias Voltage */
  writeRegisterMasked(value, REG_MASK_VBIAS, CONFIG_VBIAS_ADDRESS);	
}
/**
 * @brief  Esta funcion selecciona los valores de Internal Reference Control
 * @param  value
 *         Opcion que se desea configurar
 *         ver seccion "REG_MASK_VREFCON mask values" en ADS1148.h Ln 98
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::setInternalRef(uint8_t value){
  /* Adjust Internal Reference Control */
  writeRegisterMasked(value, REG_MASK_VREFCON, CONFIG_MUX1_ADDRESS);	
}
/**
 * @brief  Esta funcion selecciona los valores de Reference Control Select
 * @param  value
 *         Opcion que se desea configurar
 *         ver seccion "REG_MASK_REFSELT mask values" en ADS1148.h Ln 103
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::setVoltageRef(uint8_t value){
  /* Adjust Reference Control Select*/
  writeRegisterMasked(value, REG_MASK_REFSELT, CONFIG_MUX1_ADDRESS);	
}
/**
 * @brief  Esta funcion selecciona los valores de System Monitor Control
 * @param  value
 *         Opcion que se desea configurar
 *         ver seccion "REG_MASK_MUXCAL mask values" en ADS1148.h Ln 109
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::setOpMode(uint8_t value) {
	/* Adjust System Monitor Control*/
  writeRegisterMasked(value, REG_MASK_MUXCAL, CONFIG_MUX1_ADDRESS);
}
/**
 * @brief  Esta funcion selecciona los valores de Gain Setting for PGA
 * @param  value
 *         Opcion que se desea configurar
 *         ver seccion "REG_MASK_PGA mask values" en ADS1148.h Ln 124
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::setGain(uint8_t value) {
  /* Sets ADC gain. */
  writeRegisterMasked(value, REG_MASK_PGA, CONFIG_SYS0_ADDRESS);
}
/**
 * @brief  Esta funcion selecciona los valores de Data Output Rate
 * @param  value
 *         Opcion que se desea configurar
 *         ver seccion "REG_MASK_DR mask values" en ADS1148.h Ln 134
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::setDataRate(uint8_t value) {
  /* Sets the data rate for the ADC. */
  writeRegisterMasked(value, REG_MASK_DR, CONFIG_SYS0_ADDRESS);
}
/**
 * @brief  Esta funcion selecciona los valores de Data Ready Mode Setting
 * @param  value
 *         Opcion que se desea configurar
 *         ver seccion "REG_MASK_DRDY_MODE mask values" en ADS1148.h Ln 169
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::setDRDYmode(uint8_t value) {
  /* Controls the behavior of the DOUT/DRDY pin when new data are ready. */
  writeRegisterMasked(value, REG_MASK_DRDY_MODE, CONFIG_IDAC0_ADDRESS);
  if (value == DRDY_PIN_DOUT_DRDY){
    ADS1148_MODE = value;
    runContinuousMode();  // pin functions both as Data Out and Data Ready
                          // Cannot be used in SDATAC mode datasheet ads1148 Pg. 54
  }
  startSync();
}
/**
 * @brief  Esta funcion selecciona los valores de IDAC Excitation Current Magnitude
 * @param  value
 *         Opcion que se desea configurar
 *         ver seccion "REG_MASK_IMAG mask values" en ADS1148.h Ln 173
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::setIDACcurrent(uint8_t value) {
  /* Set current for IDAC0 excitation sources */
  writeRegisterMasked(value, REG_MASK_IMAG, CONFIG_IDAC0_ADDRESS);
}
/**
 * @brief  Esta funcion selecciona los valores de IDAC Excitation Current Output 1
 * @param  value
 *         Opcion que se desea configurar
 *         ver seccion "REG_MASK_I1DIR mask values" en ADS1148.h Ln 187
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::setIDAC1output(uint8_t value) {
  /* Selects where IDAC1 is routed */
  writeRegisterMasked(value, REG_MASK_I1DIR, CONFIG_IDAC1_ADDRESS);
}
/**
 * @brief  Esta funcion selecciona los valores de IDAC Excitation Current Output 2
 * @param  value
 *         Opcion que se desea configurar
 *         ver seccion "REG_MASK_I2DIR mask values" en ADS1148.h Ln 200
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::setIDAC2output(uint8_t value) {
  /* Selects where IDAC2 is routed */
  // Make sure the value is in the valid range. Otherwise set to 0x00
  writeRegisterMasked(value, REG_MASK_I2DIR, CONFIG_IDAC1_ADDRESS);
}
/**
 * @brief  Esta funcion selecciona envia el comando RESET al ADS1148
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::reset() {
  sendCommand(CMD_RESET);
}
/**
 * @brief  Esta funcion selecciona envia el comando SDATAC (detener modo lectura continua) al ADS1148
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::stopContinuousMode() {
  sendCommand(CMD_SDATAC);
  isContinuous = false;
}
/**
 * @brief  Esta funcion selecciona envia el comando SYNC (reinicia lecturas del ADC) al ADS1148
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::startSync() {
  sendCommand(CMD_SYNC);
  sendCommand(CMD_SYNC);  // Se requiere enviar el comando 2 veces.
}
/**
 * @brief  Esta funcion selecciona envia el comando RDATAC (reanuda modo lectura continua) al ADS1148
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::runContinuousMode() {
  sendCommand(CMD_RDATAC);
  isContinuous = true;
}
/**
 * @brief  The SLEEP command places the device into power-down mode
 *         Note that this command does not automatically power down the internal voltage reference
 *         If the START pin is held low, a WAKEUP command does not power up the device
 *         When using the SLEEP command, CS must be held low for the duration of the power-down mode (keeps the SPI port in use)
 *         There are two ways to put the device into power-down mode: using the SLEEP command and taking the START pin low.
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::sleep() {
  sendCommand(CMD_SLEEP);
}
/**
 * @brief  Use the WAKEUP command to power up the device after a SLEEP command
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::wakeUp() {
  sendCommand(CMD_WAKEUP);
}
/**
 * @brief  Esta funcion selecciona envia el comando RDATA (Read Data Once) al ADS1148
 * @return Nothing.
 * @note   Nothing.
 */
void ADS1148::rdata() {
  sendCommand(CMD_RDATA);
}
