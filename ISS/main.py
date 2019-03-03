import matplotlib.pyplot as plt
import numpy as np

from scipy.io import wavfile
from scipy.signal import lfilter, freqz, tf2zpk
from scipy.fftpack import fft
from scipy.ndimage import shift

# 1
fs, s_data = wavfile.read('xhorna14.wav')
s_data = s_data / 2**15
b_data = np.loadtxt('xhorna14.txt', int)
sample_count = s_data.size
time = sample_count / fs

wav_time = np.linspace(0, time, num=sample_count)
binary_time = np.linspace(0, 0.02, num=20)


# 2
def decode(my_data):
    my_binary = []
    for i in range(8, my_data.size, 16):
        if my_data[i] <= 0:
            my_binary.append(0)
        else:
            my_binary.append(1)

    return np.asarray(my_binary)


s_binary = decode(s_data)

if s_binary.any() != b_data.any():
    print('Wrong!')

f = plt.figure(figsize=(13, 6))
plt.gca().set_title('Uloha 2')
plt.gca().set_xlabel('$t[s]$')
plt.gca().set_ylabel('$s[n]$')
plt.plot(wav_time[:320], s_data[:320])
marker_line, stem_lines, base_line = plt.stem(binary_time[:20], s_binary[:20])

for stem_line in stem_lines:
    plt.setp(stem_line, 'color', 'r')

plt.setp(base_line, 'color', 'black', alpha=0.5)
plt.setp(marker_line, 'color', 'r')

plt.grid(alpha=0.5, linestyle='--')
plt.tight_layout()
plt.show()

# 3
b = np.array([0.0192, -0.0185, -0.0185, 0.0192])
a = np.array([1.0, -2.887, 2.7997, -0.9113])
z, p, _ = tf2zpk(b, a)

f = plt.figure(figsize=(6, 5.5))

ang = np.linspace(0, 2*np.pi, 100)
plt.plot(np.cos(ang), np.sin(ang))

plt.scatter(np.real(z), np.imag(z), marker='o', facecolors='none', edgecolors='r', label='nuly')
plt.scatter(np.real(p), np.imag(p), marker='x', color='g', label='póly')
plt.gca().set_xlabel('Realná složka $\mathbb{R}\{$z$\}$')
plt.gca().set_ylabel('Imaginarní složka $\mathbb{I}\{$z$\}$')
plt.gca().set_title('Uloha 3')

plt.grid(alpha=0.5, linestyle='--')
plt.legend(loc='upper left')

plt.tight_layout()
f.savefig("no3.png", bboxes_inches='tight')
plt.show()


# 4
w, H = freqz(b, a)
mf = np.min(np.abs(H))
mf = np.where(np.abs(H) == mf)
mf = (w / 2 / np.pi *fs)[mf]

f, ax = plt.subplots(1, 2, figsize=(13, 6))

ax[0].plot(w / 2 / np.pi * fs, np.abs(H))
ax[0].set_xlabel('Frekvencia [Hzf]')
ax[0].set_title('Uloha 4\nModul frekvencnej charakteristiky $|H(e^{j\omega})|$')

ax[1].plot(w / 2 / np.pi * fs, np.angle(H))
ax[1].set_xlabel('Frekvencia [Hz]')
ax[1].set_title('Uloha 4\nArgument frekvencnej charakteristiky $\mathrm{arg}\ H(e^{j\omega})$')

for ax1 in ax:
    ax1.grid(alpha=0.5, linestyle='--')

plt.tight_layout()
plt.show()

# 5
ss_data = lfilter(b, a, s_data)
t_shift = 16

f = plt.figure(figsize=(13, 6))
plt.gca().set_xlabel('$t[s]$')
plt.gca().set_ylabel('$s[n], ss[n]$')
plt.gca().set_title('Uloha 5')
plt.plot(wav_time[:320], s_data[:320], label='s[n]')
plt.plot(wav_time[:320], ss_data[:320], 'r', label = 'ss[n]')
plt.legend(loc='lower right')
plt.grid(alpha=0.5, linestyle='--')
plt.tight_layout()
plt.show()

# 6
ss_shift_data = shift(ss_data, -t_shift, cval=np.NaN)
ss_binary = decode(ss_shift_data)


