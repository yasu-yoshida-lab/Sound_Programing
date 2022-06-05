#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
	MONO_PCM pcm;
	int t0 = 0;
	double vco, gain, am, fm;
	double *vca = NULL;

	pcm.fs = 44100;
	pcm.bits = 16;
	pcm.length = pcm.fs * 2;
	pcm.s = (double*)calloc(pcm.length, sizeof(double));

	vco = 500.0;
	t0 = pcm.fs / vco;

	int m = 0;
	for (int n = 0; n < pcm.length; ++n)
	{
		pcm.s[n] = 1.0 - 2.0 * m / t0;
		++m;
		if (m >= t0)
		{
			m = 0; 
		}
	}

	vca = (double*)calloc(pcm.length, sizeof(double));


	vca[0] = 1.0;
	am = 0.2;
	fm = 2.0;

	for (int n = 0; n < pcm.length; ++n)
	{
		vca[n] = vca[0] + am * sin(2.0 * M_PI * fm * n / pcm.fs);
	}

	gain = 0.1;

	for (int n = 0; n < pcm.length; ++n)
	{
		pcm.s[n] *= vca[n] * gain;
	}

	wave_write_16bit_mono(&pcm, "ex9_1.wav");

	free(pcm.s);
	free(vca);

	return 0;
}
