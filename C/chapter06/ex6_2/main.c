#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "iir_filter.h"

int main(void) 
{
    MONO_PCM pcm0, pcm1;
    double a[3], b[3];
    wave_read_16bit_mono(&pcm0, "sine_500hz_3500hz.wav");

    pcm1.fs = pcm0.fs;
    pcm1.bits = pcm0.bits;
    pcm1.length = pcm0.length;
    pcm1.s = (double*)calloc(pcm1.length, sizeof(double));

    const double fc = 1000.0 / pcm0.fs;
    const double Q = 1.0 / sqrt(2.0);
    const int I = 2;
    const int J = 2;

    IIR_LPF(fc, Q, a, b);

    for (int n = 0; n < pcm0.length; ++n)
    {
        for (int m = 0; m <= J; ++m)
        {
            if (n - m >= 0) 
            {
                pcm1.s[n] += b[m] * pcm0.s[n - m];
            }
        }
        for (int m = 1; m <= I; ++m) 
        {
            if (n - m >= 0)
            {
                pcm1.s[n] += -1 * a[m] * pcm1.s[n - m];
            }
        }
    }

    wave_write_16bit_mono(&pcm1, "ex6_2.wav");

    free(pcm0.s);
    free(pcm1.s);

    return 0;
}
