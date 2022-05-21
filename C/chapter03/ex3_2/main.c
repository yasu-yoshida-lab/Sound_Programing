#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void) 
{
    MONO_PCM pcm;

    double f0 = 500.0;

    pcm.fs = 44100;
    pcm.bits = 16;
    pcm.length = pcm.fs * 1;
    pcm.s = (double*)calloc(pcm.length, sizeof(double));

    for (int i = 1; i <= 44; i = i + 2)
    { 
        for (int n = 0; n < pcm.length; n++) 
        {
            pcm.s[n] += 1.0 / i * sin(2.0 * M_PI * i * f0 * n / pcm.fs);
        }
    }

    double gain = 0.1;

    for (int n = 0; n < pcm.length; ++n) 
    {
        pcm.s[n] *= gain;
    }

    wave_write_16bit_mono(&pcm, "ex3_2.wav");

    free(pcm.s);

    return 0;
}
