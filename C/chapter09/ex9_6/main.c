#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "adsr.h"
#include "iir_filter.h"

int main(void)
{
	MONO_PCM pcm0, pcm1;
	int t0, I, J, A, D, R, gate, duration; 
	double vco, gain, offset, depth, S, Q;
	double *vcf, *vca, a[3], b[3];

	pcm0.fs = 44100;
	pcm0.bits = 16;
	pcm0.length = pcm0.fs * 4;
	pcm0.s = (double*)calloc(pcm0.length, sizeof(double));

	vco = 440.0;
	t0 = pcm0.fs / vco;
	int m = 0; 
	for (int n = 0; n < pcm0.length; ++n)
	{
		pcm0.s[n] = 1.0 - 2.0 * m / t0;
		++m;
		if (m >= t0)
		{
			m = 0; 
		}
	}

	vcf = (double*)calloc(pcm0.length, sizeof(double));
	gate = pcm0.fs * 1;
	duration = pcm0.fs * 4;
	A = 0;
	D = pcm0.fs * 1;
	S = 0.0;
	R = pcm0.fs * 1;
	ADSR(vcf, A, D, S, R, gate, duration);

	offset = 500.0;
	depth = 500.0;

	for (int n = 0; n < pcm0.length; ++n)
	{
		vcf[n] = offset + vcf[n] * depth;
	}

	Q = 1 / sqrt(2);
	I = 2; 
	J = 2;

	pcm1.fs = pcm0.fs;
	pcm1.bits = pcm0.bits;
	pcm1.length = pcm0.length;
	pcm1.s = (double*)calloc(pcm1.length, sizeof(double));
	
	for (int n = 0; n < pcm1.length; ++n)
	{
		IIR_LPF(vcf[n] / pcm1.fs, Q, a, b);
		for (int m = 0;  m <= J; ++m)
		{
			if (n - m >= 0)
			{
				pcm1.s[n] += b[m] * pcm0.s[n - m];
			}
		}
		for (int m = 1; m <= I; ++m)
		{
			if (n - m >= 0)
			{
				pcm1.s[n] += -1 * a[m] * pcm1.s[n - m];
			}
		}
	}

	vca = (double*)calloc(pcm1.length, sizeof(double));
	gate = pcm0.fs * 4;
	duration = pcm0.fs * 4;
	A = 0; 
	D = pcm0.fs * 4;
	S = 0.0;
	R = pcm0.fs * 1;
	ADSR(vca, A, D, S, R, gate, duration);

	gain = 0.1;

	for (int n = 0; n < pcm1.length; ++n)
	{
		pcm1.s[n] *= vca[n] * gain;
	}

	for (int n = 0; n < pcm1.fs * 0.01; ++n)
	{
		pcm1.s[n] *= (double)n / (pcm1.fs * 0.01);
		pcm1.s[pcm1.length - n - 1] *= (double)n / (pcm1.fs * 0.01);
	}

	wave_write_16bit_mono(&pcm1, "ex9_6.wav");

	free(pcm0.s);
	free(pcm1.s);
	free(vcf);
	free(vca);

	return 0;
}