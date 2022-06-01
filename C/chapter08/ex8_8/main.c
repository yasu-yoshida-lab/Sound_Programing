#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

int main(void)
{
	MONO_PCM pcm;
	int t0, pe;
	double *f0, *e;
	double gain = 0.1;

	pcm.fs = 44100;
	pcm.bits = 16;
	pcm.length = pcm.fs * 0.6;
	pcm.s = (double*)calloc(pcm.length, sizeof(double));

	f0 = (double*)calloc(pcm.length, sizeof(double));

	for (int n = 0; n < pcm.fs * 0.2; ++n)
	{
		f0[n] = 440.0;
	}
	for (int n = pcm.fs * 0.2; n < pcm.length; ++n)
	{
		f0[n] = 440.0 + (880.0 - 440.0) * (n - pcm.fs * 0.2) / (pcm.length - 1 - pcm.fs * 0.2);
	}

	t0 = pcm.fs / f0[0];
	int m = 0;

	for (int n = 0; n < pcm.length; ++n)
	{
		if (m < t0 / 2.0) 
		{ 
			pcm.s[n] = 1.0;
		}
		else 
		{
			pcm.s[n] = -1.0;
		}
		++m;
		if (m >= t0)
		{
			t0 = pcm.fs / f0[n];
			m = 0; 
		}
	}

	e = (double*)calloc(pcm.length, sizeof(double));
	pe = pcm.length;

	for (int n = 0; n < pcm.length; ++n)
	{
		e[n] = 1.0 - (double)n / pe;
	}

	gain = 0.1;
	
	for (int n = 0; n < pcm.length; ++n)
	{
		pcm.s[n] *= e[n] * gain;
	}

	wave_write_16bit_mono(&pcm, "ex8_8.wav");

	free(pcm.s);
	free(f0);
	free(e);

	return 0;
}