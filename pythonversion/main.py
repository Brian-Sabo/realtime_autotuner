import fundmental as f
import constants as c
import plot as p

def starting_test(filename, harmoniccount, correct_freq, notestring, case):
    samplerate, data = f.read_wav_file(filename)
    data, N = f.cleandata(data)
    fund_auto = f.auto_correlation_method(data, samplerate)
    fund_hps = f.hps(data, samplerate, harmoniccount, N)
    fund_fft = f.fft_method(data, samplerate, N)
    Threshold = 6   #Hz the threshold for test


    print(f"correct freq: {correct_freq} Hz")
    if abs(fund_auto - correct_freq) < Threshold:
        print("Auto Correlation Method: Correct")
        if (case == "trumpet"):
            c.testresults_auto[notestring] = True
        elif (case == "piano"):
            c.testresults_auto_piano[notestring] = True
    else:
        print("Auto Correlation Method: Incorrect difference: ", abs(fund_auto - correct_freq))

    if abs(fund_hps - correct_freq) < Threshold:
        print("HPS Method: Correct")
        if (case == "trumpet"):
            c.hpslist[harmoniccount - 2][notestring] = True
        elif (case == "piano"):
            c.hpslist_piano[harmoniccount - 2][notestring] = True
    else:        
        print("HPS Method: Incorrect difference: ", abs(fund_hps - correct_freq))

    if abs(fund_fft - correct_freq) < Threshold:
        print("FFT Method: Correct")
        if (case == "trumpet"):
            c.testresults_fft[notestring] = True
        elif (case == "piano"):
            c.testresults_fft_piano[notestring] = True
    else:
        print("FFT Method: Incorrect difference: ", abs(fund_fft - correct_freq))


for note, filename in c.pianofiles.items():
    freq = c.note2freq.get(note)
    for i in range(2, 6):
        starting_test(filename, i, freq, note, "piano")

p.plot_results("Piano", c.labels, c.results_piano)