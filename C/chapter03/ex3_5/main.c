#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void) 
{
    MONO_PCM pcm;
    double f0 = 1.0;
    double theta = 0.0;
    double gain = 0.001;

    pcm.fs = 44100;
    pcm.bits = 16;
    pcm.length = pcm.fs * 1;
    pcm.s = (double*)calloc(pcm.length, sizeof(double));

    for (int i = 1; i <= 22050; ++i)
    {
        theta = (double)rand() / RAND_MAX * 2.0 * M_PI;
        for (int n = 0; n < pcm.length; ++n) 
        {
            pcm.s[n] += sin(2.0 * M_PI * i * f0 * n / pcm.fs + theta);
        }
    }

    for (int n = 0; n < pcm.length; ++n) 
    { 
        pcm.s[n] *= gain; 
    }

    wave_write_16bit_mono(&pcm, "ex3_5.wav");

    free(pcm.s);

    return 0;
}
