#include "AUDIO_INIT.h"
#include "GPIO_func.h"
#include "constants.h"
#include <driver/i2s_std.h>

i2s_chan_handle_t rx_handle;
i2s_chan_handle_t tx_handle;

//32-bit mono samples
static int32_t buf[FRAME_COUNT];


void captureaudioTask(void *pvParams) {
    size_t bytes_read  = 0;
    size_t bytes_written = 0;

    while (true) {
        // audio in INMP441 32bit mono
        esp_err_t err = i2s_channel_read(rx_handle,buf,sizeof(buf),&bytes_read,portMAX_DELAY);

        if (err != ESP_OK) {
            continue; // add error handling later
        }

        int sample_count = bytes_read / sizeof(int32_t);
        for (int i = 0; i < sample_count; i++){
            int32_t sample = buf[i] >> 8;  // shifting the 8 bits down since mic is only 24 bits
            
            // software gain for mic
            int64_t boosted = (int64_t)sample * 2048;  //256 // to make the mic input hearable 2048
            if (boosted > INT32_MAX){
                boosted = INT32_MAX;
            }
            else if (boosted < -INT32_MAX){
                boosted = -INT32_MAX;
            }

            buf[i] = (int32_t)boosted;
        }

        // audio out MAX98357 32bit mono
        i2s_channel_write(tx_handle,buf,bytes_read,&bytes_written,portMAX_DELAY);
    }
}



void setup() {
  MIC_INIT();
  SPEAKER_INIT();
  xTaskCreatePinnedToCore(captureaudioTask,"audio_loopback",8192,NULL,5,NULL,1);
  }

void loop() {
  
}
