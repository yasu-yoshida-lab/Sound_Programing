#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
    MONO_PCM pcm; 
    double a = 0.1;
    double f0 = 500.0;

    pcm.fs = 44100;
    pcm.bits = 16;
    pcm.length = pcm.fs * 1;
    pcm.s = (double*)calloc(pcm.length, sizeof(double));

    for (int n = 0; n < pcm.length; ++n) 
    {
        pcm.s[n] = a * sin(2.0 * M_PI * f0 * n / pcm.fs);
    }

    wave_write_16bit_mono(&pcm, "ex2_1.wav");

    free(pcm.s);

    return 0;
}
