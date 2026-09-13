#include <AudioFileSourceSD.h>
#include <AudioGeneratorWAV.h>
#include <AudioOutputI2S.h>
#include <SPI.h>
#include "constants.h"
#include <freertos/queue.h>
#include <arduinoFFT.h>
#include <driver/i2s_std.h>
#include <SD.h>

#define HARMOIC_COUNT 3

i2s_chan_handle_t rx_handle;

int32_t BUF[5][SAMPLECOUNT];
uint8_t buf_index = 0;

uint8_t lastnote = -1;

size_t byte_count = 0;

QueueHandle_t audioQueue = nullptr;
QueueHandle_t noteQueue = nullptr;

double Real[SAMPLECOUNT];
double Imag[SAMPLECOUNT];

ArduinoFFT<double> FFT = ArduinoFFT<double>(Real, Imag, SAMPLECOUNT, SAMPLERATE); //FFT object from FFT library

// example I2S RX setup from ESPRESSIF docs
void MIC_INIT(){
  i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
  // DMA BUFFERS not part of exampled added to hold mic data in DMA
  chan_cfg.dma_desc_num = 8; // number of DMA descriptors
  chan_cfg.dma_frame_num = 256; // frames per descriptor

  // RX channel I2S_NUM_0 so that way the audio amp out can go a I2S_NUM_1
  i2s_new_channel(&chan_cfg, NULL, &rx_handle);

  i2s_std_config_t std_cfg = {
      .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(SAMPLERATE), // 16khz sample
      .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_MONO),
      .gpio_cfg = {
          .mclk = I2S_GPIO_UNUSED,
          .bclk = SCK_MIC,
          .ws = WS_MIC,
          .dout = I2S_GPIO_UNUSED,
          .din = SD_MIC,
          .invert_flags = {
              .mclk_inv = false,
              .bclk_inv = false,
              .ws_inv = false,
          },
      },
  };

  i2s_channel_init_std_mode(rx_handle, &std_cfg);
  i2s_channel_enable(rx_handle);
}

// using AUDIO OUT PIPELINE FROM BOPIT: ptrs for audio files, wav object, and I2S stream out
AudioOutputI2S*    I2S = nullptr;
AudioFileSourceSD* file = nullptr;
AudioGeneratorWAV* wav = nullptr;

void audio_init(float gain){
  I2S = new AudioOutputI2S(1);  //set to I2S channel 1 mic on I2S channel 0
  I2S->SetPinout(BCLK, LRC, DOUT);
  I2S->SetGain(gain);   // 0.0 to 1.0 volume
}

// function to update gain to mute
void update_gain(float gain){
  I2S->SetGain(gain);
}


// flag for when audio is done playing to start playing the next note
bool audio_done = true; // start with true so that the first note can be played immediately when the program starts

//output audio SD card reading
void start_wav(const char * path){
  //override what is playing
  if (wav && wav->isRunning()){
    wav->stop();
    delete wav;
    delete file;
    wav = nullptr;
    file = nullptr;
  }

  file = new AudioFileSourceSD(path);
  wav = new AudioGeneratorWAV();
  wav->begin(file, I2S);
}


void feed_audio(){
  if (wav && wav->isRunning()){   // checks if a wav is playing
    if (!wav->loop()){        // sends next payload of audio data to I2S
      audio_done = true;     // when done !wav->loop() returns false
      wav->stop();            // when done !wav->loop() returns false
    }                         // after the wav is done stop the wav
  }
}

// to map freqs to their corresponding notes
std::map<int, int> freq_to_note_map = {
    {130, C3},
    {138, CS3},
    {146, D3},
    {155, DS3},
    {164, E3},
    {174, F3},
    {185, FS3},
    {196, G3},
    {207, GS3},
    {220, A3},
    {233, AS3},
    {246, B3},
    {261, C4},
    {277, CS4},
    {293, D4},
    {311, DS4},
    {329, E4},
    {349, F4},
    {369, FS4},
    {391, G4},
    {415, GS4},
    {440, A4},
    {466, AS4},
    {493, B4},
    {523, C5},
    {554, CS5},
    {587, D5},
    {622, DS5},
    {659, E5},
    {698, F5},
    {740, FS5},
    {784, G5},
    {831, GS5},
    {880, A5},
    {932, AS5},
    {987, B5},
    {1046, C6}
};


// function to map the freq to the cloest note, faster than a fore loop
//used CPP:refrence std::map
int corrected_note(double fund_freq){
  int freq = (int)fund_freq;    // converting to int then passing as key

  std::map<int, int>::const_iterator iterator = freq_to_note_map.lower_bound(freq);

  if (iterator == freq_to_note_map.begin()){  //checking if below C3, return C3
    return iterator->second;  //returns the value the note INT ex 440 -> A4
  }

  if (iterator == freq_to_note_map.end()){ // checking if above C6 then C6
    return std::prev(iterator)->second;
  }

  std::map<int, int>::const_iterator prev_iterator = std::prev(iterator);  //checking between the notes in the map

  if (freq - prev_iterator->first < iterator->first - freq){  //if diff in freq in lower note, is less than higher note diff, lower note is closer to input
    return ((prev_iterator->second + shift) % 37); // return the lower note + shift
  }

  return ((iterator->second + shift) % 37); //return the upper note
}

void queueSetup(){
  // making the queue to pass the audio samples to the FFT engine 5 sets of audio 2048 samples
  audioQueue = xQueueCreate(5,sizeof(int32_t*)); // using pointers
  noteQueue = xQueueCreate(5,sizeof(int));   // will hold the next 20 notes to be played, each note will connect to a wav file on the SD card

  // init check
  if (!audioQueue || !noteQueue){
    Serial.println("Queue failed to Init");
    while(1);
  }
}

