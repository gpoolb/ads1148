#ifndef THERMOCOUPLE_h
#define THERMOCOUPLE_h

/* https://srdata.nist.gov/its90/main/its90_main_page.html
*********************************************************************
* https://srdata.nist.gov/its90/download/type_k.tab
*
*********************************************************************
* This section contains coefficients of approximate inverse 
* functions for type K thermocouples for the subranges of 
* temperature and voltage listed below. The range of errors of 
* the approximate inverse function for each subrange is also given. 
* The coefficients are in units of °C and mV and are listed in 
* the order of constant term up to the highest order.
* The equation is of the form t_90 = d_0 + d_1*E + d_2*E^2 + ... 
*     + d_n*E^n,
* where E is in mV and t_90 is in °C.
*
*    Temperature        Voltage            Error 
*      range              range            range
*      (°C)               (mV)             (° C)
*    -200. to 0.      -5.891 to 0.000    -0.02 to 0.04
*     0. to 500.      0.000 to 20.644    -0.05 to 0.04
*     500. to 1372.   20.644 to 54.886   -0.05 to 0.06
*********************************************************************
Inverse coefficients for type K:
 
Temperature  -200.             0.           500.
  Range:        0.           500.          1372.
 
  Voltage   -5.891          0.000         20.644
  Range:     0.000         20.644         54.886
 
         0.0000000E+00  0.000000E+00 -1.318058E+02
         2.5173462E+01  2.508355E+01  4.830222E+01
        -1.1662878E+00  7.860106E-02 -1.646031E+00
        -1.0833638E+00 -2.503131E-01  5.464731E-02
        -8.9773540E-01  8.315270E-02 -9.650715E-04
        -3.7342377E-01 -1.228034E-02  8.802193E-06
        -8.6632643E-02  9.804036E-04 -3.110810E-08
        -1.0450598E-02 -4.413030E-05  0.000000E+00
        -5.1920577E-04  1.057734E-06  0.000000E+00
         0.0000000E+00 -1.052755E-08  0.000000E+00
 
  Error      -0.02          -0.05          -0.05
  Range:      0.04           0.04           0.06

*/

#ifdef THERMOCOUPLE_TYPE_K
	double thermocoupleCoefficients [3][10] = {{0.0,25.173462,-1.1662878,-1.0833638,-0.89773540,-0.37342377,-0.086632643,-0.010450598,-0.00051920577,0.0}, // Range: -5.891 to 0.0 mV
											 {0.0,25.08355,0.07860106,-0.2503131,0.08315270,-0.01228034,0.0009804036,-0.00004413030,0.000001057734,-0.00000001052755}, // Range: 0.0 to 20.644 mV
											 {-131.8058,48.30222,-1.646031,0.05464731,-0.0009650715,0.000008802193,-0.00000003110810,0.0,0.0,0.0},  // Range: 20.644 to 54.886 mV
											};  
	double rangeThermocouple [4] = {-5.891, 20.644, 54.886, 64.00}; // NOTA: el valor 64.00 es el maximo valor de voltaje entrada al adc en mV
#endif
/*
*********************************************************************
*  https://srdata.nist.gov/its90/download/type_j.tab
*
*******************************************************************
* This section contains coefficients of approximate inverse 
* functions for type J thermocouples for the subranges of 
* temperature and voltage listed below. The range of errors of 
* the approximate inverse function for each subrange is also given. 
* The coefficients are in units of °C and mV and are listed in 
* the order of constant term up to the highest order.
* The equation is of the form t_90 = d_0 + d_1*E + d_2*E^2 + ... 
*     + d_n*E^n,
* where E is in mV and t_90 is in °C.
*
*    Temperature        Voltage            Error 
*      range              range            range
*      (°C)               (mV)             (° C)
*    -210. to 0.      -8.095 to 0.000    -0.05 to 0.03
*     0. to 760.       0.000 to 42.919   -0.04 to 0.04
*     760. to 1200     42.919 to 69.553  -0.04 to 0.03
**********************************************************************
Inverse coefficients for type J:
 
Temperature  -210.             0.           760.
  Range:        0.           760.          1200.
 
  Voltage   -8.095          0.000         42.919
  Range:     0.000         42.919         69.553
 
         0.0000000E+00  0.000000E+00 -3.11358187E+03
         1.9528268E+01  1.978425E+01  3.00543684E+02
        -1.2286185E+00 -2.001204E-01 -9.94773230E+00
        -1.0752178E+00  1.036969E-02  1.70276630E-01
        -5.9086933E-01 -2.549687E-04 -1.43033468E-03
        -1.7256713E-01  3.585153E-06  4.73886084E-06
        -2.8131513E-02 -5.344285E-08  0.00000000E+00
        -2.3963370E-03  5.099890E-10  0.00000000E+00
        -8.3823321E-05  0.000000E+00  0.00000000E+00
 
  Error      -0.05          -0.04          -0.04
  Range:      0.03           0.04           0.03
*/

