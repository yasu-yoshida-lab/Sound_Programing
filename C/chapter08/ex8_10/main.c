#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "window_function.h"
#include "sinc.h"
#include "fir_filter.h"

int main(void)
{
	MONO_PCM pcm0, pcm1;
	int t0 = 0; 
	int J = 0; 
	int ratio = 0;
	int m = 0;
	double *f0, *b, *w;
	double gain, fe, delta; 

	pcm0.fs = 192000;
	pcm0.bits = 16;
	pcm0.length = pcm0.fs * 2;
	pcm0.s = (double*)calloc(pcm0.length, sizeof(double));
	
	f0 = (double*)calloc(pcm0.length, sizeof(double));

	f0[0] = 500.0;
	f0[pcm0.length - 1] = 3500.0;
	for (int n = 0; n < pcm0.length; ++n)
	{
		f0[n] = f0[0] + (f0[pcm0.length - 1] - f0[0]) * n / (pcm0.length - 1);
	}

	t0 = pcm0.fs / f0[0];
	m = 0; 
	for (int n = 0; n < pcm0.length; ++n)
	{
		pcm0.s[n] = 1.0 - 2.0 * m / t0;
		++m;
		if (m >= t0) 
		{
			t0 = pcm0.fs / f0[n];
			m = 0; 
		}
	}

	pcm1.fs = 8000;
	pcm1.bits = 16;
	pcm1.length = pcm1.fs * 2;
	pcm1.s = (double*)calloc(pcm1.length, sizeof(double));

	ratio = pcm0.fs / pcm1.fs;

	fe = 0.45 / ratio;
	delta = 0.1 / ratio ;
	J = (int)(3.1 / delta + 0.5) - 1;

	if (J % 2 == 1) 
	{
		++J;
	}

	b = (double*)calloc((J + 1), sizeof(double));
	w = (double*)calloc((J + 1), sizeof(double));

	Hanning_window(w, (J + 1));

	FIR_LPF(fe, J, b, w);

	for (int n = 0; n < pcm1.length; ++n)
	{
		for (int m = 0; m <= J; ++m)
		{
			if (n * ratio + J / 2 - m >= 0 && n * ratio + J / 2 - m < pcm0.length)
			{
				pcm1.s[n] += b[m] * pcm0.s[n * ratio + J / 2 - m];
			}
		}
	}

	gain = 0.1;
	for (int n = 0; n < pcm1.length; ++n)
	{
		pcm1.s[n] *= gain;
	}

	wave_write_16bit_mono(&pcm1, "ex8_10.wav");
	
	free(pcm0.s);
	free(pcm1.s);
	free(f0);
	free(b);
	free(w);

	return 0;
}
