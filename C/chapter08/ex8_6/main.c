#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

void square_wave(MONO_PCM *pcm, double f0, double gain, int offset, int duration)
{
	int t0 = 0.0;
	int m = 0;
	double *s = (double*)calloc(duration, sizeof(double));
	
	t0 = pcm->fs / f0;

	for (int n = 0; n < duration; ++n)
	{
		if (m < t0 / 2.0)
		{
			s[n] = 1.0;
		}
		else 
		{
			s[n] = -1.0;
		}
		++m;
		if (m >= t0)
		{
			m = 0; 
		}
	}

	for (int n = 0; n < duration; ++n)
	{
		s[n] *= gain;
	}
	
	for (int n = 0; n < duration; ++n) 
	{ 
		pcm->s[offset + n] += s[n];
	}
	free(s);
}

void triangle_wave(MONO_PCM *pcm, double f0, double gain, int offset, int duration)
{
	int t0 = 0.0;
	int m = 0;
	double *s = (double*)calloc(duration, sizeof(double));
	
	t0 = pcm->fs / f0;

	for (int n = 0; n < duration; ++n) 
	{ 
		if (m < t0 / 2.0)
		{ 
			s[n] = -1.0 + 4.0 * m / t0; 
		}
		else 
		{ 
			s[n] = 3.0 - 4.0 * m / t0; 
		}
		++m;
		if (m >= t0) 
		{
			m = 0; 
		}
	}

	for (int n = 0; n < duration; ++n)
	{
		s[n] *= gain; 
	}

	for (int n = 0; n < duration; ++n)
	{
		pcm->s[offset + n] += s[n]; 
	}

	free(s); 

}

void white_noise(MONO_PCM *pcm, double gain, int offset, int duration)
{
	double *s = (double*)calloc(duration, sizeof(double));

	for (int n = 0; n < duration; ++n)
	{
		s[n] = (double)rand() / RAND_MAX * 2.0 - 1.0;
	}

	for (int n = 0; n < duration; ++n)
	{
		s[n] *= gain; 
	}

	for (int n = 0; n < duration; ++n) 
	{
		pcm->s[offset + n] += s[n];
	}

	free(s);
}

int main(void)
{
	MONO_PCM pcm;

	pcm.fs = 44100;
	pcm.bits = 16;
	pcm.length = 7000 * 16;
	pcm.s = (double*)calloc(pcm.length, sizeof(double));

	square_wave(&pcm, 659.26, 0.1, 7000 * 0, 6125); /* E5 */
	square_wave(&pcm, 659.26, 0.1, 7000 * 1, 6125); /* E5 */
	square_wave(&pcm, 659.26, 0.1, 7000 * 3, 6125); /* E5 */
	square_wave(&pcm, 523.25, 0.1, 7000 * 5, 6125); /* C5 */
	square_wave(&pcm, 659.26, 0.1, 7000 * 6, 6125); /* E5 */
	square_wave(&pcm, 783.99, 0.1, 7000 * 8, 6125); /* G5 */
	square_wave(&pcm, 392.00, 0.1, 7000 * 12, 6125); /* G4 */

	/* ベースパート */
	triangle_wave(&pcm, 146.83, 0.2, 7000 * 0, 6125); /* D3 */
	triangle_wave(&pcm, 146.83, 0.2, 7000 * 1, 6125); /* D3 */
	triangle_wave(&pcm, 146.83, 0.2, 7000 * 3, 6125); /* D3 */
	triangle_wave(&pcm, 146.83, 0.2, 7000 * 5, 6125); /* D3 */
	triangle_wave(&pcm, 146.83, 0.2, 7000 * 6, 6125); /* D3 */
	triangle_wave(&pcm, 196.00, 0.2, 7000 * 8, 6125); /* G3 */
	triangle_wave(&pcm, 196.00, 0.2, 7000 * 12, 6125); /* G3 */

	/* パーカッション */
	white_noise(&pcm, 0.1, 7000 * 0, 4000);
	white_noise(&pcm, 0.1, 7000 * 2, 1000);
	white_noise(&pcm, 0.1, 7000 * 3, 4000);
	white_noise(&pcm, 0.1, 7000 * 5, 1000);
	white_noise(&pcm, 0.1, 7000 * 6, 4000);
	white_noise(&pcm, 0.1, 7000 * 8, 4000);
	white_noise(&pcm, 0.1, 7000 * 11, 4000);
	white_noise(&pcm, 0.1, 7000 * 13, 1000);
	white_noise(&pcm, 0.1, 7000 * 14, 1000);
	white_noise(&pcm, 0.1, 7000 * 15, 1000);

	wave_write_16bit_mono(&pcm, "ex8_6.wav");

	free(pcm.s);

	return 0;
}