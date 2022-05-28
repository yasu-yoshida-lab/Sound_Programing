#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "window_function.h"
#include "fft.h"

int main(void)
{
    MONO_PCM pcm0, pcm1, pcm2;
    int offset, frame, number_of_frame;
    int band, number_of_band, band_width;
    double *x_real, *x_imag, *y_real, *y_imag, *b_real, *b_imag;
    double a, *w, *s;

    wave_read_16bit_mono(&pcm0, "synth.wav");
    wave_read_16bit_mono(&pcm1, "vocal.wav");

    pcm2.fs = pcm0.fs;
    pcm2.bits = pcm0.bits;
    pcm2.length = pcm0.length;
    pcm2.s = (double*)calloc(pcm2.length, sizeof(double));

    s = (double*)calloc(pcm2.length, sizeof(double));

    s[0] = 0.0;
    for (int n = 1; n < pcm1.length; ++n)
    {
        s[n] = pcm1.s[n] - 0.98 * pcm1.s[n - 1];
    }
    for (int n = 0; n < pcm1.length; ++n)
    {
        pcm1.s[n] = s[n];
    }

    int N = 1024;

    x_real = (double*)calloc(N, sizeof(double));
    x_imag = (double*)calloc(N, sizeof(double));
    y_real = (double*)calloc(N, sizeof(double));
    y_imag = (double*)calloc(N, sizeof(double));
    b_real = (double*)calloc(N, sizeof(double));
    b_imag = (double*)calloc(N, sizeof(double));

    w = (double*)calloc(N, sizeof(double));

    Hanning_window(w, N);

    number_of_frame = (pcm0.length - N / 2) / (N / 2);

    band_width = 8;
    number_of_band = N / 2 / band_width; 

     for (frame = 0; frame < number_of_frame; ++frame)
     {
         offset = N / 2 * frame;
         /* X(n) */
         for (int n = 0; n < N; ++n)
         {
             x_real[n] = pcm0.s[offset + n] * w[n];
             x_imag[n] = 0.0;
         }
         FFT(x_real, x_imag, N);
         /* B(k) */
         for (int n = 0; n < N; ++n)
         {
             b_real[n] = pcm1.s[offset + n] * w[n];
             b_imag[n] = 0.0;
         }
         FFT(b_real, b_imag, N);
         
         for (int k = 0; k < N; ++k)
         {
             b_real[k] = sqrt(b_real[k] * b_real[k] + b_imag[k] * b_imag[k]);
             b_imag[k] = 0.0;
         }

         for (int band = 0; band < number_of_band; ++band)
         {
             offset = band_width * band;
             a = 0.0;
             for (int k = 0; k < band_width; ++k)
             {
                 a += b_real[offset + k];
             }
             a /= band_width;
             for (int k = 0; k < band_width; ++k)
             {
                 b_real[offset + k] = a;
             }
         }
         b_real[0] = 0.0;
         b_real[N / 2] = 0.0;
         for (int k = 1; k < N / 2; ++k)
         {
             b_real[N - k] = b_real[k]; 
         }

         for (int k = 0; k < N; ++k)
         {
             y_real[k] = x_real[k] * b_real[k] - x_imag[k] * b_imag[k];
             y_imag[k] = x_imag[k] * b_real[k] + x_real[k] * b_imag[k];
         }
         IFFT(y_real, y_imag, N);

         offset = N / 2 * frame;

         for (int n = 0; n < N; ++n)
         {
             pcm2.s[offset + n] += y_real[n];
         }
     }

    wave_write_16bit_mono(&pcm2, "ex7_4.wav");

    free(pcm0.s);
    free(pcm1.s);
    free(x_real);
    free(x_imag);
    free(y_real);
    free(y_imag);
    free(b_real);
    free(b_imag);
    free(w);
    free(s);
    return 0;
}
