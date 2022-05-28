#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "window_function.h"
#include "fft.h"

int main(void)
{
    MONO_PCM pcm0, pcm1;
    int offset, number_of_frame;
    double *x_real, *x_imag, *y_real, *y_imag, *b_real, *b_imag, *w;
    double fe;

    wave_read_16bit_mono(&pcm0, "sine_500hz_3500hz.wav");

    pcm1.fs = pcm0.fs;
    pcm1.bits = pcm0.bits;
    pcm1.length = pcm0.length;
    pcm1.s = (double*)calloc(pcm1.length, sizeof(double));

    int N = 256;

    x_real = (double*)calloc(N, sizeof(double));
    x_imag = (double*)calloc(N, sizeof(double));
    y_real = (double*)calloc(N, sizeof(double));
    y_imag = (double*)calloc(N, sizeof(double));
    b_real = (double*)calloc(N, sizeof(double));
    b_imag = (double*)calloc(N, sizeof(double));

    w = (double*)calloc(N, sizeof(double));

    Hanning_window(w, N);
    number_of_frame = (pcm0.length - N / 2) / (N / 2);
    
     for (int frame = 0; frame < number_of_frame; ++frame)
     {
        offset = N / 2 * frame;
        
        for (int n = 0; n < N; ++n)
        {
            x_real[n] = pcm0.s[offset + n] * w[n];
            x_imag[n] = 0.0;
        }
        FFT(x_real, x_imag, N);
        fe = 1000.0 / pcm0.fs;
        fe *= N;
        for (int k = 0; k <= fe; ++k)
        {
            b_real[k] = 1.0;
            b_imag[k] = 0.0;
        }
        for (int k = fe + 1; k <= N / 2; ++k) 
        {
            b_real[k] = 0.0;
           b_imag[k] = 0.0;
        }
        for (int k = 1; k < N / 2; ++k)
        {
            b_real[N - k] = b_real[k];
            b_imag[N - k] = -1 * b_imag[k];
        }
        for (int k = 0; k < N; ++k)
        {
            y_real[k] = x_real[k] * b_real[k] - x_imag[k] * b_imag[k];
            y_imag[k] = x_imag[k] * b_real[k] + x_real[k] * b_imag[k];
        }
        IFFT(y_real, y_imag, N);
        for (int n = 0; n < N; ++n)
        {
            pcm1.s[offset + n] += y_real[n];
        }
     }
    wave_write_16bit_mono(&pcm1, "ex6_4.wav");

    free(pcm0.s);
    free(pcm1.s);
    free(x_real);
    free(x_imag);
    free(y_real);
    free(y_imag);
    free(b_real);
    free(b_imag);
    free(w);

    return 0;
}
