/*
https://projecthub.arduino.cc/abhilashpatel121/easyfft-fast-fourier-transform-fft-for-arduino-03724d
https://microkontroller.ru/arduino-projects/bystroe-preobrazovanie-fure-fft-na-arduino-s-vysokoj-skorostyu/
*/

// Предполагается модификация с целью ускорения

// #include "fft.h"
#include <inttypes.h>

//---------------------------------------------------------------------------//
uint8_t  sine_data[91] =
{
0,
4,    9,    13,   18,   22,   27,   31,   35,   40,   44,
49,   53,   57,   62,   66,   70,   75,   79,   83,   87,
91,   96,   100,  104,  108,  112,  116,  120,  124,  127,
131,  135,  139,  143,  146,  150,  153,  157,  160,  164,
167,  171,  174,  177,  180,  183,  186,  189,  192,  195,
198,  201,  204,  206,  209,  211,  214,  216,  219,  221,
223,  225,  227,  229,  231,  233,  235,  236,  238,  240,
241,  243,  244,  245,  246,  247,  248,  249,  250,  251,
252,  253,  253,  254,  254,  254,  255,  255,  255,  255
};
//---------------------------------------------------------------------------//

float sin(int16_t i)
{
  int16_t j = i;
  float  out;
  while (j < 0) { j = j + 360; }
  while (j > 360) { j = j - 360; }
  if (j > -1 && j < 91) { out = sine_data[j]; }
  else if (j > 90 && j < 181) { out = sine_data[180 - j]; }
  else if (j > 180 && j < 271) { out = -sine_data[j - 180]; }
  else if (j > 270 && j < 361) { out = -sine_data[360 - j]; }
  return (out / 255);
}

float cos(int16_t i)
{
  return sin(i - 90);
}

//-----------------------------FFT Function----------------------------------------------//

void fft(int16_t in[], int16_t out[], int16_t N)
{
  /*
  1. in[]     : Data
  2. N        : 2,4,8,16,32,64,128...
  3. Frequency: SPS
  */

  uint16_t data[13] = { 1,2,4,8,16,32,64,128,256,512,1024,2048 };
  int16_t a, c1, f, o = 0, x;
  a = N;

  for (int16_t i = 0; i < 12; i++)                 // расчет уровней
  {
    if (data[i] <= a) { o = i; }
  }

  int16_t in_ps[data[o]] = {};
  float re[data[o]] = {};
  float  im[data[o]] = {};

  x = 0;
  for (int16_t b = 0; b < o; b++)                     // bit reversal
  {
    c1 = data[b];
    f = data[o] / (c1 + c1);
    for (int16_t j = 0; j < c1; j++) {
      x = x + 1;
      in_ps[x] = in_ps[j] + f;
    }
  }


  for (int16_t i = 0; i < data[o]; i++)            // обновить входной массив в соответствии с обратным порядком бит
  {
    if (in_ps[i] < a) {
      re[i] = in[in_ps[i]];
    }
    if (in_ps[i] > a) {
      re[i] = in[in_ps[i] - a];
    }
  }


  int16_t i10, i11, n1;
  float e, c, s, tr, ti;

  for (int16_t i = 0; i < o; i++)                                    //fft
  {
    i10 = data[i];              // общие значения синуса/косинуса:
    i11 = data[o] / data[i + 1];    // цикл с подобным синусом-косинусом:
    // i11 = 1 << (o - i - 1);
    e = 360 / data[i + 1];
    e = 0 - e;
    n1 = 0;

    for (int16_t j = 0; j < i10; j++) {
      c = cos(e * j);
      s = sin(e * j);
      n1 = j;

      for (int16_t k = 0; k < i11; k++) {
        tr = c * re[i10 + n1] - s * im[i10 + n1];
        ti = s * re[i10 + n1] + c * im[i10 + n1];

        re[n1 + i10] = re[n1] - tr;
        re[n1] = re[n1] + tr;

        im[n1 + i10] = im[n1] - ti;
        im[n1] = im[n1] + ti;

        n1 = n1 + i10 + i10;
      }
    }
  }

  for (int16_t i = 0; i < data[o]; i++) {
    out[i] = re[i] + im[i];  // Амплитуда
  }
}

//------------------------------------------------------------------------------------//
