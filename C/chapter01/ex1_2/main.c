#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

int main(void) 
{
    STEREO_PCM pcm0, pcm1;

    wave_read_16bit_stereo(&pcm0, "a.wav");

    pcm1.fs = pcm0.fs;
    pcm1.bits = pcm0.bits;
    pcm1.length = pcm0.length;
    pcm1.sL = (double*)calloc(pcm0.length, sizeof(double));
    pcm1.sR = (double*)calloc(pcm0.length, sizeof(double));

    for (int n = 0; n < pcm1.length; ++n) {
        pcm1.sL[n] = pcm0.sL[n];
        pcm1.sR[n] = pcm0.sR[n];
    }

    wave_write_16bit_stereo(&pcm1, "b.wav");

    free(pcm0.sL);
    free(pcm0.sR);
    free(pcm1.sL);
    free(pcm1.sR);

    return 0;
}
