/*
https://projecthub.arduino.cc/abhilashpatel121/easyfft-fast-fourier-transform-fft-for-arduino-03724d
https://microkontroller.ru/arduino-projects/bystroe-preobrazovanie-fure-fft-na-arduino-s-vysokoj-skorostyu/
*/

// Переписано с использованием фиксированной точки

// #include "fft.h"
#include <inttypes.h>
#include "fixmath.h"

//---------------------------------------------------------------------------//

// Да! У нас pi/2 = 1, гениальное - просто
constexpr int32_t PIx2 = 0x400; // Q32.8
constexpr int32_t   PI = 0x200; // Q32.8
constexpr int32_t PI_2 = 0x100; // Q32.8

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
  255, 255, 255, 255, 255, 255, 255, 255,
  255
};

static inline int32_t sin(int32_t alpha) // Q32.8 sin( Q32.10 )
{
  const int32_t betta = alpha & (PI_2 - 1);
  switch ((alpha >> 8) & 0b11) {
    case 0b00: return sine_data[betta];
    case 0b01: return sine_data[PI_2 - betta];
    case 0b10: return -(int32_t)sine_data[betta];
    case 0b11: return -(int32_t)sine_data[PI_2 - betta];
  }
  return 0;
}

static inline int32_t cos(int32_t alpha) // Q32.8 cos( Q32.10 )
{
  const int32_t betta = alpha & (PI_2 - 1);
  switch ((alpha >> 8) & 0b11) {
    case 0b00: return sine_data[PI_2 - betta];
    case 0b01: return -(int)sine_data[betta];
    case 0b10: return -(int)sine_data[PI_2 - betta];
    case 0b11: return sine_data[betta];
  }
  return 0;
}

//---------------------------------------------------------------------------//

// Целочисленный логарифм по основанию 2
constexpr int32_t ilog2(int32_t x)
{
  if (x == 0) return -1;
  uint32_t result = 0;
  if (x >= (1 << 16)) { x >>= 8; result += 16; }
  if (x >= (1 << 8)) { x >>= 8; result += 8; }
  if (x >= (1 << 4)) { x >>= 4; result += 4; }
  if (x >= (1 << 2)) { x >>= 2; result += 2; }
  if (x >= (1 << 1)) { result += 1; }
  return result;
}

constexpr inline int32_t pow(const int32_t x, int32_t delta = 0)
{
  return 1 << (ilog2(x) + delta);
}

//-----------------------------FFT Class-------------------------------------//

template<typename T, const int32_t N>
class FFT {
protected:
  T *data;
  int32_t in[pow(N)] = {};
  int32_t real[pow(N)];
  int32_t imag[pow(N)];

public:
  init()
  {
    int32_t x = 0;
    for (int8_t bit = 0; bit < ilog2(N); bit++)  // Переворот битов
    {
      int32_t c1 = 1 << bit;
      int32_t reverse = pow(N, -1 - bit);
      for (int16_t j = 0; j < c1; j++) {
        in[++x] = in[j] + reverse;
      }
    }
  }

  void run(T *input)
  {
    data = input;

    // обновить входной массив в соответствии с обратным порядком бит
    for (int i = 0; i < pow(N); i++) {
      real[i] = (int32_t)data[in[i]];  // Q32
      imag[i] = 0;
    }

    for (int i = 0; i < ilog2(N); i++) //fft
    {
      const int32_t i10 = 1 << i; // общие значения синуса/косинуса
      const int32_t i11 = pow(N, -1 - i);
      const int32_t e = ilog2(PI) - i;

      for (int j = 0; j < i10; j++) {
        const  int32_t c = cos(j << e);  // Q32.8
        const  int32_t s = sin(j << e);  // Q32.8
        int32_t n1 = j;

        for (int k = 0; k < i11; k++) {
          const int32_t _real = (c * real[i10 + n1] + s * imag[i10 + n1]) >> 8;  // Q32
          const int32_t _imag = (c * imag[i10 + n1] - s * real[i10 + n1]) >> 8;  // Q32

          real[n1 + i10] = real[n1] - _real;
          real[n1] = real[n1] + _real;

          imag[n1 + i10] = imag[n1] - _imag;
          imag[n1] = imag[n1] + _imag;

          n1 = n1 + i10 + i10;
        }
      }
    }

    // Преобразование к амплитуде
    constexpr int32_t div = ilog2(N) - 1;
    constexpr int32_t half = pow(N, -1);

    for (int i = 0; i < half; i++) {
      const int32_t re = real[i] >> div;
      const int32_t im = imag[i] >> div;
      real[i] = re * re + im * im;      // квадрат амплитуды

    }
  }

  void sum()
  {
    int32_t direct = 0;
    int32_t revers = 0;
    for (int i = 0; i < half; i++) {
      if (real[i] < real[i + 1]) { direct += real[i]; real[i] = 0; }
      else { real[i] += direct; direct = 0; }
      if (real[half - i] < real[half - i - 1]) { revers += real[half - i]; real[half - i] = 0; }
      else { real[half - i] += revers; revers = 0; }
    }
  }

  void release()
  {
    constexpr int32_t half = pow(N, -1);
    constexpr int32_t div = ilog2(N) - 1;
    for (int i = 0; i < half; i++) data[i] = fix16_sqrt(real[i]) >> div;
  }
};

//------------------------------------------------------------------------------------//
