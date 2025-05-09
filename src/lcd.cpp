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

Display lcd;
CSPI SPI;
ADC adc;
DMA dma(0);

uint16_t buffer[SAMPLES];
// reg buffer[SAMPLES];
reg point[SAMPLES];
volatile uint32_t count;

void GCC_RAM sample()
{
  // adc.init(3, 63);
  adc.init(3, 1);

  adc.start();
  while (adc.value() >= 2000);
  while (adc.value() < 2000);
  // adc.stop();

  // count = 0;
  // T32_1_OVF;

  // dma.source(DMA::MEM, 2, 2, 0);
  // dma.dest(DMA::MEM, 2, 2, 1);
  // dma.start(buffer, (void *)0x0008504C, CYCLES * 4);
  // while (!(DMA_CONFIG->CONFIG_STATUS & DMA_STATUS_READY(0)));
  // dma.wait();

  // PM->CLK_AHB_SET = PM_CLOCK_AHB_DMA_M;   // Включить тактирование модуля
  DMA_CONFIG->CHANNELS[0].DST = (uint32_t)buffer;
  DMA_CONFIG->CHANNELS[0].SRC = 0x0008504C;
  DMA_CONFIG->CHANNELS[0].LEN = SAMPLES * 2 - 1;
  DMA_CONFIG->CHANNELS[0].CFG = 0 |
    // DMA_CH_CFG_READ_MODE_MEMORY_M         // Установить режим памяти
    (8 << DMA_CH_CFG_READ_REQUEST_S)      // Установить периферийную линию
    | (1 << DMA_CH_CFG_READ_SIZE_S)        // Разрядность данных
    | (1 << DMA_CH_CFG_READ_BURST_SIZE_S) // Установить размер пакета
    | (0 << DMA_CH_CFG_READ_INCREMENT_S)    // Установить инкремент
    | (1 << DMA_CH_CFG_READ_ACK_EN_S)      // Установить подтверждение
    | DMA_CH_CFG_WRITE_MODE_MEMORY_M         // Установить режим памяти
    | (2 << DMA_CH_CFG_WRITE_SIZE_S)        // Разрядность данных
    | (2 << DMA_CH_CFG_WRITE_BURST_SIZE_S) // Установить размер пакета
    | (1 << DMA_CH_CFG_WRITE_INCREMENT_S)    // Установить инкремент
    | (0 << DMA_CH_CFG_WRITE_ACK_EN_S)      // Установить подтверждение
    | (3 << DMA_CH_CFG_PRIOR_S)           // Установка приоритета
    | DMA_CH_CFG_ENABLE_M;                  // Включить

  // while (!(DMA_CONFIG->CONFIG_STATUS & DMA_STATUS_READY(0)));

  delay_us(160);

  adc.stop();

  // lcd.clear(BLACK);

  lcd.color(BLUE);
  for (reg i = 1; i < 16; i++) {
    lcd.w_line(0, i * 10, 127);
  }
  for (reg i = 1; i < 13; i++) {
    lcd.h_line(i * 10, 0, 159);
  }

  // delay_ms(10);

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

  // delay_ms(100);

}

int main(void)
{
  T32_1_POWER_ON;
  // T32_1_TOP(94);
  T32_1_TOP(32);
  // T32_1_TOP(120);
  T32_1_ON;

  ADC0(ANALOG);
  ADC1(ANALOG);
  ADC3(ANALOG);

  SPI.init();
  lcd.init();
  lcd.clear(BLACK);

  // sei();
  // EPIC->MASK_LEVEL_SET = EPIC_LINE_M(EPIC_LINE_TIMER32_1_S);
  while (true) sample();
}

extern "C" {
  __attribute__((used, interrupt))
    void isr_handler(void)
  {
    //Одинчное преобразование
    ANALOG_REG->ADC_SINGLE = 1;
    // Сохраняем предыдущий результат
    buffer[count++] = ANALOG_REG->ADC_VALUE;
    // Отключаем прерывание, если буфер заполнен
    if (count > SAMPLES) TIMER32_1->INT_MASK = 0;
    // Сбрасываем флаги прерываний
    TIMER32_1->INT_CLEAR = -1;
    EPIC->CLEAR = -1;
  }
}
