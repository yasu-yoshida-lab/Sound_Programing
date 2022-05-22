#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void) 
{
    MONO_PCM pcm;
    double a0;
    double *f0, *g0;

    pcm.fs = 44100;
    pcm.bits = 16;
    pcm.length = pcm.fs * 4;
    pcm.s = (double*)calloc(pcm.length, sizeof(double));

    a0 = 0.5;

    f0 = (double*)calloc(pcm.length, sizeof(double));
    g0 = (double*)calloc(pcm.length, sizeof(double));

    f0[0] = 2500.0;
    f0[pcm.length - 1] = 1500.0;
    for (int n = 0; n < pcm.length; ++n)
    {
        f0[n] = f0[0] + (f0[pcm.length - 1] - f0[0]) * n / (pcm.length - 1); 
    }

    for (int n = 0; n < pcm.length; ++n)
    {
        g0[0] = f0[0] * n + (f0[pcm.length - 1] - f0[0]) * n * n / (pcm.length - 1)/ 2.0;
    }

    for (int n = 0; n < pcm.length; ++n)
    {
        pcm.s[n] = a0 * sin(2.0 * M_PI * g0[n] / pcm.fs);
    }

    wave_write_16bit_mono(&pcm, "ex5_2.wav");

    free(pcm.s);
    free(f0);
    free(g0);

    return 0;
}
