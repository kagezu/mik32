#include "config.h"
#include "font/standard_5x8.h"
#include "font/arial_14.h"
#include "SPI.h"
#include "adc.h"
#include "timer.h"
#include "dma.h"

//////////////// Интерполяция Лагранжа n = 3

// static int32_t x0, x1, x2, x3;
// static int32_t d0, d1, d2, d3;
// static int32_t _y0, _y1, _y2, _y3;

// void L_dx(int32_t i)
// {
//   x0 = -i;
//   x1 = 0;
//   x2 = i;
//   x3 = i * 2;

//   i = i * i * i;

//   d0 = -i * 6;
//   d1 = i * 2;
//   d2 = -i * 2;
//   d3 = i * 6;
// }

// void L_y(uint16_t y0, uint16_t y1, uint16_t y2, uint16_t y3)
// {
//   _y0 = y0;
//   _y1 = y1;
//   _y2 = y2;
//   _y3 = y3;
// }

// uint32_t L(int32_t x)
// {
//   int32_t res = 0
//     + (x - x1) * (x - x2) * (x - x3) * _y0 / d0
//     + (x - x0) * (x - x2) * (x - x3) * _y1 / d1
//     + (x - x0) * (x - x1) * (x - x3) * _y2 / d2
//     + (x - x0) * (x - x1) * (x - x2) * _y3 / d3;
//   if (res < 0)res = 0;
//   return (uint32_t)res;
// }

////////////////

//////////////// Интерполяция Лагранжа n = 5

static int32_t x0, x1, x2, x3, x4, x5;
static int32_t d0, d1, d2, d3, d4, d5;
static int32_t _y0, _y1, _y2, _y3, _y4, _y5;

void L_dx(int32_t i)
{
  x0 = -i * 2;
  x1 = -i;
  x2 = 0;
  x3 = i;
  x4 = i * 2;
  x5 = i * 3;

  i = i * i * i * i * i;

  d0 = -i * 120;
  d1 = i * 24;
  d2 = -i * 12;
  d3 = i * 12;
  d4 = -i * 24;
  d5 = i * 120;
}

void L_y(uint16_t y0, uint16_t y1, uint16_t y2, uint16_t y3, uint16_t y4, uint16_t y5)
{
  _y0 = y0;
  _y1 = y1;
  _y2 = y2;
  _y3 = y3;
  _y4 = y4;
  _y5 = y5;
}

uint32_t L(int32_t x)
{
  int32_t res = 0
    + (x - x1) * (x - x2) * (x - x3) * (x - x4) * (x - x5) * (int64_t)_y0 / d0
    + (x - x0) * (x - x2) * (x - x3) * (x - x4) * (x - x5) * (int64_t)_y1 / d1
    + (x - x0) * (x - x1) * (x - x3) * (x - x4) * (x - x5) * (int64_t)_y2 / d2
    + (x - x0) * (x - x1) * (x - x2) * (x - x4) * (x - x5) * (int64_t)_y3 / d3
    + (x - x0) * (x - x1) * (x - x2) * (x - x3) * (x - x5) * (int64_t)_y4 / d4
    + (x - x0) * (x - x1) * (x - x2) * (x - x3) * (x - x4) * (int64_t)_y5 / d5;
  if (res < 0)res = 0;
  return (uint32_t)res;
}

////////////////

#define ADC0(f)   f(1,5)
#define ADC1(f)   f(1,7)
#define ADC3(f)   f(0,4)
#define SAMPLES   160
#define N         5
#define BLACK     RGB(0, 0, 0)
#define LIGHT     RGB(255, 255, 32)
#define BLUE      RGB(32, 32, 128)
#define ADC_V     (uint32_t)&ANALOG_REG->ADC_VALUE  // 0x0008504C

Display lcd;
CSPI SPI;
ADC adc;
DMA dma(0, DMA::VERY);

uint16_t buffer[SAMPLES];
int16_t point[SAMPLES + 240];
int16_t point2[SAMPLES + 240];
volatile uint32_t count;
reg k = 0;
int16_t p = 0;

