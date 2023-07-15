from numpy.random import randn
from numpy.fft import fftshift, ifft, fft, rfft, irfft
import numpy as np
import sys
import matplotlib.pyplot as plt
from math import floor, log2

np.set_printoptions(threshold=sys.maxsize)

fs = 48000
t = 1
f = 1243
N = fs * t
phase = 0
phase_increment = f / fs
oversampling_factor = 4
phase_increment_oversampling = phase_increment / oversampling_factor

T = 1/ fs
x = np.linspace(0.0, (N-1)*T, N)
xf = np.linspace(0.0, 1.0/(2.0*T), 2050//2)

wav_table = np.zeros(2048)
for w in range(2048):
	wav_table[w] = 2 * w/2048.0 - 1.0

wav_fft = np.fft.rfft(wav_table)
# wav_fft = np.absolute(wav_fft)  # Get rid of negatives
# sp[1:3000] = 0
W = np.fft.rfftfreq(2048, 1/fs)
octave_scale = 2**floor(log2(floor(20000/f)))
print(octave_scale)
wav_fft[(W>23.44*octave_scale)] = 0;
# wav_fft[2:] = 0;


wav_table = irfft(wav_fft);


# print(wav_table)

plt.plot(wav_table)
plt.show()

# # print(freq)
# plt.plot(xf, wav_fft)
# plt.show()


samples = np.linspace(0, t, int(fs*t), endpoint=False)
signal = np.zeros(samples.shape[0])
oversampling_buffer = []

fir_coefficient=[0.02442415, 0.09297315, 0.16712938, 0.21547332]
# fir_coefficient=[
# 	0.20469201621193667,
# 	0.7467708073303536,
# 	0.7467708073303536,
# 	0.20469201621193667]


head = 0
tail = 0

for i in range(samples.shape[0]):
	for j in range(oversampling_factor):
		saw_signal = wav_table[floor(phase*2048.0)]
		if oversampling_factor == 1:
			head = saw_signal
		else:
			head += saw_signal * fir_coefficient[3 - (j & 3)]
			tail += saw_signal * fir_coefficient[j & 3]
		phase += phase_increment_oversampling
		if phase >= 1.0:
			phase -= 1.0

	head = max(-1.0, min(head, 1.0))
	signal[i] = head
	head = tail
	tail = 0
	# if i % 2 == 1:
	# 	signal[i//2] = 2 * phase - 1.0		
	# oversampling_buffer.append(2 * phase - 1.0)

	# signal[i] = 2 * phase - 1.0 


	# signal[i] = 2 * phase - 1.0 
	# phase += phase_increment
	# if phase >= 1.0:
	# 	phase -= 1.0
# signal = np.sin(2 * np.pi * f * samples)

signal *= 32767
signal = np.int16(signal)

plt.plot(signal[:100])
plt.show()
# result = fft(signal)

sp = np.fft.rfft(signal)
sp = np.absolute(sp)  # Get rid of negatives
# sp[1:3000] = 0
# sp[20000:] = 0;
signal_cleared = irfft(sp);
plt.plot(signal[:100])
plt.show()

# sp[end-N_clear+2:end] = 0;
freq = np.fft.rfftfreq(N, 1/fs)
# print(freq)
plt.plot(freq, sp)
plt.show()


# freqs = np.fft.fftfreq(10, 0.1)

# print(freqs)

sys.exit()

N = 1000;
signal = randn(N, 1);

N_clear = 490;

# Remove the calls to fftshift, if you want to delete the lower frequency components
S = fftshift(fft(signal));   
S_cleared = S;
S_cleared[1:N_clear] = 0;
S_cleared[end-N_clear+2:end] = 0;
S_cleared = fftshift(S_cleared);

signal_cleared = ifft(S_cleared);

subplot(2,2,1);
plot(signal);
title('input signal');

subplot(2,2,2);
plot(abs(S));
title('input spectrum');

subplot(2,2,3);
plot(abs(S_cleared));
title('output spectrum');

subplot(2,2,4);
plot(signal_cleared);
title('output signal');