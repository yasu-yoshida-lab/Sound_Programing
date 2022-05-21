#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

void sine_wave(MONO_PCM *pcm, double f0, double a, int offset, int duration)
{
    double *s = (double*)calloc(duration, sizeof(double));

    for (int n = 0; n < duration; ++n)
    {
        s[n] = a * sin(2.0 * M_PI * f0 * n / pcm->fs);
    }

    for (int n = 0; n < pcm->fs * 0.01; ++n) 
    { 
        s[n] *= (double)n / (pcm->fs * 0.01);
        s[duration - n - 1] *= (double)n / (pcm->fs * 0.01);
    }

    for (int n = 0; n < duration; ++n)
    { 
        pcm->s[offset + n] += s[n];
    }

    free(s);
}

int main(void) 
{
    MONO_PCM pcm;

    pcm.fs = 44100;
    pcm.bits = 16;
    pcm.length = pcm.fs * 2;
    pcm.s = (double*)calloc(pcm.length, sizeof(double));

    sine_wave(&pcm, 261.63, 0.1, pcm.fs * 0.00, pcm.fs * 0.25); /* C4 */
    sine_wave(&pcm, 293.66, 0.1, pcm.fs * 0.25, pcm.fs * 0.25); /* D4 */
    sine_wave(&pcm, 329.63, 0.1, pcm.fs * 0.50, pcm.fs * 0.25); /* E4 */
    sine_wave(&pcm, 349.23, 0.1, pcm.fs * 0.75, pcm.fs * 0.25); /* F4 */
    sine_wave(&pcm, 392.00, 0.1, pcm.fs * 1.00, pcm.fs * 0.25); /* G4 */
    sine_wave(&pcm, 440.00, 0.1, pcm.fs * 1.25, pcm.fs * 0.25); /* A4 */
    sine_wave(&pcm, 493.88, 0.1, pcm.fs * 1.50, pcm.fs * 0.25); /* B4 */
    sine_wave(&pcm, 523.25, 0.1, pcm.fs * 1.75, pcm.fs * 0.25); /* C5 */

    wave_write_16bit_mono(&pcm, "ex2_2.wav");

    free(pcm.s);

    return 0;
}
