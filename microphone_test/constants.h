#pragma once
#include <driver/i2s_std.h>

// AUDIO OUT pins   had to use GPIO_NUM instead of default number for I2S configs
#define BCLK GPIO_NUM_16
#define LRC GPIO_NUM_17
#define DOUT GPIO_NUM_18
// AUDIO In pins
#define WS_MIC GPIO_NUM_9
#define SCK_MIC GPIO_NUM_10
#define SD_MIC GPIO_NUM_8

#define SAMPLERATE     44100   //
#define DMA_FRAME_NUM  256
#define DMA_DESC_NUM   8
#define FRAME_COUNT  (DMA_FRAME_NUM * DMA_DESC_NUM)  // 2048 frames per read

extern i2s_chan_handle_t rx_handle;
extern i2s_chan_handle_t tx_handle;


// AUDIO CONSTS  S = sharp RANGE
#define C3 0
#define CS3 1
#define D3 2
#define DS3 3
#define E3 4
#define F3 5
#define FS3 6
#define G3 7
#define GS3 8
#define A3 9
#define AS3 10
#define B3 11
#define C4 12
#define CS4 13
#define D4 14
#define DS4 15
#define E4 16
#define F4 17
#define FS4 18
#define G4 19
#define GS4 20
#define A4 21
#define AS4 22
#define B4 23
#define C5 24
#define CS5 25
#define D5 26
#define DS5 27
#define E5 28
#define F5 29
#define FS5 30
#define G5 31
#define GS5 32
#define A5 33
#define B5 34
#define C6 35
