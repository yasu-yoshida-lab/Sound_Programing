#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
    MONO_PCM pcm;
    double *a0;
    double f0 = 500.0;

    pcm.fs = 44100;
    pcm.bits = 16;
    pcm.length = pcm.fs * 4;
    pcm.s = (double*)calloc(pcm.length, sizeof(double));

    a0 = (double*)calloc(pcm.length, sizeof(double));

    a0[0] = 0.5;
    a0[pcm.length - 1] = 0.0;
    for (int n = 0; n < pcm.length; ++n)
    {
        a0[n] = a0[0] + (a0[pcm.length - 1] - a0[0]) * n / (pcm.length - 1); 
    }

    for (int n = 0; n < pcm.length; ++n)
    {
        pcm.s[n] = a0[n] * sin(2.0 * M_PI * f0 * n / pcm.fs);
    }

    wave_write_16bit_mono(&pcm, "ex5_1.wav");

    free(pcm.s);
    free(a0);
    
    return 0;
}
