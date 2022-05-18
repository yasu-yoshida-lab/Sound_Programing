import wave 
import numpy as np
import struct
from matplotlib import pyplot as plt

a = 1
fs = 44100
f0 = 440
sec = 5

if __name__=='__main__':
	x = np.arange(fs * sec)
	y = [a * np.sin(2.0 * np.pi * f0 * n / fs) for n in x]
	pcm_data = [int(data * 32767.3) for data in y]
	bin_data = struct.pack("h" * len(pcm_data), *pcm_data)

	wave_file = wave.Wave_write("sample.wav")
	p = (1, 2, fs, len(bin_data), 'NONE', 'not compressed')
	wave_file.setparams(p)
	wave_file.writeframes(bin_data)
	wave_file.close()
