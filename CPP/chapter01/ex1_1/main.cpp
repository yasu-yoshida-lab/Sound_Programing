#include <iostream>
#include "wave.hpp"

int main()
{
    MONO_PCM pcm0, pcm1;

    wave_read_16bit_mono(&pcm0, "a.wav");

    pcm1.fs = pcm0.fs;
    pcm1.bits = pcm0.bits;
    pcm1.length = pcm0.length;
    pcm1.s.reserve(pcm1.length);

    for (int n = 0; n < pcm1.length; ++n) 
    {
        pcm1.s[n] = pcm0.s[n];
    }

    wave_write_16bit_mono(&pcm1, "b.wav");

    return 0;
}
