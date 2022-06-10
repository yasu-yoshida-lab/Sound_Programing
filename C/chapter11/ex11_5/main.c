#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "sinc.h"

int main(void) 
{
	MONO_PCM pcm0, pcm1, pcm2; 
	int template_size, pmin, pmax, p, q, offset0, offset1, ta, tb, N; 
	double rate, rmax, t, pitch; 
	double *x, *y, *r; 

	wave_read_16bit_mono(&pcm0, "sine_500hz.wav"); 

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
	r = (double*)calloc((pmax + 1), sizeof(double)); 

	offset0 = 0; 
	offset1 = 0; 

	while (offset0 + pmax * 2 < pcm0.length) { 

		for (int n = 0; n < template_size; ++n) {
			x[n] = pcm0.s[offset0 + n]; 
		}
 		for (int n = 0; n < template_size; ++n) { 
 			x[n] = pcm0.s[offset0 + n];
 		}
 		rmax = 0.0; 
 		p = pmin; 
 
 		for (int m = pmin; m <= pmax; ++m) { 
 			for (int n = 0; n < template_size; ++n) { 
 				y[n] = pcm0.s[offset0 + m + n]; 
 			}
 			r[m] = 0.0; 
 			for (int n = 0; n < template_size; ++n) { 
 				r[m] += x[n] * y[n]; 
 			}
 			if (r[m] > rmax) { 
 				rmax = r[m]; 
 				p = m; 
 			}
 		}
 
 		for (int n = 0; n < p; ++n) { 
 			pcm1.s[offset1 + n] = pcm0.s[offset0 + n];
 		}
 
 		for (int n = 0; n < p; ++n) { 
 			pcm1.s[offset1 + p + n] = pcm0.s[offset0 + p + n] * (p - n) / p; 
 			pcm1.s[offset1 + p + n] += pcm0.s[offset0 + n] * n / p; 
 		}
 
 		q = (int)(p * rate / (1.0 - rate) + 0.5); 
 		for (int n = p; n < q; ++n) { 
 			if (offset0 + n >= pcm0.length) { 
 				break; 
 			}
 			pcm1.s[offset1 + p + n] = pcm0.s[offset0 + n]; 
 		}
 		
 		offset0 += q; 
 		offset1 += p + q; 

	}

	pitch = 1.0 / rate; 
	
	pcm2.fs = pcm0.fs; 
	pcm2.bits = pcm0.bits; 
	pcm2.length = pcm0.length;
	pcm2.s = (double*)calloc(pcm2.length, sizeof(double)); 

	N = 128; 

 	for (int n = 0; n < pcm2.length; ++n) { 
 		t = pitch * n; 
 		ta = (int)t; 
 
 		if (t == ta) { 
 			tb = ta; 
 		} 
 		else { 
 			tb = ta + 1; 
 		}
 
 		for (int m = tb - N / 2; m <= ta + N / 2; ++m) { 
 			if (m >= 0 && m < pcm1.length) { 
 				pcm2.s[n] += pcm1.s[m] * sinc(M_PI * (t - m)) * (0.5 + 0.5 * cos(2.0 * M_PI * (t - m) / (N * 2 + 1))); 
 			}
 		}
 	}

	wave_write_16bit_mono(&pcm2, "ex11_5.wav"); 

	free(pcm0.s); 
	free(pcm1.s); 
	free(pcm2.s); 
	free(x); 
	free(y); 
	free(r); 

	return 0; 
}
