#ifndef ADS1148_h
#define ADS1148_h

#include <Arduino.h>
#include <SPI.h>

// Commands for the ADC
//System Control
#define CMD_WAKEUP 		0x00
#define CMD_SLEEP 		0x02
#define CMD_SYNC		0x04
#define CMD_RESET 		0x06
#define CMD_NOP	 		0xFF

//Data Read
#define CMD_RDATA		0x12
#define CMD_RDATAC 		0x14
#define CMD_SDATAC 		0x16

//Register Read / Write
#define CMD_RREG		0x20
#define CMD_WREG		0x40

//Calibration
#define CMD_SYSOCAL		0x60
#define CMD_SYSGCAL		0x61
#define CMD_SELFOCAL	0x62

// Configuration registers
#define CONFIG_MUX0_ADDRESS 	0x00
#define CONFIG_VBIAS_ADDRESS 	0x01
#define CONFIG_MUX1_ADDRESS 	0x02
#define CONFIG_SYS0_ADDRESS 	0x03
#define CONFIG_OFC0_ADDRESS 	0x04
#define CONFIG_OFC1_ADDRESS 	0x05
#define CONFIG_OFC2_ADDRESS 	0x06
#define CONFIG_FSC0_ADDRESS 	0x07
#define CONFIG_FSC1_ADDRESS 	0x08
#define CONFIG_FSC2_ADDRESS 	0x09
#define CONFIG_IDAC0_ADDRESS 	0x0A
#define CONFIG_IDAC1_ADDRESS 	0x0B
#define CONFIG_GPIOCFG_ADDRESS 	0x0C
#define CONFIG_GPIODIR_ADDRESS 	0x0D
#define CONFIG_GPIODAT_ADDRESS 	0x0E

// Register masks for settings
// Register mask - for CONFIG_MUX0_ADDRESS (MUX0 Register)
#define REG_MASK_BCS 0xC0 		// Burn-out Detect Current Source Register
#define REG_MASK_MUX_SP 0x38  	// Multiplexer Selection - ADC Positive Input selection
#define REG_MASK_MUX_SN 0x07 	// Multiplexer Selection - ADC Negative Input selection

// REG_MASK_BCS mask values -  Burn-out Detect Current Source Register  selection
#define BURNOUT_SOURCE_OFF 		0x00    // Burn-out current source off (default)
#define BURNOUT_SOURCE_HALF_UA 	0x40  	// Burn-out current source on, 0.5 uA
#define BURNOUT_SOURCE_2UA 		0x80    // Burn-out current source on, 2 uA
#define BURNOUT_SOURCE_10UA 	0xC0 	// Burn-out current source on, 10 uA

// REG_MASK_MUX_SP mask values - Multiplexer Selection - ADC Positive Input selection
#define POSITIVE_INPUT_AIN0 0x00 // AIN0 (default)
#define POSITIVE_INPUT_AIN1 0x08 // AIN1
#define POSITIVE_INPUT_AIN2 0x10 // AIN2
#define POSITIVE_INPUT_AIN3 0x18 // AIN3
#define POSITIVE_INPUT_AIN4 0x20 // AIN4
#define POSITIVE_INPUT_AIN5 0x28 // AIN5
#define POSITIVE_INPUT_AIN6 0x30 // AIN6
#define POSITIVE_INPUT_AIN7 0x38 // AIN7

// REG_MASK_MUX_SN mask values - Multiplexer Selection - ADC Negative Input selection
#define NEGATIVE_INPUT_AIN0 0x00 // AIN0
#define NEGATIVE_INPUT_AIN1 0x01 // AIN1 (default)
#define NEGATIVE_INPUT_AIN2 0x02 // AIN2
#define NEGATIVE_INPUT_AIN3 0x03 // AIN3
#define NEGATIVE_INPUT_AIN4 0x04 // AIN4
#define NEGATIVE_INPUT_AIN5 0x05 // AIN5
#define NEGATIVE_INPUT_AIN6 0x06 // AIN6
#define NEGATIVE_INPUT_AIN7 0x07 // AIN7

// Register mask - for CONFIG_VBIAS_ADDRESS (VBIAS Register)
#define REG_MASK_VBIAS 0xFF

