#include <stdio.h>
#include <stdlib.h>
#include "wave.hpp"

int main(void) 
{
    STEREO_PCM pcm0, pcm1;

    wave_read_16bit_stereo(&pcm0, "a.wav");

    pcm1.fs = pcm0.fs;
    pcm1.bits = pcm0.bits;
    pcm1.length = pcm0.length;
    pcm1.sL.reserve(pcm1.length);
    pcm1.sR.reserve(pcm1.length);

    for (int n = 0; n < pcm1.length; ++n) 
    {
        pcm1.sL[n] = pcm0.sL[n];
        pcm1.sR[n] = pcm0.sR[n];
    }

    wave_write_16bit_stereo(&pcm1, "b.wav");

    return 0;
}
