#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

int main(void)
{
	MONO_PCM pcm0, pcm1; 
	int template_size, pmin, pmax, p, q, offset0, offset1;
	double rate, rmax;
	double *x, *y, *r;

	wave_read_16bit_mono(&pcm0, "sine_1s.wav");

	rate = 0.5;

	pcm1.fs = pcm0.fs; 
	pcm1.bits = pcm0.bits; 
	pcm1.length = (int)(pcm0.length / rate) + 1; 
	pcm1.s = (double*)calloc(pcm1.length, sizeof(double));

	template_size = (int)(pcm1.fs * 0.01);
	pmin = (int)(pcm1.fs * 0.005); 
	pmax = (int)(pcm1.fs * 0.02);

	x = (double*)calloc(template_size, sizeof(double));
	y = (double*)calloc(template_size, sizeof(double));
	r = (double*)calloc(template_size, sizeof(double));

	offset0 = 0; 
	offset1 = 0; 

	while (offset0 + pmax * 2 < pcm0.length) 
	{
		for (int n = 0; n < template_size; ++n)
		{
			x[n] = pcm0.s[offset0 + n];
		}

		rmax = 0.0; 
		p = pmin;

		for (int m = pmin; m < template_size; ++m)
		{
			for (int n = 0; n < template_size; ++n)
			{ 
				y[n] = pcm0.s[offset0 + m + n];
			}
			r[m] = 0.0; 
			for (int n = 0; n < template_size; ++n)
			{
				r[m] += x[n] * y[n];
			}
			if (r[m] > rmax)
			{
				rmax = r[m]; 
				p = m;
			}
		}

		for (int n = 0; n < p; ++n)
		{
			pcm1.s[offset1 + n] = pcm0.s[offset0 + n];
		}
		for (int n = 0; n < p; ++n)
		{
			pcm1.s[offset1 + p + n] = pcm0.s[offset0 + p + n] * (p - n) / p; 
			pcm1.s[offset1 + p + n] += pcm0.s[offset0 + n] * n / p;
		}

		q = (int)(p * rate / (1.0 - rate) + 0.5);

		for (int n = p; n < q; ++n)
		{
			if (offset0 + n >= pcm0.length)
			{
				break; 
			}
			pcm1.s[offset1 + p + n] = pcm0.s[offset0 + n];
		}
		offset0 += q; 
		offset1 += p + q; 
	}

	wave_write_16bit_mono(&pcm1, "ex11_2.wav");

	free(pcm0.s);
	free(pcm1.s);
	free(x);
	free(y);
	free(r);

	return 0;
}