// REG_MASK_VBIAS mask values - VBIAS—Bias Voltage Register output selection
#define VBIAS_APPLIED_OFF_ALL_PIN 0x00  // voltage of mid-supply (AVDD + AVSS) / 2 is off to all inputs
#define VBIAS_APPLIED_TO_AIN0 0x01 		// voltage of mid-supply (AVDD + AVSS) / 2 is applied to AIN0
#define VBIAS_APPLIED_TO_AIN1 0x02 		// voltage of mid-supply (AVDD + AVSS) / 2 is applied to AIN1
#define VBIAS_APPLIED_TO_AIN2 0x04 		// voltage of mid-supply (AVDD + AVSS) / 2 is applied to AIN2
#define VBIAS_APPLIED_TO_AIN3 0x08 		// voltage of mid-supply (AVDD + AVSS) / 2 is applied to AIN3
#define VBIAS_APPLIED_TO_AIN4 0x10 		// voltage of mid-supply (AVDD + AVSS) / 2 is applied to AIN4
#define VBIAS_APPLIED_TO_AIN5 0x20 		// voltage of mid-supply (AVDD + AVSS) / 2 is applied to AIN5
#define VBIAS_APPLIED_TO_AIN6 0x40 		// voltage of mid-supply (AVDD + AVSS) / 2 is applied to AIN6
#define VBIAS_APPLIED_TO_AIN7 0x80 		// voltage of mid-supply (AVDD + AVSS) / 2 is applied to AIN7

// Register mask - for CONFIG_MUX1_ADDRESS (MUX1 Register)
#define REG_MASK_CLKSTAT 0x80  // Sólo lectura
#define REG_MASK_VREFCON 0x60
#define REG_MASK_REFSELT 0x18
#define REG_MASK_MUXCAL 0x07

// REG_MASK_VREFCON mask values - Internal Reference Control selection
#define INTERNAL_REF_ALWAYS_OFF 0x00 		// Internal reference is always off (default)
#define INTERNAL_REF_ALWAYS_ON 0x20 		// Internal reference is always on
#define INTERNAL_REF_ONLY_CONVERSION 0x40 	// Internal reference is on when a conversion is in progress

// REG_MASK_REFSELT mask values - Reference Select Control selection
#define REF_INPUT_IN_REFP0_REFN0 0x00 			// REFP0 and REFN0 reference inputs selected (default)
#define REF_INPUT_IN_REFP1_REFN1 0x08 			// REFP1 and REFN1 reference inputs selected
#define REF_INPUT_INTERNAL 0x10 				// Internal reference selected
#define REF_INPUT_INTERNAL_AND_REFP0_REFN0 0x18 // Internal reference selected and internally connected to REFP0 and REFN0 input pins

// REG_MASK_MUXCAL mask values - System Monitor Control selection
#define NORMAL_OPERATION 0x00 		// Normal operation (default)
#define OFFSET_CALIBRATION 0x01 	// Offset calibration.
#define GAIN_CALIBRATION 0x02 		// Gain calibration
#define TEMP_MEASUREMENT 0x03 		// Temperature measurement
#define REF1_MONITOR 0x04 			// REF1 monitor
#define REF0_MONITOR 0x05 			// REF0 monitor
#define ANALOG_SUPPLY_MONITOR 0x06 	// Analog supply monitor
#define DIGITAL_SUPPLY_MONITOR 0x07 // Digital supply monitor

// Register mask - for CONFIG_SYS0_ADDRESS (SYS0 Register)
#define REG_MASK_RESERVED 0x80  // Sólo lectura
#define REG_MASK_PGA 0x70
#define REG_MASK_DR 0x0F

// REG_MASK_PGA mask values - Gain Setting for PGA
#define GAIN_PGA_TO_1 0x00 // PGA = 1 (default)
#define GAIN_PGA_TO_2 0x10 // PGA = 2
#define GAIN_PGA_TO_4 0x20 // PGA = 4
#define GAIN_PGA_TO_8 0x30 // PGA = 8
#define GAIN_PGA_TO_16 0x40 // PGA = 16
#define GAIN_PGA_TO_32 0x50 // PGA = 32
#define GAIN_PGA_TO_64 0x60 // PGA = 64
#define GAIN_PGA_TO_128 0x70 // PGA = 128

