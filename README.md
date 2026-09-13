# realtime_autotuner
A realtime_autotuner using a INMP441 mic board, esp32s3, and MAX98357 I2S amp, to a 3W speaker
  
The ESP32S3 is dual core, both cores are used in parallel by using free RTOS.  

**The main project is the autotuner folder**


## file structure

**audiooutput** - all paino wav files created from aiff files from the University of Iowa Electronic Music Studios: https://theremin.music.uiowa.edu/mispiano.html

**autotuner** - has the main working version of the real time autotuner

**autotuner_hps** - has as not working attempt at using HPS instead of the FFT lib find major peaks function

**diagrams** - diagrams made to help explain the system 

**microphone_test** - is the project I built and used to test my microphone

**pythonversion** - includes the python scripts I made to prototype the DSP algorithms


## System Overview
High Level Overview
![High Level Overview](diagrams\high_level_overview.png)

Mid Level Overview

![Mid Level Overview](diagrams\mid_level_overview.png)  

Mic input to Output
![Mic input to Output](diagrams\mic_input_to_output.png)
  
Overview of Buffer system used for processing
![Overview of Buffer system used for processing](diagrams\buffer_system_overview.png)



## steps/goals
#### Part I:
Write a DSP algorithm to perform an FFT with interpolation or auto coloration to correct the notes in an audio input to a scale, interval shift, or harmonization depending on the mode in Python.

#### Part II:
On a breadboard connect the INMP441 mic breakout board to the esp32s3 and connect the eps32s3 to the MAX98357 amplifier board to the 3 W speaker. Also connect and wire up a screen, and two buttons
The goal will be capturing audio input and repeating the audio input as output to start with not in real time.

#### Part III:
outputting specific notes from the esp32s3 to the speaker via I2S from 44.1 khz 16 bit wav files

#### Part IV:
Integrating the DSP Algorithm into Parts II and III to output the “corrected” notes snaped to the scale, interval, harmonic chosen. The process: recording the input, processing and correcting the notes to the scale, interval, or harmonic, then outputting the corrected notes in the output. Not in real time.

#### Part V:
Implement RTOS and more tunning on the DMA RAM buffers to aim for close to real time correction target – 80-120 ms output after input.

#### Part VI:
Moving the bread board to a PCB this may be done before Parts IV and V

## Next Steps:
Completing Part VI, as this was a class project part IV may be completed in the future.