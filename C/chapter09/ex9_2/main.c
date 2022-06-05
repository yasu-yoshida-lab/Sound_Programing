#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
	MONO_PCM pcm;
	int t0 = 0;
	double *vco;
	double gain, am, fm;

	pcm.fs = 44100;
	pcm.bits = 16;
	pcm.length = pcm.fs * 2;
	pcm.s = (double*)calloc(pcm.length, sizeof(double));

	vco = (double*)calloc(pcm.length, sizeof(double));

	vco[0] = 500;
	am = 100.0;
	fm = 2.0;

	for (int n = 0; n < pcm.length; ++n)
	{
		vco[n] = vco[0] + am * sin(2.0 * M_PI * fm * n / pcm.fs);
	}

	t0 = pcm.fs / vco[0];
	int m = 0; 
	for (int n = 0; n < pcm.length; ++n)
	{
		pcm.s[n] = 1.0 - 2.0 * m / t0;
		++m;
		if (m >= t0)
		{
			t0 = pcm.fs / vco[n];
			m = 0; 
		}
	}

	gain = 0.1;

	for (int n = 0; n < pcm.length; ++n)
	{
		pcm.s[n] *= gain;
	}

	wave_write_16bit_mono(&pcm, "ex9_2.wav");

	free(pcm.s);
	free(vco);

	return 0;
}
