#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

int main(void)
{
  STEREO_PCM pcm0, pcm1;
  int n;
  
  wave_read_16bit_stereo(&pcm0, "a.wav"); /* ���f�[�^�̓��� */
  
  pcm1.fs = pcm0.fs; /* �W�{�����g�� */
  pcm1.bits = pcm0.bits; /* �ʎq�����x */
  pcm1.length = pcm0.length; /* ���f�[�^�̒��� */
  pcm1.sL = calloc(pcm1.length, sizeof(double)); /* ���f�[�^�i���j */
  pcm1.sR = calloc(pcm1.length, sizeof(double)); /* ���f�[�^�i�E�j */
  
  for (n = 0; n < pcm1.length; n++)
  {
    pcm1.sL[n] = pcm0.sL[n]; /* ���f�[�^�i���j �̃R�s�[ */
    pcm1.sR[n] = pcm0.sR[n]; /* ���f�[�^�i�E�j�̃R�s�[ */
  }
  
  wave_write_16bit_stereo(&pcm1, "b.wav"); /* ���f�[�^�̏o�� */
  
  free(pcm0.sL);
  free(pcm0.sR);
  free(pcm1.sL);
  free(pcm1.sR);
  
  return 0;
}
