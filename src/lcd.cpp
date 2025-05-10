#include "config.h"
#include "font/standard_5x8.h"
#include "font/arial_14.h"
#include "SPI.h"
#include "adc.h"
#include "timer.h"
#include "dma.h"

#define ADC0(f)   f(1,5)
#define ADC1(f)   f(1,7)
#define ADC3(f)   f(0,4)
#define SAMPLES   160
#define BLACK     RGB(32, 32, 32)
#define LIGHT     RGB(64, 255, 64)
#define BLUE      RGB(0, 0, 127)
#define ADC_V     (uint32_t)&ANALOG_REG->ADC_VALUE  // 0x0008504C

Display lcd;
CSPI SPI;
ADC adc;
DMA dma(0, DMA::VERY);

uint16_t buffer[SAMPLES];
// reg buffer[SAMPLES];
reg point[SAMPLES];
volatile uint32_t count;

void GCC_RAM sample()
{
  // adc.init(3, 63);
  adc.init(3, 1);

  // adc.stop();

  // count = 0;
  // T32_1_OVF;

  adc.start();
  while (adc.value() >= 2000);
  while (adc.value() < 2000);
  dma.start();
  dma.wait();
  adc.stop();

  // lcd.clear(BLACK);

  lcd.color(BLUE);
  for (reg i = 1; i < 16; i++) {
    lcd.w_line(0, i * 10, 127);
  }
  for (reg i = 1; i < 13; i++) {
    lcd.h_line(i * 10, 0, 159);
  }

  reg old = point[0];
  reg draw = buffer[0] >> 5;
  point[0] = draw;
  for (reg i = 1; i < SAMPLES; i++) {
    lcd.color(BLACK);
    lcd.w_line(point[i] > old ? old : point[i], i, point[i] > old ? point[i] : old);
    old = point[i];
    // lcd.pixel(point[i], i, BLACK);
    point[i] = buffer[i] >> 5;
    // lcd.pixel(point[i], i, LIGHT);
    lcd.color(LIGHT);
    lcd.w_line(point[i] > draw ? draw : point[i], i, point[i] > draw ? point[i] : draw);
    draw = point[i];
  }

  delay_ms(100);
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

  dma.setup(buffer, ADC_V, SAMPLES << 1);
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
    ANALOG_REG->ADC_SINGLE = 1;               //Одинчное преобразование
    buffer[count++] = ANALOG_REG->ADC_VALUE;  // Сохраняем предыдущий результат
    if (count > SAMPLES) T32_1_IC;            // Отключаем прерывание, если буфер заполнен
    T32_1_FC;                                 // Сбрасываем флаги прерываний
    EPIC_C;
  }
}
