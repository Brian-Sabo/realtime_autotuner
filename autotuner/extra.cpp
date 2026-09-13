#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include "constants.h"
#include "GPIO_func.h"

#include "Audio_Pipeline.h"

hd44780_I2Cexp lcd;

void LCD_init(){
  pinMode(SDA_LCD, OUTPUT);
  pinMode(SCL_LCD, OUTPUT);
  Wire.begin(SDA_LCD, SCL_LCD);  // SDA=48, SCL=45
  lcd.begin(16,2); //16x2 display
  lcd.print("SCALE    AUDIO");  // the header that will stay
  lcd.setCursor(0,1);
  lcd.print("0       ON");   // initial scale and audio status
}

void LCD_MSG(const char * text_row2){
  //clears row 2 and updates the score and password
  lcd.setCursor(0,1);
  lcd.print("                "); // clear row 2
  lcd.setCursor(0,1);
  lcd.print(text_row2);   // update row 2 with new message
}

void BTN_INIT(){
  pinMode(SHIFT_BTN, INPUT_PULLUP);
  pinMode(MUTE_BTN, INPUT_PULLUP);
}

static uint8_t muted = 0x00;
static char row2text[17];
void process_btns(uint8_t shift, bool mute){
    if (mute){
      muted ^= 0x01; // toggle
      if (muted == 0x01){
        update_gain(0.0); // mute

        snprintf(row2text,sizeof(row2text),"%u       MUTED", shift);
        LCD_MSG(row2text);
      } else {
        update_gain(0.8); // unmute

        snprintf(row2text,sizeof(row2text),"%u      ON", shift);
        LCD_MSG(row2text);
      }
    } 
}