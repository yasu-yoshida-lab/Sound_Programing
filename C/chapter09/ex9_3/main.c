#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "iir_filter.h"

int main(void)
{
	MONO_PCM pcm0, pcm1;
	int t0, I, J;
	double vco, gain, am, fm, Q;
	double *vcf = NULL;
	double a[3], b[3];

	pcm0.fs = 44100;
	pcm0.bits = 16;
	pcm0.length = pcm0.fs * 2;
	pcm0.s = (double*)calloc(pcm0.length, sizeof(double));

	vco = 500.0;

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

	vcf[0] = 1000.0;
	am = 800.0;
	fm = 2.0;

	for (int n = 0; n < pcm0.length; ++n)
	{
		vcf[n] = vcf[0] + am * sin(2.0 * M_PI * fm * n / pcm0.fs);
	}

	Q = 5.0;
	I = 2; 
	J = 2;

	pcm1.fs = pcm0.fs;
	pcm1.bits = pcm0.bits;
	pcm1.length = pcm0.length;
	pcm1.s = (double*)calloc(pcm1.length, sizeof(double));

	for (int n = 0; n < pcm1.length; ++n)
	{
		IIR_LPF(vcf[n] / pcm1.fs, Q, a, b);
		for (int m = 0; m <= J; ++m)
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

	gain = 0.1;

	for (int n = 0; n < pcm1.length; ++n)
	{
		pcm1.s[n] *= gain; 
	}

	wave_write_16bit_mono(&pcm1, "ex9_3.wav");

	free(pcm0.s);
	free(pcm1.s);
	free(vcf);

	return 0;
}
