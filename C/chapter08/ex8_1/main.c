#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

int main(void) 
{
	MONO_PCM pcm;
	int t0 = 0;
	int m = 0;
	double f0 = 500.0;
	double gain = 0.1;

	pcm.fs = 44100;
	pcm.bits = 16;
	pcm.length = pcm.fs * 1;
	pcm.s = (double*)calloc(pcm.length, sizeof(double));
	
	t0 = pcm.fs / f0;

	for (int n = 0; n < pcm.length; ++n)
	{
		pcm.s[n] = 1.0 - 2.0 * m / t0;
		++m;
		if (m >= t0) { m = 0; } 
	}

	for (int n = 0; n < pcm.length; ++n)
	{
		pcm.s[n] *= gain; 
	}

	wave_write_16bit_mono(&pcm, "ex8_1.wav");

	free(pcm.s);

	return 0;
}
