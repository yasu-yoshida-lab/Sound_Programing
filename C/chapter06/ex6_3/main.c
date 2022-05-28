#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "window_function.h"
#include "sinc.h"
#include "fir_filter.h"
#include "fft.h"

int main(void) 
{
    MONO_PCM pcm0, pcm1;
    double *x_real, *x_imag, *y_real, *y_imag, *b_real, *b_imag;
    double *b, *w;

    wave_read_16bit_mono(&pcm0, "sine_500hz_3500hz.wav");

    pcm1.fs = pcm0.fs;
    pcm1.bits = pcm0.length;
    pcm1.length = pcm0.length;
    pcm1.s = (double*)calloc(pcm1.length, sizeof(double));

    double fe = 1000.0 / pcm0.fs;
    double delta = 1000.0 / pcm0.fs;

    int J = (int)(3.1 / delta + 0.5) - 1;
    if (J % 2 == 1)
    {
        ++J;
    }

    b = (double*)calloc((J + 1), sizeof(double));
    w = (double*)calloc((J + 1), sizeof(double));

    Hanning_window(w, (J + 1));

     int L = 128;
     int N = 256;

    FIR_LPF(fe, J, b, w);

    x_real = (double*)calloc(N, sizeof(double));
    x_imag = (double*)calloc(N, sizeof(double));
    y_real = (double*)calloc(N, sizeof(double));
    y_imag = (double*)calloc(N, sizeof(double));
    b_real = (double*)calloc(N, sizeof(double));
    b_imag = (double*)calloc(N, sizeof(double));

    int number_of_frame = pcm0.length / L;

    int offset = 0;
    for (int frame = 0; frame < number_of_frame; ++frame)
    {
        offset = L * frame;
        for (int n = 0; n < N; +n)
        {
            x_real[n] = 0.0;
            x_imag[n] = 0.0;
        }
        for (int n = 0; n < N; +n)
        {
            x_real[n] = pcm0.s[offset + n];
        }
        FFT(x_real, x_imag, N);
        for (int m = 0; m < N; ++m)
        {
            b_real[m] = 0.0;
            b_imag[m] = 0.0;
        }
        for (int m = 0; m <= J; ++m)
        {
            b_real[m] = b[m];
        }
        FFT(b_real, b_imag, N);
        for (int k = 0; k < N; ++k)
        {
            y_real[k] = x_real[k] * b_real[k] - x_imag[k] * b_imag[k];
            y_imag[k] = x_imag[k] * b_real[k] + x_real[k] * b_imag[k];
        }
        FFT(y_real, y_imag, N);
        for (int n = 0; n < N; ++n)
        {
            if (offset + n < pcm1.length)
            {
                pcm1.s[offset + n] += y_real[n];
            }
        }
    }

    wave_write_16bit_mono(&pcm1, "ex6_3_.wav");

    free(pcm0.s);
    free(pcm1.s);
    free(x_real);
    free(x_imag);
    free(y_real);
    free(y_imag);
    free(b_real);
    free(b_imag);
    free(b);
    free(w);

    return 0;
}
