#include "display.h"
#include "font/standard_5x8.h"
#include "font/arial_14.h"
#include "adc.h"
#include "timer.h"
#include "dma.h"
#include "lagrange.h"
#include "pinout.h"

// #define RGB   RGB12   // 4x4x4 bit
#define RGB   RGB16   // 5x6x5 bit
// #define RGB   RGB18   // 6x6x6 bit

#define Ln        9   // Степень полинома Лагранжа (нечётная)
#define Lx        1  // Интервал между значениями

#define ADC_VALUE (uint32_t)&ANALOG_REG->ADC_VALUE  // 0x0008504C

#define POINTES   1000
#define SAMPLES   (POINTES / Lx + 10)

#define BLACK     RGB(0, 0, 0)
#define RED       RGB(255, 0, 0)
#define WHITE     RGB(255, 255, 255)
#define LIGHT     RGB(255, 255, 64)
#define BLUE      RGB(64, 64, 255)

Display<ILI9486_16<RGB>, RGB, ROT_0> lcd;
ADC adc;
DMA dma(0, DMA::VERY);

uint16_t buffer[SAMPLES];
int16_t point[POINTES];
int16_t point2[POINTES];

// GCC_RAM
void sample()
{
  adc.init(1, 63);
  adc.start();
  // while (adc.value() >= 1000);
  // while (adc.value() < 1000);
  dma.start();
  dma.wait();
  adc.stop();

  //////////////////////////

  int16_t a_max = 0;
  int16_t a_min = 4096;

  // for (reg i = 0; i < POINTES / Lx; i++) {
  //   Ly(&buffer[i]);
  //   for (reg j = 0; j < Lx; j++) {
  //     int16_t l = L(j);
  //     if (a_max < l)a_max = l;
  //     if (a_min > l)a_min = l;
  //     point[i * Lx + j] = ((4096 - l) * lcd.max_y()) >> 12;
  //   }
  // }

  for (reg i = 0; i < POINTES; i++) {
    if (a_max < buffer[i])a_max = buffer[i];
    if (a_min > buffer[i])a_min = buffer[i];
    point[i] = ((4096 - buffer[i]) * lcd.max_y()) >> 12;
  }

  int16_t k = 0;
  while (k++ < POINTES - lcd.max_x() - 3) if (point[k + lcd.max_x() / 2] > lcd.max_y() >> 1) break;
  while (k++ < POINTES - lcd.max_x() - 3) if (point[k + lcd.max_x() / 2] < lcd.max_y() >> 1) break;

  //////////////////////////

  // lcd.color(BLUE);
  // for (reg i = 1; i <= lcd.max_x() / Lx; i++)
  //   for (reg j = 1; j <= lcd.max_y() / Lx; j++)
  //     lcd.pixel(i * Lx, j * Lx);

  lcd.color(WHITE);
  lcd.at(5, 5);
  // lcd.printf(" 1 us X 0.1 V  :  Lagrange n = %u  :  Ampl %u mV  \r", Ln, ((a_max - a_min) * 3300) >> 12);
  // lcd.printf("AAAAAAAAAAAA   -   111111111111  000000 \n");

  //////////////////////////

  int16_t last = point2[0];
  int16_t last2 = point[k];
  point2[0] = last2;
  for (reg i = 1; i <= lcd.max_x(); i++) {
    lcd.color(BLACK);
    // lcd.area(i, last, i, point2[i], BLACK);
    lcd.h_line(i, last, point2[i]);
    // lcd.pixel(i, last);
    last = point2[i];

    lcd.color(LIGHT);
    // lcd.area(i, last2, i, point[i + k], LIGHT);
    lcd.h_line(i, last2, point[i + k]);
    // lcd.pixel(i, last2);
    last2 = point[i + k];
    point2[i] = last2;
  }
}

int main(void)
{
  ADC1(ANALOG);

  T32_1_PS;
  T32_1_TOP(320);
  // T32_1_TOP(96);
  T32_1_E;

  lcd.init();
  lcd.font(arial_14);
  // lcd.font(standard_5x8);
  lcd.clear(BLACK);

  dma.setup(buffer, ADC_VALUE, sizeof(buffer));
  dma.read(DMA::TIMER1, DMA::HALF, DMA::HALF, DMA::IMM, DMA::ACK);
  dma.write(DMA::MEM, DMA::WORD, DMA::WORD, DMA::INC);

  L_init(Ln, Lx);

  // uint8_t x = 0;
  while (true)
    // lcd.demo(x++);
    sample();
}