// REG_MASK_DR mask values - Data Output Rate Setting
#define DATA_RATE_TO_5_SPS 0x00 	// DR = 5 SPS (default)
#define DATA_RATE_TO_10_SPS 0x01 	// DR = 10 SPS 
#define DATA_RATE_TO_20_SPS 0x02 	// DR = 20 SPS 
#define DATA_RATE_TO_40_SPS 0x03 	// DR = 40 SPS 
#define DATA_RATE_TO_80_SPS 0x04 	// DR = 80 SPS 
#define DATA_RATE_TO_160_SPS 0x05 	// DR = 160 SPS 
#define DATA_RATE_TO_320_SPS 0x06 	// DR = 320 SPS 
#define DATA_RATE_TO_640_SPS 0x07 	// DR = 640 SPS 
#define DATA_RATE_TO_1000_SPS 0x08 	// DR = 1000 SPS 
#define DATA_RATE_TO_2000_SPS 0x0F 	// DR = 2000 SPS 

// Register mask - for CONFIG_OFC0_ADDRESS (OFC0 Register)
#define REG_MASK_OFC0 0xFF

// Register mask - for CONFIG_OFC1_ADDRESS (OFC1 Register)
#define REG_MASK_OFC1 0xFF

// Register mask - for CONFIG_OFC2_ADDRESS (OFC2 Register)
#define REG_MASK_OFC2 0xFF

// Register mask - for CONFIG_FSC0_ADDRESS (FSC0 Register)
#define REG_MASK_FSC0 0xFF

// Register mask - for CONFIG_FSC1_ADDRESS (FSC1 Register)
#define REG_MASK_FSC1 0xFF

// Register mask - for CONFIG_FSC2_ADDRESS (FSC2 Register)
#define REG_MASK_FSC2 0xFF

// Register mask - for CONFIG_IDAC0_ADDRESS (IDAC0 Register)
#define REG_MASK_ID 0xF0  // Sólo lectura
#define REG_MASK_DRDY_MODE 0x08
#define REG_MASK_IMAG 0x07

// REG_MASK_DRDY_MODE mask values - Data Ready Mode Setting
#define DRDY_PIN_ONLY_DOUT 0x00
#define DRDY_PIN_DOUT_DRDY 0x08

// REG_MASK_IMAG mask values - IDAC Excitation Current Magnitude
#define IDAC_CURRENT_OFF 0x00
#define IDAC_CURRENT_50UA 0x01
#define IDAC_CURRENT_100UA 0x02
#define IDAC_CURRENT_250UA 0x03
#define IDAC_CURRENT_500UA 0x04
#define IDAC_CURRENT_750UA 0x05
#define IDAC_CURRENT_1000UA 0x06
#define IDAC_CURRENT_1500UA 0x07

// Register mask - for CONFIG_IDAC1_ADDRESS (IDAC1 Register)
#define REG_MASK_I1DIR 0xF0
#define REG_MASK_I2DIR 0x0F

// REG_MASK_I1DIR mask values - IDAC Excitation Current Output 1
#define IDAC1_CURRENT_TO_AIN0 0x00
#define IDAC1_CURRENT_TO_AIN1 0x10
#define IDAC1_CURRENT_TO_AIN2 0x20
#define IDAC1_CURRENT_TO_AIN3 0x30
#define IDAC1_CURRENT_TO_AIN4 0x40
#define IDAC1_CURRENT_TO_AIN5 0x50
#define IDAC1_CURRENT_TO_AIN6 0x60
#define IDAC1_CURRENT_TO_AIN7 0x70
#define IDAC1_CURRENT_TO_IEXC1 0xA0
#define IDAC1_CURRENT_TO_IEXC2 0xB0
#define IDAC1_CURRENT_TO_NONE 0xF0

// REG_MASK_I2DIR mask values - IDAC Excitation Current Output 2
#define IDAC2_CURRENT_TO_AIN0 0x00
#define IDAC2_CURRENT_TO_AIN1 0x01
#define IDAC2_CURRENT_TO_AIN2 0x02
#define IDAC2_CURRENT_TO_AIN3 0x03
#define IDAC2_CURRENT_TO_AIN4 0x04
#define IDAC2_CURRENT_TO_AIN5 0x05
#define IDAC2_CURRENT_TO_AIN6 0x06
#define IDAC2_CURRENT_TO_AIN7 0x07
#define IDAC1_CURRENT_TO_IEXC1 0x0A
#define IDAC1_CURRENT_TO_IEXC2 0x0B
#define IDAC1_CURRENT_TO_NONE 0x0F

