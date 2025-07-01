/*
https://projecthub.arduino.cc/abhilashpatel121/easyfft-fast-fourier-transform-fft-for-arduino-03724d
https://microkontroller.ru/arduino-projects/bystroe-preobrazovanie-fure-fft-na-arduino-s-vysokoj-skorostyu/
*/

// Переписано с использованием фиксированной точки

// #include "fft.h"
#include <inttypes.h>
#include <math.h>

//---------------------------------------------------------------------------//

static const uint8_t sine_data[] = { // Q8.8
  0, 2, 3, 5, 6, 8, 9, 11,
  13, 14, 16, 17, 19, 20, 22, 23,
  25, 27, 28, 30, 31, 33, 34, 36,
  37, 39, 41, 42, 44, 45, 47, 48,
  50, 51, 53, 54, 56, 57, 59, 60,
  62, 63, 65, 67, 68, 70, 71, 73,
  74, 76, 77, 79, 80, 81, 83, 84,
  86, 87, 89, 90, 92, 93, 95, 96,
  98, 99, 100, 102, 103, 105, 106, 108,
  109, 110, 112, 113, 115, 116, 117, 119,
  120, 122, 123, 124, 126, 127, 128, 130,
  131, 132, 134, 135, 136, 138, 139, 140,
  142, 143, 144, 146, 147, 148, 149, 151,
  152, 153, 154, 156, 157, 158, 159, 161,
  162, 163, 164, 165, 167, 168, 169, 170,
  171, 172, 174, 175, 176, 177, 178, 179,
  180, 181, 183, 184, 185, 186, 187, 188,
  189, 190, 191, 192, 193, 194, 195, 196,
  197, 198, 199, 200, 201, 202, 203, 204,
  205, 206, 207, 208, 208, 209, 210, 211,
  212, 213, 214, 215, 215, 216, 217, 218,
  219, 220, 220, 221, 222, 223, 223, 224,
  225, 226, 226, 227, 228, 228, 229, 230,
  231, 231, 232, 232, 233, 234, 234, 235,
  236, 236, 237, 237, 238, 238, 239, 240,
  240, 241, 241, 242, 242, 243, 243, 244,
  244, 244, 245, 245, 246, 246, 247, 247,
  247, 248, 248, 248, 249, 249, 249, 250,
  250, 250, 251, 251, 251, 252, 252, 252,
  252, 252, 253, 253, 253, 253, 253, 254,
  254, 254, 254, 254, 254, 254, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255
};


int sinx(int a) // Q16.8 sin( Q16.10 )
{
  const int16_t b = a & 0xff;
  switch ((a >> 8) & 0b11) {
    case 0b00: return sine_data[b];
    case 0b01: return sine_data[0xff - b];
    case 0b10: return -(int)sine_data[b];
    case 0b11: return -(int)sine_data[0xff - b];
  }
  return 0;
}

int cosx(int a) // Q16.8 cos( Q16.10 )
{
  const int16_t b = a & 0xff;
  switch ((a >> 8) & 0b11) {
    case 0b00: return sine_data[0xff - b];
    case 0b01: return -(int)sine_data[b];
    case 0b10: return -(int)sine_data[0xff - b];
    case 0b11: return sine_data[b];
  }
  return 0;
}

//---------------------------------------------------------------------------//

template<typename U>
constexpr static inline uint8_t log2n(U x)
{
  uint8_t res = -1;
  while (x) { x >>= 1; res++; }
  return res;
}

//-----------------------------FFT Class-------------------------------------//

template< const int N>
class FFT {
protected:
  int16_t *data;
  int16_t o;

  int16_t in_ps[1 << log2n(N)] = {};
  float   re[1 << log2n(N)];
  float   im[1 << log2n(N)];


public:
  FFT()
  {
    int16_t c1, f, x = 0;
    o = log2n(N);

    for (int8_t b = 0; b < o; b++)    // bit reversal
    {
      c1 = 1 << b;
      f = (1 << o) / (c1 + c1);
      for (int16_t j = 0; j < c1; j++) {
        x = x + 1;
        in_ps[x] = in_ps[j] + f;
      }
    }
  }

  void run(int16_t *data)
  {

    // обновить входной массив в соответствии с обратным порядком бит
    for (int16_t i = 0; i < 1 << o; i++) {
      if (in_ps[i] < N) {
        re[i] = data[in_ps[i]];
      }
      im[i] = 0;
    }



    int16_t i10, i11, n1;
    float e, c, s, tr, ti;

    for (int16_t i = 0; i < o; i++)      //fft
    {
      i10 = 1 << i;              // общие значения синуса/косинуса
      i11 = 1 << (o - i - 1);
      e = 0x400 / (1 << (i + 1));
      e = 0 - e;

      for (int16_t j = 0; j < i10; j++) {
        c = cosx(e * j) / 255.0;
        s = sinx(e * j) / 255.0;
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

    for (int16_t i = 0; i < 1 << (o - 1); i++) {
      data[i] = (uint32_t)sqrt(re[i] * re[N - i] - im[i] * im[N - i]) >> ((o >> 1) + 2);  // Амплитуда
    }
  }


};

//------------------------------------------------------------------------------------//
