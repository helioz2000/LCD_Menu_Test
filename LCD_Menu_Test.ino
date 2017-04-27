/*
 * LCD Menu Test
 */

#include <Wire.h> 
#include "LiquidCrystal_PCF8574.h"
#include "LCD_Menu.h"
#include "Encoder.h"

#define SERIAL_BAUDRATE 19200 // For console interface

#define LCD_I2C_ADDRESS 0x3F
#define LCD_ROWS 4
#define LCD_COLUMNS 20    // Characters per line

// Pin definitions
// Multifunction Board 
#define BUTTON1 A1
#define BUTTON2 A2
#define BUTTON3 A3
#define LED3 11
#define LED4 10
#define LED_ON LOW
#define LED_OFF HIGH

// Encoder
#define BUTTON4 14



#define BUTTON_DEBOUNCE_TIME 150UL         // ms
#define BUTTON_LONGPRESS_TIME 600UL       // ms

#define MENU_ITEM_COUNT 8
menu_item menucontents[MENU_ITEM_COUNT] = {
  {"Menu Line 1", 12, 0, 1},
  {"Menu Line 2", 13, 1, 5},
  {"Menu Line 3", 14, 1, 10},
  {"Menu Line 4", 15, 2, 20},
  {"Menu Line 5", 16, 3, 30},
  {"Menu Line 6", 15, 4, 40},
  {"Menu Line 7", 14, 5, 50},
  {"Menu Line 8", 13, 6, 60}
};

int values[MENU_ITEM_COUNT] = { 10, 50, 100, 300, 412, 1254, 9335, 834 };

LiquidCrystal_PCF8574 lcd(LCD_I2C_ADDRESS);  // set the LCD address 
LCD_Menu menu(LCD_ROWS, LCD_COLUMNS);
bool button1shadow, button2shadow, button3shadow;
unsigned long button3time;
bool buttonDebounce;

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
  lcd.noCursor();
  lcd.noBlink();
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

  pinMode(BUTTON4, INPUT);
  digitalWrite(BUTTON4, HIGH);

  beginEncoder();

  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  digitalWrite(LED3, LED_OFF);
  digitalWrite(LED4, LED_OFF);
  
  menu.setCallbackValueChange(menuItemValueChange);
  menu.setCallbackValueGet(menuItemValueGet);
  menu.updateLCD();

}

void menuItemValueChange(int change) {
  int selectedItem = menu.getSelectedMenuItem();
  if (selectedItem < 1) return;   // noting selected
  selectedItem--;   // convert to zero based index
  switch (change) {
    case 1:   // up
      values[selectedItem]++;
      break;
    case 2:   // down
      values[selectedItem]--;
      break;
    default:
      break;
  }
}

int menuItemValueGet(int menuItem) {
  return (values[menuItem-1]);
}

uint8_t delta;

void loop() {
  
  if(!digitalRead(BUTTON1)) {
    if (!button1shadow) {
      //debug("Button 1 activated\n");
      button1shadow = true;
      menu.uiDown();
      buttonDebounce = true;
    }
  } else {
      button1shadow = false;
  }

  if(!digitalRead(BUTTON2)) {
    if (!button2shadow) {
      //debug("Button 2 activated\n");
      button2shadow = true;
      menu.uiUp();
      buttonDebounce = true;
    }
  } else {
    button2shadow = false;
  }

  if(!digitalRead(BUTTON4)) {
    if (!button3shadow) {
      //debug("Button 3 activated\n");
      button3shadow = true;
      button3time = millis();
      
    }
  } else {
    if (button3shadow) {
      if ((millis() - button3time) < BUTTON_LONGPRESS_TIME) {
        menu.uiSelect(false);
      } else {
        menu.uiSelect(true);  // long press
      }
    }
    button3shadow = false;
  }

  digitalWrite(LED3, digitalRead(ENCODER_A));
  digitalWrite(LED4, digitalRead(ENCODER_B));

  if (buttonDebounce) 
    {
      delay(BUTTON_DEBOUNCE_TIME);
      buttonDebounce=false;
    }

  if (updateEncoders(&delta)) {
    debug("delta: %d\n",delta);
  }

  delay(1000);
  Serial.println(test_l);
  /*
  if (valueHasChanged) {
    menu.updateSelectedMenuItemValue();
    valueHasChanged = false;
  }
  */
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

