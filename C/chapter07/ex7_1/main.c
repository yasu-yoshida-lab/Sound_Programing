#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "iir_filter.h"

int main(void)
{
    MONO_PCM pcm0, pcm1;
    double *fc, a[3], b[3];

    wave_read_16bit_mono(&pcm0, "pulse_train.wav");
    
    fc = (double*)calloc(pcm0.length, sizeof(double));

    for (int n = 0; n < pcm0.length; ++n)
    {
        fc[n] = 10000.0 * exp(-5.0 * n / pcm0.length);
    }

    double Q = 1.0 / sqrt(2.0);
    int I = 2;
    int J = 2;

    pcm1.fs = pcm0.fs;
    pcm1.bits = pcm0.bits;
    pcm1.length = pcm0.length;
    pcm1.s = (double*)calloc(pcm1.length, sizeof(double));

    for (int n = 0; n < pcm1.length; ++n)
    {
        IIR_LPF(fc[n] / pcm1.fs, Q, a, b);
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

    wave_write_16bit_mono(&pcm1, "ex7_1.wav");

    free(pcm0.s);
    free(pcm1.s);
    free(fc);
    
    return 0;
}
