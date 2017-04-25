/*
 * LCD Menu Test
 */

#include <Wire.h> 
#include "LiquidCrystal_PCF8574.h"
#include "LCD_Menu.h"

#define SERIAL_BAUDRATE 19200 // For console interface

#define LCD_I2C_ADDRESS 0x3F
#define LCD_LINES 4
#define LCD_CPL 20    // Characters per line

LiquidCrystal_PCF8574 lcd(LCD_I2C_ADDRESS);  // set the LCD address 

// print debug output on console interface
void debug(char *sFmt, ...)
{
  char acTmp[128];       // place holder for sprintf output
  va_list args;          // args variable to hold the list of parameters
  va_start(args, sFmt);  // mandatory call to initilase args 

  vsprintf(acTmp, sFmt, args);
  Serial.print(acTmp);
  // mandatory tidy up
  va_end(args);
  return;
}

void setup_LCD() {
  int error;
  // Check for presence of the LCD at the I2C address 
  Wire.begin();
  Wire.beginTransmission(LCD_I2C_ADDRESS);
  error = Wire.endTransmission();

  if (error != 0) {
    debug("Error: <%d> - LCD not found at I2C address 0x%X\n", error, LCD_I2C_ADDRESS );
  } // if

  lcd.begin(LCD_CPL, LCD_LINES); // initialize the lcd
  lcd.setBacklight(1);
  lcd.home();
  lcd.clear();
  lcd.print("Modbus: ");
  
}

void setup_serial() {
  // Open serial communications and wait for port to open:
  Serial.begin(SERIAL_BAUDRATE);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  debug("LCD Menu Test\n");
}


void setup() {
  setup_serial();
  setup_LCD();
}

void loop() {
  // put your main code here, to run repeatedly:

}


void lcdPrint(char *sFmt, ...)
{
  char acTmp[128];       // place holder for sprintf output
  va_list args;          // args variable to hold the list of parameters
  va_start(args, sFmt);  // mandatory call to initilase args 

  vsprintf(acTmp, sFmt, args);
  lcd.print(acTmp);
  // mandatory tidy up
  va_end(args);
  return;
}

