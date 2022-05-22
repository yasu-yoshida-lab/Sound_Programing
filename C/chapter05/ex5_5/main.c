#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void) 
{
    MONO_PCM pcm;
    double *a0, *a1, *a2, *a3, *a4;
    double *f0, *f1, *f2, *f3, *f4;
    double gain = 0.1;

    pcm.fs = 44100;
    pcm.bits = 16;
    pcm.length = pcm.fs * 4;
    pcm.s = (double*)calloc(pcm.length, sizeof(double));

    a0 = (double*)calloc(pcm.length, sizeof(double));
    a1 = (double*)calloc(pcm.length, sizeof(double));
    a2 = (double*)calloc(pcm.length, sizeof(double));
    a3 = (double*)calloc(pcm.length, sizeof(double));
    a4 = (double*)calloc(pcm.length, sizeof(double));
    
    f0 = (double*)calloc(pcm.length, sizeof(double));
    f1 = (double*)calloc(pcm.length, sizeof(double));
    f2 = (double*)calloc(pcm.length, sizeof(double));
    f3 = (double*)calloc(pcm.length, sizeof(double));
    f4 = (double*)calloc(pcm.length, sizeof(double));

    for (int n = 0; n < pcm.length; ++n)
    {
      a0[n] = 1.0 * exp(-5.0 * n / (pcm.fs * 4.0));
      a1[n] = 0.8 * exp(-5.0 * n / (pcm.fs * 2.0));
      a2[n] = 0.6 * exp(-5.0 * n / (pcm.fs * 1.0));
      a3[n] = 0.5 * exp(-5.0 * n / (pcm.fs * 0.5));
      a4[n] = 0.4 * exp(-5.0 * n / (pcm.fs * 0.2));
      
      f0[n] = 440;
      f1[n] = 880;
      f2[n] = 1320;
      f3[n] = 1760;
      f4[n] = 2200;
    }

    for (int n = 0; n < pcm.length; ++n)
    {
      pcm.s[n] += a0 [n] * sin(2.0 * M_PI * f0[n] * n / pcm.fs);
      pcm.s[n] += a1 [n] * sin(2.0 * M_PI * f1[n] * n / pcm.fs);
      pcm.s[n] += a2 [n] * sin(2.0 * M_PI * f2[n] * n / pcm.fs);
      pcm.s[n] += a3 [n] * sin(2.0 * M_PI * f3[n] * n / pcm.fs);
      pcm.s[n] += a4 [n] * sin(2.0 * M_PI * f4[n] * n / pcm.fs);
    }

    for (int n = 0; n < pcm.length; ++n)
    {
        pcm.s[n] *= gain;
    }

    for (int n = 0; n < pcm.length * 0.01; ++n)
    {
        pcm.s[n] *= (double)n / (pcm.fs * 0.01);
        pcm.s[pcm.length - n - 1] *= (double)n / (pcm.fs * 0.01);
    }

    wave_write_16bit_mono(&pcm, "ex5_5.wav");

    free(pcm.s);
    free(a0);
    free(a1);
    free(a2);
    free(a3);
    free(a4);
    free(f0);
    free(f1);
    free(f2);
    free(f3);
    free(f4);

    return 0;
}
