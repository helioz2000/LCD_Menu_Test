/*
 * LCD Menu Test
 */

#include <Wire.h> 
#include "LiquidCrystal_PCF8574.h"
#include "LCD_Menu.h"

#define SERIAL_BAUDRATE 19200 // For console interface

#define LCD_I2C_ADDRESS 0x3F
#define LCD_ROWS 4
#define LCD_COLUMNS 20    // Characters per line

#define BUTTON1 A1
#define BUTTON2 A2
#define BUTTON3 A3

#define MENU_ITEM_COUNT 8
menu_item menucontents[MENU_ITEM_COUNT] = {
  {"Menu Line 1", 12, 0, 1},
  {"Menu Line 2", 12, 1, 5},
  {"Menu Line 3", 12, 1, 10},
  {"Menu Line 4", 12, 2, 20},
  {"Menu Line 5", 12, 3, 30},
  {"Menu Line 6", 12, 4, 40},
  {"Menu Line 7", 12, 5, 50},
  {"Menu Line 8", 12, 6, 60}
};

LiquidCrystal_PCF8574 lcd(LCD_I2C_ADDRESS);  // set the LCD address 
LCD_Menu menu(LCD_ROWS, LCD_COLUMNS);
bool button1shadow, button2shadow, button3shadow;

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

  lcd.begin(LCD_COLUMNS, LCD_ROWS); // initialize the lcd
  lcd.setBacklight(1);
  lcd.home();
  lcd.clear();  
}

void setup_serial() {
  // Open serial communications and wait for port to open:
  Serial.begin(SERIAL_BAUDRATE);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  debug("LCD Menu Test\n");
}

void setup_menu() {
  menu.setMenuItems(menucontents, MENU_ITEM_COUNT);
}

void setup() {
  setup_serial();
  setup_menu();
  setup_LCD();

  button1shadow = false;
  button2shadow = false;
  button3shadow = false;
  
  menu.updateLCD();
}

void loop() {
  if(!digitalRead(BUTTON1)) {
    if (!button1shadow) {
      //debug("Button 1 activated\n");
      button1shadow = true;
      menu.moveMenu(true);
    }
  } else {
      button1shadow = false;
  }

  if(!digitalRead(BUTTON2)) {
    if (!button2shadow) {
      //debug("Button 2 activated\n");
      button2shadow = true;
      menu.moveMenu(false);
    }
  } else {
    button2shadow = false;
  }
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

