#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
#include <conio.h>

#define NUMBER_OF_BUFFER 8 /* バッファの数 */
#define BUFFER_SIZE 160 /* バッファのサイズ */

int main(void)
{
  char key;
  int n, in0, in1, out0, out1, flag;
  short s[BUFFER_SIZE]; /* 音データ */
  
  short in_buffer[NUMBER_OF_BUFFER][BUFFER_SIZE]; /* 入力バッファ */
  WAVEHDR in_hdr[NUMBER_OF_BUFFER]; /* 入力バッファのヘッダ */
  HWAVEIN in_hdl = 0; /* サウンドデバイスのハンドル */
  
  short out_buffer[NUMBER_OF_BUFFER][BUFFER_SIZE]; /* 出力バッファ */
  WAVEHDR out_hdr[NUMBER_OF_BUFFER]; /* 出力バッファのヘッダ */
  HWAVEOUT out_hdl = 0; /* サウンドデバイスのハンドル */
  
  WAVEFORMATEX wave_format_ex = {WAVE_FORMAT_PCM, /* PCM */
                                 1, /* モノラル */
                                 8000, /* 標本化周波数（8000Hz） */
                                 16000, /*1秒あたりの音データのサイズ（16000byte） */
                                 2, /* 音データの最小単位（2byte） */
                                 16, /* 量子化精度（16bit） */
                                 0 /* オプション情報のサイズ（0byte） */
                                };
  
  /* サウンドデバイスのオープン */
   waveInOpen(&in_hdl, 0, &wave_format_ex, 0, 0, CALLBACK_NULL);
  
  /* 0を書き込むことでin_hdrをクリアする */
  for (in0 = 0; in0 < NUMBER_OF_BUFFER; in0++)
  {
    memset(&in_hdr[in0], 0, sizeof(WAVEHDR));
  }
  
  /* サウンドデバイスのオープン */
  waveOutOpen(&out_hdl, 0, &wave_format_ex, 0, 0, CALLBACK_NULL);
  
  /* サウンドデバイスの一時停止 */
  waveOutPause(out_hdl);
  
  /* 0を書き込むことでout_hdrをクリアする */
  for (out0 = 0; out0 < NUMBER_OF_BUFFER; out0++)
  {
    memset(&out_hdr[out0], 0, sizeof(WAVEHDR));
  }
  
  in0 = 0;
  in1 = 0;
  out0 = 0;
  out1 = 0;
  flag = 0;
  
  while (1) /* 無限ループ */
  {
    if (flag == 1) /* flagが1のとき */
    {
      /* 音データの出力 */
      if (out0 < NUMBER_OF_BUFFER)
      {
        /* 出力バッファに音データを書き込む */
        for (n = 0; n < BUFFER_SIZE; n++)
        {
          out_buffer[out0][n] = s[n];
        }
        
        flag = 0; /* flagを0にする */
        
        /* 出力バッファの設定 */
        out_hdr[out0].lpData = (char *)out_buffer[out0]; /* 出力バッファの指定 */
        out_hdr[out0].dwBufferLength = BUFFER_SIZE * 2; /* 出力バッファのサイズ */
        out_hdr[out0].dwFlags = 0; /* フラグのクリア */
        
        /* 出力バッファのロック */
        waveOutPrepareHeader(out_hdl, &out_hdr[out0], sizeof(WAVEHDR));
        
        /* 出力バッファを出力待ちキューに追加する */
        waveOutWrite(out_hdl, &out_hdr[out0], sizeof(WAVEHDR));
        
        out0++;
        if (out0 == NUMBER_OF_BUFFER)
        /* すべての出力バッファが出力待ちキューに格納されたとき */
        {
          /* 音データの出力を開始する */
          waveOutRestart(out_hdl);
        }
      }
      else
      {
        if ((out_hdr[out1].dwFlags & WHDR_DONE) != 0)
        /* 出力バッファのおわりまで音データが再生されたとき */
        {
          /* 出力バッファに音データを書き込む */
          for (n = 0; n < BUFFER_SIZE; n++)
          {
            out_buffer[out1][n] = s[n];
          }
          
          flag = 0; /* flagを0にする */
          
          /* 出力バッファのアンロック */
          waveOutUnprepareHeader(out_hdl, &out_hdr[out1], sizeof(WAVEHDR));
          
          /* 出力バッファの設定 */
          out_hdr[out1].lpData = (char *)out_buffer[out1]; /* 出力バッファの指定 */
          out_hdr[out1].dwBufferLength = BUFFER_SIZE * 2; /* 出力バッファのサイズ */
          out_hdr[out1].dwFlags = 0; /* フラグのクリア */
          
          /* 出力バッファのロック */
          waveOutPrepareHeader(out_hdl, &out_hdr[out1], sizeof(WAVEHDR));
          
          /* 出力バッファを出力待ちキューに追加する */
          waveOutWrite(out_hdl, &out_hdr[out1], sizeof(WAVEHDR));
          
          out1++;
          if (out1 == NUMBER_OF_BUFFER)
          {
            out1 = 0; /* out1を0に戻し，出力バッファを再利用する */
          }
        }
      }
    }
    
    /* 音データの入力 */
    if (in0 < NUMBER_OF_BUFFER)
    {
      /* 入力バッファの設定 */
      in_hdr[in0].lpData = (char *)in_buffer[in0]; /* 入力バッファの指定 */
      in_hdr[in0].dwBufferLength = BUFFER_SIZE * 2; /* 入力バッファのサイズ */
      in_hdr[in0].dwFlags = 0; /* フラグのクリア */
      
      /* 入力バッファのロック */
      waveInPrepareHeader(in_hdl, &in_hdr[in0], sizeof(WAVEHDR));
      
      /* 入力バッファを入力待ちキューに追加する */
      waveInAddBuffer(in_hdl, &in_hdr[in0], sizeof(WAVEHDR));
      
      in0++;
      if (in0 == NUMBER_OF_BUFFER)
      /* すべての入力バッファが入力待ちキューに追加されたとき */
      {
        /* 音データの入力を開始する */
        waveInStart(in_hdl);
      }
    }
    else
    {
      if ((in_hdr[in1].dwFlags & WHDR_DONE) != 0)
      /* 入力バッファのおわりまで音データが録音されたとき */
      {
        /* 入力バッファのアンロック */
        waveInUnprepareHeader(in_hdl, &in_hdr[in1], sizeof(WAVEHDR));
        
        /* 入力バッファから音データを読み取る */
        for (n = 0; n < BUFFER_SIZE; n++)
        {
          s[n] = in_buffer[in1][n];
        }
        
        flag = 1; /* flagを1にする */
        
        /* 入力バッファの設定 */
        in_hdr[in1].lpData = (char *)in_buffer[in1]; /* 入力バッファの指定 */
        in_hdr[in1].dwBufferLength = BUFFER_SIZE * 2; /* 入力バッファのサイズ */
        in_hdr[in1].dwFlags = 0; /* フラグのクリア */
        
        /* 入力バッファのロック */
        waveInPrepareHeader(in_hdl, &in_hdr[in1], sizeof(WAVEHDR));
        
        /* 入力バッファを入力待ちキューに追加する */
        waveInAddBuffer(in_hdl, &in_hdr[in1], sizeof(WAVEHDR));
        
        in1++;
        if (in1 == NUMBER_OF_BUFFER)
        {
          in1 = 0; /* in1を0に戻し，入力バッファを再利用する */
        }
      }
    }
    
    if (kbhit()) /* キーボードから入力があるとき */
    {
      key = getch(); /* キーのチェック */
      if (key == 13) /* Enterキーが押されたとき */
      {
        waveInStop(in_hdl); /* 音データの入力を停止する */
        for (in0 = 0; in0 < NUMBER_OF_BUFFER; in0++)
        {
          /* 入力バッファのアンロック */
          if ((in_hdr[in0].dwFlags & WHDR_PREPARED) != 0)
          {
            waveInUnprepareHeader(in_hdl, &in_hdr[in0], sizeof(WAVEHDR ));
          }
        }
        waveInClose(in_hdl); /* サウンドデバイスのクローズ */
        
        waveOutPause(out_hdl); /* 音データの出力を停止する */
        for (out0 = 0; out0 < NUMBER_OF_BUFFER; out0++)
        {
          /* 出力バッファのアンロック */
          if ((out_hdr[out0].dwFlags & WHDR_PREPARED) != 0)
          {
            waveOutUnprepareHeader(out_hdl, &out_hdr[out0], sizeof(WAVEHDR));
          }
        }
        waveOutClose(out_hdl); /* サウンドデバイスのクローズ */
        
        return 0;
      }
    }
    
    Sleep(1); /* 1msのスリープをはさむことで，プログラムの実行速度を調節する */
  }
}
