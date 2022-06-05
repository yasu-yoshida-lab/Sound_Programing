#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
	MONO_PCM pcm;
	double ac, fc, am, fm, ration, gain;

	pcm.fs = 44100;
	pcm.bits = 16;
	pcm.length = pcm.fs * 1;
	pcm.s = (double*)calloc(pcm.length, sizeof(double));

	ac = 1.0; 
	fc = 500.0;

	am = 1.0; 
	ration = 2.0;
	fm = fc * ration;

	for (int n = 0; n < pcm.length; ++n)
	{
		pcm.s[n] = ac * sin(2.0 * M_PI * fc * n / pcm.fs + am * sin(2.0 * M_PI * fm * n / pcm.fs));
	}

	gain = 0.1;

	for (int n = 0; n < pcm.length; ++n)
	{
		pcm.s[n] *= gain;
	}

	wave_write_16bit_mono(&pcm, "ex10_2.wav");

	free(pcm.s);

	return 0;
}
