/*
* Source code for Smart Buck step-down voltage converter with power monitoring functionality using the Texas Instruments INA219 Zero-Drift current/power monitor.
* Input power to the physical circuit is intended to be an 18-22VDC lithium-ion battery source, while the output is 12VDC regulated up to 3A of current achieved by
* utilizing the Texas Instruments LM2596 SIMPLE SWITCHER 3A Step-Down Voltage Regulator.
*
* https://www.ti.com/lit/ds/symlink/ina219.pdf
* https://www.ti.com/lit/ds/symlink/lm2596.pdf
*
* Dependencies:
*   * Arduino Wire library
*   * INA219 library by DeCristofaro John, Jukka-Pekka Sarjanen, gandy92, Flavius Bindea, Robert Wolff
*
* This code was developed and tested to run on the Microchip SAMD21E17A ARM Cortex M0 microcontroller.
*
* March 2023 by Fabian Butkovich
*/ 

#include <Wire.h>
#include <INA219.h>

// Create instance for INA219 library class
INA219 PWR_IN_IMON;
// INA219 sensor for power output has different I2C address
INA219 PWR_OUT_IMON(INA219::I2C_ADDR_41);

#define R_SHUNT 0.1
#define V_SHUNT_MAX 0.3
#define V_BUS_IN_MAX 32
#define V_BUS_OUT_MAX 16
#define I_MAX_EXPECTED 3
// How many samples to take and average for current measurments, different types of loads (resistive, capacitive etc.) on
// the regulated output can cause the over-current function to false trigger if for example there is a high startup current but then the 
// value settles after a short period of time. Averaging N shunt current readings over a period of time can prevent this.
#define NUMSAMPLES 12
// SAMD21E17A PA02
#define BUCK_ENABLE 2 

// Long positive integer for storing interval to wait before reading sensors, units are in milliseconds
unsigned long readinterval = 100;
unsigned long printinterval = 10000;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;

float I_IN_AVERAGE_PREVIOUS;
float I_IN_AVERAGE;
float I_OUT_AVERAGE_PREVIOUS;
float I_OUT_AVERAGE;

enum unit_type {
  voltage,
  current,
  power
};

void setup()
{
  Serial.begin(115200);

  // Initially enable LM2596 simple switcher
  pinMode(BUCK_ENABLE, OUTPUT);
  digitalWrite(BUCK_ENABLE, LOW);
  
  // Begin I2C communication with and configure 1st INA219 used for input power source
  PWR_IN_IMON.begin();
  // Configure input for 26V max voltage and 3.2A max current @ 0.8mA resolution, averaging of 8 samples per sensor reading
  PWR_IN_IMON.configure(INA219::RANGE_32V, INA219::GAIN_8_320MV, INA219::ADC_8SAMP, INA219::ADC_8SAMP, INA219::CONT_SH_BUS);
  PWR_IN_IMON.calibrate(R_SHUNT, V_SHUNT_MAX, V_BUS_IN_MAX, I_MAX_EXPECTED);
  
  // Configure output for 16V max voltage and 3.2A max current @ 0.8mA resolution
  PWR_OUT_IMON.begin();
  PWR_OUT_IMON.configure(INA219::RANGE_16V, INA219::GAIN_8_320MV, INA219::ADC_8SAMP, INA219::ADC_8SAMP, INA219::CONT_SH_BUS);
  PWR_OUT_IMON.calibrate(R_SHUNT, V_SHUNT_MAX, V_BUS_OUT_MAX, I_MAX_EXPECTED);
}

void loop()
{
  // Use the millis() function which returns the number of milliseconds elapsed since the start of the code loop, this is used
  // to calculate how long to wait before polling the power monitoring sensors
  unsigned long currentMillis = millis();

  if (Serial.available() > 0)
  {
    // Read char array input from serial monitor and store it within a string datatype
    String input = Serial.readStringUntil('\n');
    if (input == "1") 
    {
      digitalWrite(BUCK_ENABLE, LOW);
    }
    else if (input == "0")
    {
      digitalWrite(BUCK_ENABLE, HIGH);
    }
  }

  // Read and average current data at 10Hz (every 100ms)
  if (currentMillis - previousMillis >= readinterval)
  {
    // Restart the non-blocking millis() timer
    previousMillis = currentMillis;

    // Detect under-voltage and over-current events
    if (over_current_event() || under_voltage_event()) 
    {
      digitalWrite(BUCK_ENABLE, HIGH);
    }

    I_IN_AVERAGE_PREVIOUS = I_IN_AVERAGE;
    I_OUT_AVERAGE_PREVIOUS = I_OUT_AVERAGE;
  }

  // Check to see if the INA219 sensors should be read by comparing the delta between currentMillis and previousMillis
  if (currentMillis - previousMillis2 >= printinterval)
  {
    // Restart the non-blocking millis() timer
    previousMillis2 = currentMillis;

    // Retrieve INA219 measurments by calling class object methods within the INA219 library, print to serial port
    Serial.print(build_unit_string(voltage, PWR_IN_IMON.busVoltage(), PWR_OUT_IMON.busVoltage()) + '\n');

    // Dynamically change units displayed between A/mA depending on whether the measured current is less than or greater than a single unit
    Serial.print(build_unit_string(current, I_IN_AVERAGE, I_OUT_AVERAGE) + '\n');

    // Dynamically change units displayed between W/mW depending on whether the measured power is less than or greater than a single unit
    Serial.println(build_unit_string(power, PWR_IN_IMON.busPower(), PWR_OUT_IMON.busPower()) + '\n');
  }
}

