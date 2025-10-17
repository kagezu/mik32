// #include "fft.h"
#include <inttypes.h>
#include "fixmath.h"
#include "int8.h"

//---------------------------------------------------------------------------//

// количество точек должно быть >= N
constexpr static uint16_t RAM_RO sin_lut[] = {
   0,   25,   50,   75,  101,  126,  151,  176,  201,  226,  251,  276,  301,  326,  351,  376,
 401,  426,  451,  476,  501,  526,  551,  576,  601,  626,  651,  675,  700,  725,  750,  774,
 799,  824,  848,  873,  897,  922,  946,  971,  995, 1020, 1044, 1068, 1092, 1117, 1141, 1165,
1189, 1213, 1237, 1261, 1285, 1309, 1332, 1356, 1380, 1404, 1427, 1451, 1474, 1498, 1521, 1544,
1567, 1591, 1614, 1637, 1660, 1683, 1706, 1729, 1751, 1774, 1797, 1819, 1842, 1864, 1886, 1909,
1931, 1953, 1975, 1997, 2019, 2041, 2062, 2084, 2106, 2127, 2149, 2170, 2191, 2213, 2234, 2255,
2276, 2296, 2317, 2338, 2359, 2379, 2399, 2420, 2440, 2460, 2480, 2500, 2520, 2540, 2559, 2579,
2598, 2618, 2637, 2656, 2675, 2694, 2713, 2732, 2751, 2769, 2788, 2806, 2824, 2843, 2861, 2878,
2896, 2914, 2932, 2949, 2967, 2984, 3001, 3018, 3035, 3052, 3068, 3085, 3102, 3118, 3134, 3150,
3166, 3182, 3198, 3214, 3229, 3244, 3260, 3275, 3290, 3305, 3320, 3334, 3349, 3363, 3378, 3392,
3406, 3420, 3433, 3447, 3461, 3474, 3487, 3500, 3513, 3526, 3539, 3551, 3564, 3576, 3588, 3600,
3612, 3624, 3636, 3647, 3659, 3670, 3681, 3692, 3703, 3713, 3724, 3734, 3745, 3755, 3765, 3775,
3784, 3794, 3803, 3812, 3822, 3831, 3839, 3848, 3857, 3865, 3873, 3881, 3889, 3897, 3905, 3912,
3920, 3927, 3934, 3941, 3948, 3954, 3961, 3967, 3973, 3979, 3985, 3991, 3996, 4002, 4007, 4012,
4017, 4022, 4027, 4031, 4036, 4040, 4044, 4048, 4052, 4055, 4059, 4062, 4065, 4068, 4071, 4074,
4076, 4079, 4081, 4083, 4085, 4087, 4088, 4090, 4091, 4092, 4093, 4094, 4095, 4095, 4096, 4096,
4096
};

// У нас pi/2 = 1
constexpr int32_t PI_2 = sizeof(sin_lut) / 2 - 1; // Q32.E
constexpr int32_t PI = PI_2 << 1;                 // Q32.E
constexpr int32_t PIx2 = PI_2 << 2;               // Q32.E
constexpr int32_t E = ilog2(sin_lut[PI_2]);       // Дробная часть sin/cos
constexpr int32_t W = ilog2(PIx2);                // Дробная часть аргумента sin/cos

constexpr static inline int32_t sin(int32_t alpha)  // Q32.E sin( Q32.W )
{
  const int32_t betta = alpha & (PI_2 - 1);
  switch ((alpha >> ilog2(PI_2)) & 0b11) {
    case 0b00: return sin_lut[betta];
    case 0b01: return sin_lut[PI_2 - betta];
    case 0b10: return -sin_lut[betta];
    case 0b11: return -sin_lut[PI_2 - betta];
  }
  return 0;
}

constexpr static inline int32_t cos(int32_t alpha) // Q32.E cos( Q32.W )
{
  const int32_t betta = alpha & (PI_2 - 1);
  switch ((alpha >> ilog2(PI_2)) & 0b11) {
    case 0b00: return sin_lut[PI_2 - betta];
    case 0b01: return -sin_lut[betta];
    case 0b10: return -sin_lut[PI_2 - betta];
    case 0b11: return sin_lut[betta];
  }
  return 0;
}

//-----------------------------FFT Class-------------------------------------//

template<const int32_t S, const int32_t M>
class FFT {
  typedef int32_t  int_t;

protected:
  constexpr static int32_t N(int32_t pow = 0) { return 1 << (ilog2(S) + pow); }
  int16_t in[N(-2)] = {};
  int_t real[N()];
  int_t imag[N()];

public:
  // Создание таблицы с перевёрнутыми битами
  void init()
  {
    // Переворот битов.
    int32_t x = 0;
    for (int8_t bit = 0; bit < ilog2(S) - 2; bit++) {
      const int32_t c1 = 1 << bit;
      const int32_t reverse = N(-3 - bit);
      for (int16_t j = 0; j < c1; j++)
        in[++x] = in[j] + reverse;
    }
  }

