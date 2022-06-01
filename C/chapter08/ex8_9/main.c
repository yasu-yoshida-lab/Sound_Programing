#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

int main(void)
{
	MONO_PCM pcm;
	int t0 = 0; 
	double *f0 = NULL;
	double gain = 0.1;

	pcm.fs = 8000; 
	pcm.bits = 16;
	pcm.length = pcm.fs * 2;
	pcm.s = (double*)calloc(pcm.length, sizeof(double));

	f0 = (double*)calloc(pcm.length, sizeof(double));

	f0[0] = 500.0;
	f0[pcm.length - 1] = 3500.0;
	for (int n = 0; n < pcm.length; ++n)
	{
		f0[n] = f0[0] + (f0[pcm.length - 1] - f0[0]) * n / (pcm.length - 1);
	}

	t0 = pcm.fs / f0[0];
	int m = 0;
	for (int n = 0; n < pcm.length; ++n)
	{
		pcm.s[n] = 1.0 - 2.0 * m / t0;
		++m;
		if (m >= t0)
		{
			t0 = pcm.fs / f0[n];
			m = 0; 
		}
	}

	for (int n = 0; n < pcm.length; ++n)
	{
		pcm.s[n] *= gain;
	}

	wave_write_16bit_mono(&pcm, "ex8_9.wav");

	free(pcm.s);
	free(f0);

	return 0;
}