import numpy as np
import matplotlib.pyplot as plt
import seaborn as sn
from scipy.fft import fft
from scipy.fft import fftfreq
from scipy.io import wavfile
from scipy.signal import find_peaks
from scipy.signal import correlate
from scipy.signal import decimate


# step 1 reading in the wav file (formated as mono, 24-bit, 44.1 kHz)
def read_wav_file(filename):
    fs, data = wavfile.read(filename)
    print(f"filename: {filename}")
    print(f"Sample Rate: {fs} Hz")
    print(f"length of clip: {data.shape[0] / fs} seconds")
    return fs, data

# step 2 cleaning the data (normalization, windowing, etc.)
def cleandata(data):
    # normalize the data
    data = data / np.max(np.abs(data))
    # apply a Hanning window to the data
    window = np.hanning(len(data))
    data = data * window
    N = len(data)
    return data, N

# step 3 method of finding the fundamental frequency

# harmoic product spectrum method
def hps(data, fs, harmoniccount, N):
    yf = np.abs(np.fft.rfft(data))
    spectrum = np.copy(yf)  # to return arr
    
    
    for i in range(2, harmoniccount + 1):
        downsampled_spectrum = decimate(spectrum, i) # downsample the spectrum by i
        spectrum[:len(downsampled_spectrum)] *= downsampled_spectrum # mult overlapping legnth to find the peaks
        spectrum[len(downsampled_spectrum):] = 0

    # finding the peak
    index = np.argmax(spectrum)
    fundamental_freq = index * fs / N
    print(f"HPS Fundamental Frequency: {fundamental_freq} Hz, harmonic : {harmoniccount} count")
    return fundamental_freq

def auto_correlation_method(data, fs):
    corr = correlate(data, data, mode='full')
    corr = corr[corr.size // 2:]

    d = np.diff(corr)
    start = np.where(d > 0)[0][0]

    peak = np.argmax(corr[start:]) + start
    fund = fs/peak
    #plotting  the signal and its autocorrelation
    print(f"Fundamental Frequency: {fund} Hz")
    return fund

def fft_method(data, fs, N):
    yf = np.abs(np.fft.rfft(data))
    xf = fftfreq(N, 1/fs)
    index = np.argmax(yf)
    freq = index * fs / N
    # parabolic interpolation
    if index > 0 and index < len(yf) - 1:
        alpha = yf[index - 1]
        beta = yf[index]
        gamma = yf[index + 1]
        p = 0.5 * (alpha - gamma) / (alpha - 2*beta + gamma)
        freq += p * fs / N

    print(f"FFT Fundamental Frequency: {freq} Hz")
    return freq
