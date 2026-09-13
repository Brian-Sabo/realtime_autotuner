#include "constants.h"
#include "GPIO_func.h"
#include "Audio_Pipeline.h"
#include "extra.h"


// RTOS - for dual core
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include <SPI.h>
#include <SD.h>


void setup() {
  Serial.begin(115200);
  SPI.begin(SCK, MISO, MOSI);

  // SD CARD  
  if (!SD.begin(SD_CS)){
    Serial.println("SD not connected");
    return;
  } else {
    Serial.println("SD init complete");
  }
  
  // LCD init
  LCD_init();
  BTN_INIT();

  //setting up the I2S channels for audio input and output
  MIC_INIT();
  audio_init(0.8);   //gain of 0.8 to start
  queueSetup();

  // creating the audio input and output task on core 0
  xTaskCreatePinnedToCore(audioinputTask,"audio_input",8192,NULL,5,NULL,0);
  xTaskCreatePinnedToCore(audiooutputTask,"audio_output",8192,NULL,4,NULL,1);

  // creating the HPS task on core 1
  xTaskCreatePinnedToCore(hpsprocessTask,"processing",16384,NULL,3,NULL,0);
}

// arduino set to run on core 1
uint8_t shift = 0x00;
static char row2text[17];
void loop() {
    if (!readPin(SHIFT_BTN)){
    shift = (shift + 1) % 12; // shift up by a semitone wrap around after 12
    snprintf(row2text,sizeof(row2text),"%u       ON", shift);
    LCD_MSG(row2text);
  }

  if (!readPin(MUTE_BTN)){
    process_btns(shift, true);
  }
  delay(200); //checking btn press at 5hz to keep core 1 free for audio
}
