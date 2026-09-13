# Constant the constants used in the project

note2freq = {
    'C3':130.81,
    'C#3/Db3': 138.59,
    'D3': 146.83,
    'D#3/Eb3': 155.56,
    'E3': 164.81,
    'F3': 174.61,
    'F#3/Gb3': 185.00,
    'G3': 196.00,
    'G#3/Ab3': 207.65,
    'A3': 220.00,
    'A#3/Bb3': 233.08,
    'B3': 246.94,
    'C4': 261.63,
    'C#4/Db4': 277.18,
    'D4': 293.66,
    'D#4/Eb4': 311.13,
    'E4': 329.63,
    'F4': 349.23,
    'F#4/Gb4': 369.99,
    'G4': 392.00,
    'G#4/Ab4': 415.30,
    'A4': 440.00,
    'A#4/Bb4': 466.16,
    'B4': 493.88
}

pianofiles = {
    'C3': '../audiooutput/C3.wav',
    'D3': '../audiooutput/D3.wav',
    'E3': '../audiooutput/E3.wav',
    'F3': '../audiooutput/F3.wav',
    'G3': '../audiooutput/G3.wav',
    'A3': '../audiooutput/A3.wav',
    'B3': '../audiooutput/B3.wav',
    'C4': '../audiooutput/C4.wav',
}

testresults_auto = {
    'A3': False,
    'B3': False,
    'C4': False,
    'D4': False,
    'E4': False,
    'F4': False,
    'G4': False,
}

testresults_hps2 = {
    'A3': False,
    'B3': False,
    'C4': False,
    'D4': False,
    'E4': False,
    'F4': False,
    'G4': False,
}

testresults_hps3 = {
    'A3': False,
    'B3': False,
    'C4': False,
    'D4': False,
    'E4': False,
    'F4': False,
    'G4': False,
}

testresults_hps4 = {
    'A3': False,
    'B3': False,
    'C4': False,
    'D4': False,
    'E4': False,
    'F4': False,
    'G4': False,
}
testresults_hps5 = {
    'A3': False,
    'B3': False,
    'C4': False,
    'D4': False,
    'E4': False,
    'F4': False,
    'G4': False,
}

hpslist = [testresults_hps2, testresults_hps3, testresults_hps4, testresults_hps5]

testresults_fft = {
    'A3': False,
    'B3': False,
    'C4': False,
    'D4': False,
    'E4': False,
    'F4': False,
    'G4': False,
}

results = [testresults_auto, testresults_fft] + hpslist

labels = ['Auto Correlation', 'FFT', 'HPS 2', 'HPS 3', 'HPS 4', 'HPS 5']

# for piano notes

testresults_auto_piano = {
    'C3': False,
    'D3': False,
    'E3': False,
    'F3': False,
    'G3': False,
    'A3': False,
    'B3': False,
    'C4': False
}

testresults_fft_piano = {
    'C3': False,
    'D3': False,
    'E3': False,
    'F3': False,
    'G3': False,
    'A3': False,
    'B3': False,
    'C4': False
}

testresults_hps2_piano = {
    'C3': False,
    'D3': False,
    'E3': False,
    'F3': False,
    'G3': False,
    'A3': False,
    'B3': False,
    'C4': False
}

testresults_hps3_piano = {
    'C3': False,
    'D3': False,
    'E3': False,
    'F3': False,
    'G3': False,
    'A3': False,
    'B3': False,
    'C4': False
}

testresults_hps4_piano = {
    'C3': False,
    'D3': False,
    'E3': False,
    'F3': False,
    'G3': False,
    'A3': False,
    'B3': False,
    'C4': False
}

testresults_hps5_piano = {
    'C3': False,
    'D3': False,
    'E3': False,
    'F3': False,
    'G3': False,
    'A3': False,
    'B3': False,
    'C4': False
}
hpslist_piano = [testresults_hps2_piano, testresults_hps3_piano, testresults_hps4_piano, testresults_hps5_piano]
results_piano = [testresults_auto_piano, testresults_fft_piano] + hpslist_piano

