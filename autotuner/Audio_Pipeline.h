#pragma once

// I2S channel inits
void MIC_INIT();
void audio_init(float gain);
void update_gain(float gain);

// RTOS queues
void queueSetup();


// the pipeline INPUT -> HPS -> OUTPUT
void audioinputTask(void * pvParams);
void hpsprocessTask(void * pvParams);
void audiooutputTask(void * pvParams);