void GCC_RAM sample()
{
  // adc.init(3, 63);
  adc.init(3, 1);

  // adc.stop();

  // count = 0;
  // T32_1_OVF;

  adc.start();
  while (adc.value() <= 2000);
  while (adc.value() > 2000);
  dma.start();
  dma.wait();
  adc.stop();

  // reg last = point[k];
  // lcd.color(BLACK);
  // for (reg i = 1; i < SAMPLES; i++) {
  //   lcd.w_line(point[i + k] > last ? last : point[i + k], i, point[i + k] > last ? point[i + k] : last);
  //   last = point[i + k];
  // }

  //////////////////////////

  L_dx(N);
  for (reg i = 0; i < SAMPLES / N + 240 / N; i++) {
    L_y(buffer[i], buffer[i + 1], buffer[i + 2], buffer[i + 3], buffer[i + 4], buffer[i + 5]);
    for (reg j = 0; j < N; j++) {
      point[i * N + j] = L(j) >> 5;
    }
  }
  if (p == 0) p = point[0];
  k = 0;
  while (k++ < 240) if (point[k] < p) break;
  while (k++ < 240) if (point[k] >= p) break;
  if (k >= 240) p = point[0];

  //////////////////////////

  // reg  last2 = point[k];
  // lcd.color(LIGHT);
  // for (reg i = 1; i < SAMPLES; i++) {
  //   lcd.w_line(point[i + k] > last2 ? last2 : point[i + k], i, point[i + k] > last2 ? point[i + k] : last2);
  //   last2 = point[i + k];
  // }

  //////////////////////////

  lcd.color(BLUE);
  for (reg i = 0; i < 16; i++)
    for (reg j = 0; j < 13; j++)
      lcd.pixel(j * 10 + 3, i * 10 + 4);

  //////////////////////////

  int16_t last = point2[0];
  int16_t last2 = point[k];
  point2[0] = last2;
  for (reg i = 1; i < SAMPLES; i++) {
    lcd.color(BLACK);
    lcd.w_line(last, i, point2[i]);
    last = point2[i];

    lcd.color(LIGHT);
    lcd.w_line(last2, i, point[i + k]);
    last2 = point[i + k];
    point2[i] = last2;
  }

  //////////////////////////

  /*
    reg last = point[0];
    reg last2 = buffer[0] >> 5;
    point[0] = last2;
    for (reg i = 1; i < SAMPLES; i++) {
      lcd.color(BLACK);
      lcd.w_line(point[i] > last ? last : point[i], i, point[i] > last ? point[i] : last);
      last = point[i];
      point[i] = buffer[i] >> 5;
      lcd.color(LIGHT);
      lcd.w_line(point[i] > last2 ? last2 : point[i], i, point[i] > last2 ? point[i] : last2);
      last2 = point[i];
    }
  */
}

int main(void)
{
  T32_1_PS;
  // T32_1_TOP(94);
  T32_1_TOP(32);
  // T32_1_TOP(120);
  T32_1_E;

  ADC0(ANALOG);
  ADC1(ANALOG);
  ADC3(ANALOG);

  SPI.init();
  lcd.init();
  lcd.font(standard_5x8);
  lcd.clear(BLACK);

  dma.setup(buffer, ADC_V, sizeof(buffer));
  dma.read(DMA::TIMER1, DMA::HALF, DMA::HALF, DMA::IMM, DMA::ACK);
  dma.write(DMA::MEM, DMA::WORD, DMA::WORD, DMA::INC);

  // sei();
  // T32_1_IS;
  while (true)
    sample();
}

extern "C" {
  __attribute__((used, interrupt))
    void isr_handler(void)
  {
    ANALOG_REG->ADC_SINGLE = 1;               //Одиночное преобразование
    buffer[count++] = ANALOG_REG->ADC_VALUE;  // Сохраняем предыдущий результат
    if (count > SAMPLES) T32_1_IC;            // Отключаем прерывание, если буфер заполнен
    T32_1_FC;                                 // Сбрасываем флаги прерываний
    EPIC_C;
  }
}
