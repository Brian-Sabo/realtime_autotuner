# Realtime_Autotuner
A Realtime_Autotuner using a INMP441 mic board, esp32s3, and MAX98357 I2S amp, to a 3W speaker
  
The ESP32S3 is dual core, both cores are used in parallel by using free RTOS.  

**The main project is the autotuner folder**


## File Structure

**audiooutput** - all paino wav files created from aiff files from the University of Iowa Electronic Music Studios: https://theremin.music.uiowa.edu/mispiano.html

**autotuner** - has the main working version of the real time autotuner

**autotuner_hps** - has as not working attempt at using HPS instead of the FFT lib find major peaks function

**diagrams** - diagrams made to help explain the system 

**microphone_test** - is the project I built and used to test my microphone

**pythonversion** - includes the python scripts I made to prototype the DSP algorithms


## System Overview
High Level Overview

<img width="742" height="263" alt="Screenshot 2026-09-13 012047" src="https://github.com/user-attachments/assets/ca3701ae-97d2-46e0-b90a-89db32c45833" />

Mid Level Overview

<img width="650" height="517" alt="Screenshot 2026-09-13 012320" src="https://github.com/user-attachments/assets/8996b349-d73c-45bb-a5db-f62affac6cf3" /> 

Mic input to Output

<img width="833" height="299" alt="Screenshot 2026-09-13 012416" src="https://github.com/user-attachments/assets/d2393773-ede3-4651-9f71-b6b8ca2c9901" />

Overview of Buffer system used for processing

<img width="663" height="576" alt="Screenshot 2026-09-13 012523" src="https://github.com/user-attachments/assets/70485bca-d6ce-444b-960a-c15aff3e74ea" />

## Steps/Goals
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
