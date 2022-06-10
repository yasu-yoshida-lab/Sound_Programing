#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

int main(void) 
{ 
	MONO_PCM pcm0, pcm1; 

	wave_read_16bit_mono(&pcm0, "vocal.wav"); 

	wave_write_PCMU_mono(&pcm0, "pcmu.wav"); 

	wave_read_PCMU_mono(&pcm1, "pcmu.wav"); 

	wave_write_16bit_mono(&pcm1, "pcm.wav"); 

	free(pcm0.s); 
	free(pcm1.s); 

	return 0; 
}