// Register mask - for CONFIG_GPIOCFG_ADDRESS (GPIOCFG Register)
#define REG_MASK_IOCFG0 0xFF

// Register mask - for CONFIG_GPIODIR_ADDRESS (GPIODIR Register)
#define REG_MASK_IOCFG1 0xFF

// Register mask - for CONFIG_GPIODAT_ADDRESS (GPIODAT Register)
#define REG_MASK_IOCFG2 0xFF

// REG_MASK_IOCFG0, REG_MASK_IOCFG1, REG_MASK_IOCFG2 mask values - GPIO Pins
#define ADS1148_GPIO0_PIN 0x01
#define ADS1148_GPIO1_PIN 0x02
#define ADS1148_GPIO2_PIN 0x04
#define ADS1148_GPIO3_PIN 0x08
#define ADS1148_GPIO4_PIN 0x10
#define ADS1148_GPIO5_PIN 0x20
#define ADS1148_GPIO6_PIN 0x40
#define ADS1148_GPIO7_PIN 0x80

// Analog input channel
#define ANALOG_CHANNEL_0 0x00
#define ANALOG_CHANNEL_1 0x01
#define ANALOG_CHANNEL_2 0x02
#define ANALOG_CHANNEL_3 0x03

class ADS1148 {
  public:
    
#if defined(__AVR__)
    ADS1148(uint8_t cs_pin, uint8_t drdy_pin, uint8_t start_pin, uint8_t reset_pin);         ///< Constructor
    ADS1148(uint8_t cs_pin);
#elif defined(ESP32)
	ADS1148(uint8_t cs_pin, uint8_t drdy_pin, uint8_t start_pin, uint8_t reset_pin, SPIClass *spi = &SPI);	// This method initialize the SPI port and the config register        
    ADS1148(uint8_t cs_pin, SPIClass *spi = &SPI);
#endif
		
    uint8_t ADS1148_CS_PIN;
    uint8_t ADS1148_DRDY_PIN;
	uint8_t ADS1148_START_PIN;
	uint8_t ADS1148_RESET_PIN;
    uint8_t ADS1148_MODE;
    const uint32_t SCLK       = 2000000;///< ADS1148 SCLK frequency: 2000000 Hz Maximum for ADS1148
    bool isContinuous = false;
	void writeRegister(uint8_t address, uint8_t value);
    uint8_t readRegister(uint8_t address);
    bool begin();
    bool isDataReady(void);
    byte * readADC_Array();
	void setReadingInternalTemperature();
	void setReadingThermocouple(uint8_t channel = ANALOG_CHANNEL_0);
	double readInternalTemperature();
	double readInputVoltage();
	double readInputVoltageIn_mV();
	double readInputThermocouple();
	void pinModeGPIO(uint8_t pin, uint8_t mode);
	void digitalWriteGPIO(uint8_t pin, uint8_t value);
	bool digitalReadGPIO(uint8_t pin);
    uint16_t convertToValue(byte * data);
    void sendCommand(uint8_t command);
    void reset(void);
	void stopContinuousMode();
    void startSync(void);
	void runContinuousMode();
    void rdata(void);
    void writeRegisterMasked(uint8_t value, uint8_t mask, uint8_t address);
    void setDataRate(uint8_t value);
    void setDRDYmode(uint8_t value);
    void setBurnoutCurrentSources(uint8_t value);
	void setADCPositiveInput(uint8_t value);
	void setADCNegativeInput(uint8_t value);
	void setVBIASEnableTo(uint8_t value);
	void setInternalRef(uint8_t value);
    void setVoltageRef(uint8_t value);
    void setOpMode(uint8_t value);
    void setGain(uint8_t value);
    void setIDACcurrent(uint8_t value);
    void setIDAC1output(uint8_t value);
    void setIDAC2output(uint8_t value);
    void ADS1148::sleep();
    void ADS1148::wakeUp();
	
private:
#if defined(ESP32)
	SPIClass *pSpi;
#endif 
  };
#endif