void audioinputTask(void * pvParams){
  while (true){
    int32_t * bufptr = BUF[buf_index];

    esp_err_t err = i2s_channel_read(rx_handle, bufptr, sizeof(BUF[0]), &byte_count, portMAX_DELAY);
    if (err != ESP_OK){
      continue; // add error pretection later
      }

  int numSamples = byte_count/ 4;   //as int32_t is 4 bytes
  for (int i = 0; i < numSamples; i++){
    bufptr[i] >>= 8; // shifting down by 8 bits to get rid of the extra 8 bits
  }

  // putting the audio sample on the queue based on example, gives 10 ticks to pass audio samples if queue is full
  xQueueSend(audioQueue, &bufptr, pdMS_TO_TICKS(10));  // updating to pdMS to ticks
  buf_index = (buf_index + 1) % 5;
  }
}


void hpsprocessTask(void * pvParams){
    while (true){
        int note;
        float fund_freq;
        int32_t * bufptr = nullptr;
        int maxVal = (SAMPLECOUNT / 2)/HARMOIC_COUNT;  //only sreaching 3 harmoics
        double product = 1.0;
        int maxBin = 0;
        if (xQueueReceive(audioQueue, &bufptr, portMAX_DELAY) == pdTRUE){
            // copying over the buffer to the real part of the FFT
            for (int i = 0; i < SAMPLECOUNT; i++){
                Real[i] = (double) bufptr[i] / 2147483648.0; // normalizing the audio samples to be between -1 and 1 for the FFT
                Imag[i] = 0.0;
            }

            // FFT on the audio samples in the buffer example from arduinoFFT library
            FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);
            FFT.compute(FFTDirection::Forward);  // find FFT
            FFT.complexToMagnitude(); //Compute magnitudes with abs from FFT lib
            // find the major peak which is the fund freq that is built into the FFT lib convert the fund freq to a note using map
            // fund_freq = FFT.majorPeak();
            // fund_freq *= 2;
            for (int h = 1; h < maxVal; h++){   //to avoid the dc offset
              for (int i = 1; i <= HARMOIC_COUNT; i++){
                product *= Real[h* i];
              }
              if (product > maxVal) {
                maxVal = product;
                maxBin = h;
              }
            }
            fund_freq = (maxBin * SAMPLERATE) / SAMPLECOUNT;

            Serial.print("Fundamental Frequency: ");
            Serial.println(fund_freq);
            note = corrected_note(fund_freq);
            Serial.print("Note: ");
            Serial.println(note);
            // put the note on the note queue to be played by the audio output task
            if (note != lastnote){   //don't put duplicate notes
              xQueueSend(noteQueue, &note, pdMS_TO_TICKS(10));
              lastnote = note;
            }
            
        }
    }
}

void audiooutputTask(void * pvParams){
    while (true){
        int note;
        if (xQueueReceive(noteQueue, &note, portMAX_DELAY) == pdTRUE){
            // play the note by reading the corresponding wav file
            if (1){   //allows audio to be iterrupted, if not the same note
                audio_done = false; // reset the flag for the next note
                Serial.print("Playing note: ");
                Serial.println(note);

                switch(note){
                    case C3: start_wav("/C3.wav"); break;
                    case CS3: start_wav("/CS3.wav"); break;
                    case D3: start_wav("/D3.wav"); break;
                    case DS3: start_wav("/DS3.wav"); break;
                    case E3: start_wav("/E3.wav"); break;
                    case F3: start_wav("/F3.wav"); break;
                    case FS3:  start_wav("/FS3.wav"); break;
                    case G3: start_wav("/G3.wav"); break;
                    case GS3: start_wav("/GS3.wav");break;
                    case A3: start_wav("/A3.wav"); break;
                    case AS3: start_wav("/AS3.wav"); break;
                    case B3: start_wav("/B3.wav"); break;
                    case C4: start_wav("/C4.wav"); break;
                    case CS4: start_wav("/CS4.wav"); break;
                    case D4: start_wav("/D4.wav"); break;
                    case DS4: start_wav("/DS4.wav"); break;
                    case E4: start_wav("/E4.wav"); break;
                    case F4: start_wav("/F4.wav"); break;
                    case FS4: start_wav("/FS4.wav"); break;
                    case G4: start_wav("/G4.wav"); break;
                    case GS4: start_wav("/GS4.wav"); break;
                    case A4: start_wav("/A4.wav"); break;
                    case AS4: start_wav("/AS4.wav"); break;
                    case B4: start_wav("/B4.wav"); break;
                    case C5: start_wav("/C5.wav"); break;
                    case CS5: start_wav("/CS5.wav"); break;
                    case D5: start_wav("/D5.wav"); break;
                    case DS5: start_wav("/DS5.wav"); break;
                    case E5: start_wav("/E5.wav"); break;
                    case F5: start_wav("/F5.wav"); break;
                    case FS5: start_wav("/FS5.wav"); break;
                    case G5: start_wav("/G5.wav"); break;
                    case GS5: start_wav("/GS5.wav"); break;
                    case A5: start_wav("/A5.wav"); break;
                    case AS5: start_wav("/AS5.wav"); break;
                    case B5: start_wav("/B5.wav"); break;
                    case C6: start_wav("/C6.wav"); break;
                }
                feed_audio();
                vTaskDelay(pdMS_TO_TICKS(10)); //feeds the WAV out every 10ms
            }
            feed_audio();
            vTaskDelay(pdMS_TO_TICKS(10)); // keep feeding the audio every 10 ms to make sure it plays all the way through before the next note is played
        }
    }
}