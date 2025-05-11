#include "config.h"
#include "font/standard_5x8.h"
#include "font/arial_14.h"
#include "adc.h"
#include "timer.h"
#include "dma.h"

#define ADC0(f)   f(1,5)
#define ADC1(f)   f(1,7)
#define ADC3(f)   f(0,4)
#define SAMPLES   140
#define POINTES   600
// Интервал между значениями
#define N         10
#define BLACK     RGB(0, 0, 0)
#define LIGHT     RGB(255, 255, 64)
#define BLUE      RGB(64, 64, 255)
#define ADC_V     (uint32_t)&ANALOG_REG->ADC_VALUE  // 0x0008504C

//////////////// Интерполяция Лагранжа

// Степень полинома Лагранжа (нечётная)
#define L_EXP     9

static uint16_t *yn;
static int32_t l[L_EXP + 1][N];
static int32_t factor;

void L_y(uint16_t *y) { yn = y; }
int32_t fact(int32_t x)
{
  if (x) return x * fact(x - 1);
  return 1;
}

void L_init()
{
  factor = 1;
  for (int32_t i = 0; i < L_EXP / 2; i++) factor *= N;
  for (int32_t n = 0; n <= L_EXP; n++) {
    int64_t d = factor * fact(L_EXP - n) * fact(n) * (((n & 1) << 1) - 1);
    for (int32_t x = 0; x < N; x++) {
      int64_t ls = 1;
      for (int32_t i = 0; i <= L_EXP; i++)
        if (i != n) ls *= x + N * ((L_EXP >> 1) - i);
      l[n][x] = ls / d / N;
    }
  }
}

uint32_t L(int32_t x)
{
  int32_t res = 0;
  for (uint32_t i = 0; i <= L_EXP; i++)
    res += l[i][x] * yn[i];
  res /= factor;
  return (uint32_t)res;
}

////////////////////////////

Display lcd;
ADC adc;
DMA dma(0, DMA::VERY);

uint16_t buffer[SAMPLES];
int16_t point[POINTES];
int16_t point2[POINTES];

void GCC_RAM sample()
{
  adc.init(1, 1);
  adc.start();
  while (adc.value() <= 2000);
  while (adc.value() > 2000);
  dma.start();
  dma.wait();
  adc.stop();

  //////////////////////////

  for (reg i = 0; i < POINTES / N; i++) {
    L_y(&buffer[i]);
    for (reg j = 0; j < N; j++) {
      point[i * N + j] = (L(j) * lcd.max_x()) >> 12;
    }
  }

  int16_t k = 0;
  while (k++ < POINTES - lcd.max_y() - 3) if (point[k + lcd.max_y() / 2] < lcd.max_x() >> 1) break;
  while (k++ < POINTES - lcd.max_y() - 3) if (point[k + lcd.max_y() / 2] > lcd.max_x() >> 1) break;

  //////////////////////////

  lcd.color(BLUE);
  for (reg i = 1; i <= lcd.max_y() / N; i++)
    for (reg j = 1; j <= lcd.max_x() / N; j++)
      lcd.pixel(j * N, i * N);

  //////////////////////////

  int16_t last = point2[0];
  int16_t last2 = point[k];
  point2[0] = last2;
  for (reg i = 1; i <= lcd.max_y(); i++) {
    lcd.color(BLACK);
    lcd.w_line(last, i, point2[i]);
    last = point2[i];

    lcd.color(LIGHT);
    lcd.w_line(last2, i, point[i + k]);
    last2 = point[i + k];
    point2[i] = last2;
  }
}

int main(void)
{
  ADC1(ANALOG);

  T32_1_PS;
  T32_1_TOP(32);
  T32_1_E;

  lcd.init();
  lcd.font(standard_5x8);
  lcd.clear(BLACK);

  dma.setup(buffer, ADC_V, sizeof(buffer));
  dma.read(DMA::TIMER1, DMA::HALF, DMA::HALF, DMA::IMM, DMA::ACK);
  dma.write(DMA::MEM, DMA::WORD, DMA::WORD, DMA::INC);

  L_init();

  while (true) sample();
}