bool over_current_event()
{
  // Retrieve INA219 measurments by calling class object methods within the INA219 library
  I_IN_AVERAGE = PWR_IN_IMON.shuntCurrent();
  I_OUT_AVERAGE = PWR_OUT_IMON.shuntCurrent();

  // Check for large delta in input/output current samples over time, which influences how frequently serial data should be printed to be logged
  if (abs(I_IN_AVERAGE - I_IN_AVERAGE_PREVIOUS) > 0.01 || abs(I_OUT_AVERAGE - I_OUT_AVERAGE_PREVIOUS) > 0.01)
  {
    printinterval = 1000;
  }
  else 
  {
    printinterval = 10000;
  }

  // If either the input current or output current exceed the amount specified by I_MAX_EXPECTED, return true
  if (I_IN_AVERAGE > I_MAX_EXPECTED || I_OUT_AVERAGE > I_MAX_EXPECTED)
  {
    return true;
  }
  else
  {
    return false;
  }
}
 
bool under_voltage_event()
{
  // If input voltage falls below 16VDC, disable regulated output to prevent over-draining of the battery
  if (PWR_IN_IMON.busVoltage() < 16.0)
  {
    return true;
  }
  else
  {
    return false;
  } 
}

/* This functions prints a formatted human-readable string to the serial port specified in the setup
* the string that is built is dynamically changed between units of voltage, current or power dependent upon the enumerator passed to this function
* @param electrical_unit 
* @param UNIT_IN_MEASURMENT input voltage, current or power
* @param UNIT_OUT_MEASURMENT output voltage, current or power */
String build_unit_string(unit_type electrical_unit, float UNIT_IN_MEASUREMENT, float UNIT_OUT_MEASUREMENT)
{
  bool UNIT_IN_RANGE = UNIT_IN_MEASUREMENT >= 1.0 ? true : false;
  bool UNIT_OUT_RANGE = UNIT_OUT_MEASUREMENT >= 1.0 ? true : false;
  // Size of char array which stores floating point number is set large enough to store 4 digits e.g. XXX.X
  char UNIT_IN_STRING[6], UNIT_OUT_STRING[6]; 
  char UNIT_TEMP_STRING[32];
  char UNIT_STRING[32];
  UNIT_TEMP_STRING[0] = '\0';
  // Convert floating point value to char array
  dtostrf(UNIT_IN_RANGE ? UNIT_IN_MEASUREMENT : UNIT_IN_MEASUREMENT * 1000.0, UNIT_IN_RANGE ? 3 : 4, UNIT_IN_RANGE ? 2 : 1, UNIT_IN_STRING);
  dtostrf(UNIT_OUT_RANGE ? UNIT_OUT_MEASUREMENT : UNIT_OUT_MEASUREMENT * 1000.0, UNIT_OUT_RANGE ? 3 : 4, UNIT_OUT_RANGE ? 2 : 1, UNIT_OUT_STRING);
  // Char array must be populated in two parts, first by using strcpy to store the first half, then concatenate the second
  // half using strcat()
  switch(electrical_unit)
  {
    case voltage:
      strcpy(UNIT_TEMP_STRING, UNIT_IN_RANGE ? "V_IN/V_OUT:   %sV" : "V_IN/V_OUT:   %smV");
      strcat(UNIT_TEMP_STRING, UNIT_OUT_RANGE ? " / %sV" : " / %smV");
      break;
    case current:
      strcpy(UNIT_TEMP_STRING, UNIT_IN_RANGE ? "I_IN/I_OUT:   %sA" : "I_IN/I_OUT:   %smA");
      strcat(UNIT_TEMP_STRING, UNIT_OUT_RANGE ? " / %sA" : " / %smA");
      break;
    case power:
      strcpy(UNIT_TEMP_STRING, UNIT_IN_RANGE ? "P_IN/P_OUT:   %sW" : "P_IN/P_OUT:   %smW");
      strcat(UNIT_TEMP_STRING, UNIT_OUT_RANGE ? " / %sW" : " / %smW");
      break;
    default:
      break;
  }
  // Insert floating point number strings into final unit string using the %s specifier
  snprintf(UNIT_STRING, sizeof(UNIT_STRING), UNIT_TEMP_STRING, UNIT_IN_STRING, UNIT_OUT_STRING);
  return UNIT_STRING;
}

// Must explicitly define dtostrf function as it is unsupported for SAMD core in the Wire.h library
char *dtostrf (double val, signed char width, unsigned char prec, char *sout) 
{
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}
