#pragma once

void LCD_init();
void LCD_MSG(const char * text_row2);

void BTN_INIT();
void process_btns(uint8_t shift, bool mute);