  template<typename T>
  void run(T *input)
  {
    // Определяем количество свободных бит, и используем их для повышения точности
    constexpr int32_t D = (sizeof(int_t) << 3);   // Битов в используемом типе
    constexpr int32_t P = D - M - E - ilog2(N(-2)); // Остаток под дробную часть
    // constexpr int32_t norm = ilog2(N(-2)) + P;      // Нормализация амплитуды результатов
    constexpr int32_t norm = D - M - E;             // Нормализация амплитуды результатов
    // static_assert(P < 0);
    (void)(1 << P); // Потеря значения

    // Быстрый расчет FFT для 4-точечного сигнала.
    // Основан на простоте 4-точечной синусоиды.
    // При N = 512, экономия ~10% времени.
    for (int i = 0; i < N(-2); i++) {             // Q32.P
      const int_t x0 = (int_t)input[in[i]] << P;
      const int_t x1 = (int_t)input[in[i] + N(-2)] << P;
      const int_t x2 = (int_t)input[in[i] + N(-1)] << P;
      const int_t x3 = (int_t)input[in[i] + N(-2) + N(-1)] << P;

      real[(i << 2) + 0] = x0 + x1 + x2 + x3;
      imag[(i << 2) + 0] = 0;
      real[(i << 2) + 1] = x0 - x2;
      imag[(i << 2) + 1] = -x1 + x3;
      real[(i << 2) + 2] = x0 - x1 + x2 - x3;
      imag[(i << 2) + 2] = 0;
      real[(i << 2) + 3] = x0 - x2;
      imag[(i << 2) + 3] = x1 - x3;
    }

    for (int i = 2; i < ilog2(S); i++) {
      // Количество блоков DFT в итерации: N/2 = p·q
      const int32_t q = 1 << i;   // q = 2^i; количество комплексных корней из 1
      const int32_t p = N(-1 - i);// p = N/2 / q; вызовов DFT для каждого корня
      // Будем считать аргумент как j·2^w; => w = log2(π/q) = log2 π - i;
      // Таким образом мы избавились от операций деления и последующих умножений.
      const int32_t w = ilog2(PI) - i; // w = W - i - 1;

      // реализация бабочки
      for (int j = 0; j < q; j++) {
        const int32_t c = cos(j << w); // Q32.E cos( Q32.W )
        const int32_t s = sin(j << w);
        const int32_t q2 = q << 1;
        int32_t n = j;

        for (int k = 0; k < p; k++) {
          const int_t re = (c * real[n + q] + s * imag[n + q]) >> E; // Q32.P -> Q32.PE -> Q32.P
          const int_t im = (c * imag[n + q] - s * real[n + q]) >> E;
          real[n + q] = real[n] - re;
          imag[n + q] = imag[n] - im;
          real[n] += re;
          imag[n] += im;
          n += q2;
        }
      }
    }

    // Преобразование к квадрату  амплитуды.
    for (int i = 0; i < N(-1); i++) {
      const int32_t re = real[i] >> norm; // Q32.PN -> Q32
      const int32_t im = imag[i] >> norm;
      real[i] = re * re + im * im;
    }
  }

  void sum()
  {
    constexpr int32_t half = N(-1);
    int_t direct = 0;
    int_t revers = 0;
    for (int i = 0; i < half; i++) {
      if (real[i] < real[i + 1]) { direct += real[i]; real[i] = 0; }
      else { real[i] += direct; direct = 0; }
      if (real[half - i] < real[half - i - 1]) { revers += real[half - i]; real[half - i] = 0; }
      else { real[half - i] += revers; revers = 0; }
    }
  }

  template<typename T>
  void sqrt(T *output)
  {
    for (int i = 0; i < N(-1); i++) output[i] = fix16_sqrt(real[i]) >> 8;// Q32 -> Q32.8 -> Q32
  }

  template<typename T>
  void sqrt(T *output, int len)
  {
    int32_t k = (N(-1) << 8) / len;
    for (int i = 0; i < len; i++) output[i] = fix16_sqrt(real[(i * k) >> 8]) >> 8;
  }

  template<typename T>
  void log(T *output)
  {
    // constexpr int32_t D = (sizeof(int32_t) << 3);   // Бит в используемом типе
    // for (int i = 0; i < N(-1); i++) output[i] = (ilog2(real[i])) << (M - ilog2(D));// тоже самое только в профиль =)
    for (int i = 0; i < N(-1); i++) output[i] = (ilog2(real[i])) << (M - ilog2(M) - 2);
  }

  int32_t *square() { return real; }
};
