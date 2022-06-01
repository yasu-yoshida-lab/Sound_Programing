#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

int main(void)
{
	MONO_PCM pcm;
	double gain = 0.1;

	pcm.fs = 44100;
	pcm.bits = 16;
	pcm.length = pcm.fs * 1;
	pcm.s = (double*)calloc(pcm.length, sizeof(double));

	for (int n = 0; n < pcm.length; ++n)
	{
		pcm.s[n] = (double)rand() / RAND_MAX * 2.0 - 1.0;
	}

	for (int n = 0; n < pcm.length; ++n)
	{
		pcm.s[n] *= gain; 
	}

	wave_write_16bit_mono(&pcm, "ex8_4.wav");

	free(pcm.s);

	return 0;
}