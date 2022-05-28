#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "window_function.h"
#include "sinc.h"
#include "fir_filter.h"

int main(void) 
{
    MONO_PCM pcm0, pcm1; 
    double *b, *w;

    wave_read_16bit_mono(&pcm0, "sine_500hz_3500hz.wav");

    pcm1.fs = pcm0.fs;
    pcm1.bits = pcm0.bits;
    pcm1.length = pcm0.length;
    pcm1.s = (double*)calloc(pcm1.length, sizeof(double));

    double fe = 1000.0 / pcm0.fs;
    double delta = 1000.0 / pcm0.fs;

    int J = (int)(3.1 / delta + 0.5) - 1;
    if (J % 2 == 1) 
    {
        ++J;
    }

    b = (double*)calloc((J + 1) , sizeof(double));
    w = (double*)calloc((J + 1) , sizeof(double));
    
    Hanning_window(w, (J + 1));

    FIR_LPF(fe, J, b, w);

    for (int n = 0; n < pcm1.length; ++n)
    {
        for (int m = 0; m <= J; ++m)
        {
            if (n - m >= 0)
            {
                pcm1.s[n] += b[m] * pcm0.s[n - m];
            }
        }
    }

    wave_write_16bit_mono(&pcm1, "ex6_1.wav");

    free(pcm0.s);
    free(pcm1.s);
    free(b);
    free(w);

    return 0;
}


