#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
    MONO_PCM pcm0, pcm1, pcm2;
    
    wave_read_16bit_mono(&pcm0, "drum.wav");
    wave_read_16bit_mono(&pcm1, "response.wav");

    pcm2.fs = pcm0.fs;
    pcm2.bits = pcm0.bits;
    pcm2.length = pcm0.length; 
    pcm2.s = (double*)calloc(pcm2.length, sizeof(double));

    int J = pcm1.fs * 1.0;

    for (int n = 0; n < pcm2.length; ++n)
    {
        for (int m = 0; m <= J; ++m)
        {
            pcm2.s[n] += pcm1.s[m] * pcm0.s[n - m];
        }
    }

    wave_write_16bit_mono(&pcm2, "ex6_5_.wav");

    free(pcm0.s);
    free(pcm1.s);
    free(pcm2.s);

    return 0;
}