#ifdef THERMOCOUPLE_TYPE_J
	double thermocoupleCoefficients [3][10] = {{0.0,19.528268,-1.2286185,-1.0752178,-0.59086933,-0.17256713,-0.028131513,-0.0023963370,-0.000083823321,0.0}, // Range: -8.095 to 0.0 mV
											 {0.0,19.78425,-0.2001204,0.01036969,-0.0002549687,0.000003585153,-0.00000005344285,0.0000000005099890,0.0,0.0}, // Range: 0.0 to 42.919 mV
											 {-3113.58187,300.543684,-9.94773230,0.170276630,-0.00143033468,0.00000473886084,0.0,0.0,0.0,0.0},  // Range: 42.919 to 69.553 mV
											};  
	double rangeThermocouple [4] = {-8.095,42.919,60.000,64.000}; // NOTA: el valor 64.00 es el maximo valor de voltaje entrada al adc en mV
																  // El valor 60.00 es el maximo valor de voltaje entrada arbitrareo se elige para el proyecto
#endif

/*
 * Convierte el voltaje proporcionado en el argumento
 * en temperatura correspondiente al tipo de termopar 
 * previamente definido en su programa:
 *
 * // Defina el typo de termopar que utilice
 * #define THERMOCOUPLE_TYPE_K
 * #include "Thermocouple.h" * 
 * 
 * Requerimentos:
 * 
 * El parametro requerido es la entrada de voltaje
 * en milivolts
 * 
 * La funcion devuelve la temperatura en °C considerando
 * que la punta fria se encuentra a 0 °C. Aun se requiere
 * compensar la temperatura de la punta fria, por lo general
 * la compensacion consiste en sumar la temperatura en la que 
 * se encuentra el bloque de terminales donde se conecta 
 * la entrada del termopar.
 * 
 */
double convertInputThermocoupleToTemperature(double InputVoltage){

  uint8_t index = 0;    // Determina el rango del voltaje de entrada y 
					    // determina la tabla de coeficientes a utilizar
  uint8_t i = 0;        // Selecciona en la suma el numero de coeficiente
  double t_90 = 0.0;    // Temperatura remota del termopar con punta fria a 0° C
  double E_exp_i = 1.0; // Representacion de E^i (pow (E, i))
	  
  // positive temperatures range
  if ((InputVoltage <= rangeThermocouple [2]) && (InputVoltage >= 0)){ 
    index = 1; // 1st Range
	while (InputVoltage > rangeThermocouple [index]){
	  index++; // 2nd or 3rd range
    }
  // upper range exceeded!
  } else if ((InputVoltage > rangeThermocouple [2]) && (InputVoltage < rangeThermocouple [3])) {
    return 1500.0; // voltaje del termopar por arriba del rango de operacion o abierto
  // lower range exceeded!
  } else if ((InputVoltage < rangeThermocouple [0]) && (InputVoltage >= rangeThermocouple [3])){
	  return -273.0; // voltaje del termopar por debajo del rango de operacion
  // negative temperatures range
  } else {
	index = 0;  // Rango de trabajo del termopar en temperaturas negativas
  }
  
  // t_90 = d_0 + d_1*E + d_2*E^2 + ... + d_9*E^9
  while (i < 10){
	t_90 += thermocoupleCoefficients [index][i] * E_exp_i;
	i++;
	E_exp_i *= InputVoltage; // E_exp_i = pow (InputVoltage, i)
  }
  
  return t_90;
}

#endif





