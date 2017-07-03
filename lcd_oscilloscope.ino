/*
Primitive Oscillator

Code originally created 07 July 2017
by Ramast

This code is under GPLv3 License

https://github.com/ramast/arduino-lcd-oscilloscope
*/

// include the LCD library
#include <LiquidCrystal.h>

//INPUT_PIN: Define which arduino pin should the code probe for input
#define INPUT_PIN A0
//MAX_REPEATITION: Maximum number of times certain value can be repeated before re-print it on scren
//Recommended values are: 0 for no repeation, 9, 19 or 100 
#define MAX_REPEATITION 19
//For how long should the LCD keep it's current reading before refreshing, default is 1000 (one second)
#define LCD_REFRESH_DELAY 1000
// Please specify here the correct pins for your LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//This array contain the characters that would represent different voltage levels (0 to 9)
//the final character '*' is used sometimes to represent very high repeatition
const byte LEVELS[] = {' ', 0, '_', 1, 2, 3, 4, 5, 6, 255, '*'};


#if MAX_REPEATITION < 10
void inline print_rep(int unsigned repeation_count) {
    lcd.print(repeation_count);
}

#elif MAX_REPEATITION < 20
void print_rep(int unsigned repeation_count) {
    if (repeation_count < 10) {
      lcd.print(repeation_count);
    } else {
      lcd.write(LEVELS[(repeation_count - 9)]);
    }
}

#else
void print_rep(int unsigned repeation_count) {
    if (repeation_count < 10) {
      lcd.print(repeation_count);
    } else if (repeation_count < 99) {
      lcd.write(LEVELS[(repeation_count / 10)]);
    } else {
       lcd.write(LEVELS[10]);        
    }
}
#endif

void setup() {
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    
    //create custom characters for voltage levels
    //If you are using different characters for LEVELS constant, you won't need this code
    byte bar[] = {0, 0, 0, 0, 0, 0, 0, 21};
    lcd.createChar(0, bar);
    bar[7] = 31;
    for (int i=1; i < 7; i++) {
        bar[7 - i] = 31;
        lcd.createChar(i, bar);
    }
    //--- end of custom characters creation
    delay(100);
    lcd.clear();
}

int inline readInput() {
    // Get reading on grade from 0 to 9
    return (analogRead(INPUT_PIN) / 103.0);
}

void loop() {
    int unsigned repeatition;
    byte unsigned last_value;
    byte unsigned value;

    byte unsigned levels[20];
    byte unsigned repeatitions[20];

    byte i = 0;
    
    value = last_value = readInput();
    for (i = 0; i < 20; i++) {
       repeatition = 0;
       while (value == last_value) {
           value = readInput();
           if (++repeatition == MAX_REPEATITION) break; 
        }
        levels[i] = LEVELS[last_value];
        repeatitions[i] = repeatition;
        last_value = value;
    }
    //Try to start with the value that has highest repeatition/voltage first to keep screen output a bit consistent
    //we ignore first value in the result because it's usually inaccurate
    long int unsigned best_value = 0;
    byte best_index;
    for (i=1; i < 5; i++) {
      if (best_value < (repeatitions[i] + levels[i] * 10)) {
          best_value = repeatitions[i] + levels[i] * 10;
          best_index = i;
      }
    }
    lcd.home();
    lcd.noDisplay();
    for (i = 0; i < 16; i++) {
        lcd.write(levels[i + best_index]);
    }
    lcd.setCursor(0, 1);
    for (i = 0; i < 16; i++) {
        print_rep(repeatitions[i + best_index]);
    }
    lcd.display();
    delay(LCD_REFRESH_DELAY);
}

