#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "adsr.h"

int main(void)
{
	MONO_PCM pcm;
	int A, D, R, gate, duration;
	double fc, fm, ratio, gain, S;
	double *ac, *am;

	pcm.fs = 44100;
	pcm.bits = 16;
	pcm.length = pcm.fs * 4; 
	pcm.s = (double*)calloc(pcm.length, sizeof(double));

	ac = (double*)calloc(pcm.length, sizeof(double));
	am = (double*)calloc(pcm.length, sizeof(double));

	gate = pcm.fs * 4; 
	duration = pcm.fs * 4; 
	A = 0; 
	D = pcm.fs * 4; 
	S = 0.0; 
	R = pcm.fs * 4; 
	ADSR(ac, A, D, S, R, gate, duration);

	fc = 440.0; 

	gate = pcm.fs * 4; 
	duration = pcm.fs * 4; 
	A = 0; 
	D = pcm.fs * 2; 
	S = 0.0; 
	R = pcm.fs * 2; 
	ADSR(am, A, D, S, R, gate, duration);

	ratio = 3.5; 
	fm = fc * ratio;

	for (int n = 0; n < pcm.length; ++n)
	{
		pcm.s[n] = ac[n] * sin(2.0 * M_PI * fc * n / pcm.fs + am[n] * sin(2.0 * M_PI * fm * n / pcm.fs));
	}

	gain = 0.1; 

	for (int n = 0; n < pcm.length; ++n)
	{
		pcm.s[n] *= gain;
	}

	wave_write_16bit_mono(&pcm, "ex10_4.wav");

	free(pcm.s);
	free(ac);
	free(am);

	return 0;
}
