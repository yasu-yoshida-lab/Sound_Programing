#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

int main(void) 
{
    MONO_PCM pcm0, pcm1;

    wave_read_16bit_mono(&pcm0, "a.wav");

    pcm1.fs = pcm0.fs;
    pcm1.bits = pcm0.bits;
    pcm1.length = pcm0.length;
    pcm1.s = (double*)calloc(pcm1.length, sizeof(double));

    for (int n = 0; n < pcm1.length; ++n) {
        pcm1.s[n] = pcm0.s[n];
    }

    wave_write_16bit_mono(&pcm1, "b.wav");

    free(pcm0.s);
    free(pcm1.s);

    return 0;
}