f = plt.figure(figsize=(13, 6))
plt.gca().set_xlabel('$t[s]$')
plt.gca().set_ylabel('$s[n], ss[n], ss_{shifted}[n], ss_{symbols}$')
plt.gca().set_title('Uloha 6')
plt.plot(wav_time[:320], s_data[:320], label='$s[n]$')
plt.plot(wav_time[:320], ss_data[:320], label = '$ss$[n]')
plt.plot(wav_time[:320], ss_shift_data[:320], label = '$ss_{shift}$')
marker_line, stem_lines, base_line = plt.stem(binary_time[:20], ss_binary[:20], label = '$ss_{symbols}$')

for stem_line in stem_lines:
    plt.setp(stem_line, 'color', 'r')

plt.setp(base_line, 'color', 'black')
plt.setp(marker_line, 'color', 'r')

plt.legend(loc='lower right')
plt.grid(alpha=0.5, linestyle='--')
plt.tight_layout()
f.savefig("no6.png", bboxes_inches='tight')
plt.show()

# 7
errors = np.count_nonzero(s_binary != ss_binary)
percentage = errors/s_binary.size * 100


# 8
s_spectrum = np.abs(fft(s_data))
ss_spectrum = np.abs(fft(ss_data))

f = plt.figure(figsize=(13, 6))
plt.plot(np.arange(0, fs//2), s_spectrum[:fs//2])
plt.plot(np.arange(0, fs//2), ss_spectrum[:fs//2])
plt.gca().set_xlabel('$f[Hz]$')
plt.gca().set_title('Uloha 8\nModuly spektier signalov s[n], ss[n]')
plt.grid(alpha=0.5, linestyle='--')
plt.tight_layout()
plt.show()

# 9
xmin = np.min(s_data)
xmax = np.max(s_data)
n_aprx = 50
x = np.linspace(xmin, xmax, n_aprx)

binsize = np.abs(x[1] - x[0])
hist, _ = np.histogram(s_data, n_aprx)
px = hist / sample_count / binsize

if np.sum(px * binsize) != 1.0:
    print('Integral P(x) nie je rovny 1')
    
f = plt.figure(figsize=(13,6))
plt.plot(x, px)
plt.gca().set_xlabel('$x$')
plt.gca().set_title('Uloha 9\nOdhad funkcie hustoty rozdelenia pravdepodobnosti $p(x)$')
plt.gca().grid(alpha=0.5, linestyle='--')
plt.tight_layout()
plt.show()

# 10
k = np.arange(-50, 50)
rk = np.correlate(s_data, s_data, 'full') / sample_count

f = plt.figure(figsize=(13, 6))
plt.plot(k, rk[rk.size//2-50:rk.size//2+50])
plt.gca().set_xlabel('$k$')
plt.gca().set_ylabel('$R[k]$')
plt.gca().set_title('Uloha 10\nVychyleny odhad autokorelacnych koeficientov')
plt.gca().grid(alpha=0.5, linestyle='--')
plt.xticks(np.arange(-50, 51, 10))
plt.tight_layout()
plt.show()

# 11
rk0 = rk[rk.size//2]
rk1 = rk[rk.size//2 + 1]
rk16 = rk[rk.size//2 + 16]


# 12
px1x2, x1_edges, x2_edges = np.histogram2d(s_data[:s_data.size-1], s_data[1:], n_aprx, normed=True)
binsize = np.abs(x1_edges[0] - x1_edges[1]) * np.abs(x2_edges[0] - x2_edges[1])
X, Y = np.meshgrid(x1_edges, x2_edges)

f = plt.figure(figsize=(7, 6))
plt.pcolormesh(X, Y, px1x2)
plt.gca().set_title('Uloha 12\nZdruzena funckia hustoty rozdelenia pravdepodobnesti medzi casmi n, n+1')
plt.colorbar()
plt.tight_layout()
plt.show()

# 13
integral = np.sum(px1x2 * binsize)

# 14

bin_centers_x1 = x1_edges[:-1] + (x1_edges[1:] - x1_edges[:-1]) / 2
bin_centers_x2 = x2_edges[:-1] + (x2_edges[1:] - x2_edges[:-1]) / 2
x1x2 = np.outer(bin_centers_x1, bin_centers_x2)
rkn = np.sum(x1x2 * px1x2 * binsize)