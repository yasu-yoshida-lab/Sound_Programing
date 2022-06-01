#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

int main(void)
{
	MONO_PCM pcm;
	int m = 0; 
	int te = 0;
	double *e = NULL;
	double gain = 0.1;

	pcm.fs = 44100;
	pcm.bits = 16;
	pcm.length = pcm.fs * 8;
	pcm.s = (double*)calloc(pcm.length, sizeof(double));

	for (int n = 0; n < pcm.length; ++n)
	{
		pcm.s[n] = (double)rand() / RAND_MAX * 2.0 - 1.0; 
	}

	e = (double*)calloc(pcm.length, sizeof(double));
	te = pcm.fs * 2;

	m = 0; 
	for (int n = 0; n < pcm.length; ++n)
	{
		if (m < te)
		{
			e[n] = (double)m / te; 
		}
		else 
		{ 
			e[n] = 1.0 - ((double)m - te) / te;
		}
		++m;
		if (m >= te * 2) 
		{
			m = 0; 
		}
	}

	for (int n = 0; n < pcm.length; ++n)
	{
		pcm.s[n] *= e[n] * gain; 
	}

	wave_write_16bit_mono(&pcm, "ex8_5.wav");

	free(pcm.s);
	free(e);

	return 0;
}