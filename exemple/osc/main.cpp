#include "config.h"
#include "adc.h"
#include "timer.h"
#include "dma.h"
#include "lagrange.h"
#include "osc.h"
#include "encoder.h"
#include "fft.h"

#define Lp            12    // Узловых точек для интерполяции Лагранжа (чётная)
#define Lh            10    // Шаг интерполяции
#define BORDER_Y      20    // Отступ от верха экрана
#define BORDER_X      1     // Бордюр по краям
#define AXIS_X        10    // Шаг сетки по X
#define AXIS_Y        10    // Шаг сетки по Y
#define AREF_MV       1300  // Опорное напряжение в милливольтах
#define INT_FQ        100   // Hz

SPI spi;
LCD lcd;
ADC adc(2);
DMA dma(0, DMA::VERY);
Encoder enc([] (reg d) { menu.next(d); });

#define POINTES   ((lcd.max_x() + 1)-(BORDER_X << 1))
#define SAMPLES   ((POINTES << 2) + Lp)
#define END_POINT SAMPLES - POINTES - 3

Rect view = Rect(
  BORDER_X,
  BORDER_Y,
  lcd.max_x() - BORDER_X,
  lcd.max_y() - 1
);
int16_t buffer[SAMPLES];
int16_t point[POINTES + 2];
int16_t point2[POINTES + 1] = {};

// Коэффициенты Лагранжа
// int16_t li[Lh][Lp];
// int32_t io_bits = 12;
// Lagrange L(li, io_bits);

///////////////////////////////////////////////////////////////////////////////

volatile uint32_t m_sec = 0;

void info()
{
  static uint32_t fps = 0;
  static uint32_t time = 0;
  fps = fps - (fps >> 3) + (INT_FQ / (m_sec - time));
  time = m_sec;

  lcd.viewport();
  lcd.color(Aqua);
  lcd.printf(
    P("\f%uus %umV %cC %S   "),
    Fq.get_item<int>(),
    VScale.get_item<int>(),
    VType.get_item<char>(),
    OType.get_item<char *>()
  );
  lcd.printf(P("\f\n%s              "), menu.get_path());
  lcd.at(lcd.max_x(), 0);
  lcd.printf(P("\b\b\b\b\b\b\vFPS %.1.3q"), fps);
}

///////////////////////////////////////////////////////////////////////////////

void sample(uint32_t tick)
{
  if (tick < 32) tick = 32;
  T32_1_TOP(tick);
  T32_1_C;

  adc.delay(tick < 96 ? tick - 31 : 63);
  adc.start();
  dma.start();
  dma.wait();
  adc.stop();
}

///////////////////////////////////////////////////////////////////////////////

void osc()
{
  int32_t med = 0;
  int16_t k = 0;

  int16_t min = 1 << 12, max = 0;
  for (int i = 0; i < END_POINT; i++) {
    const int j = i + (POINTES >> 1);
    if (min > buffer[j]) min = buffer[j];
    if (max < buffer[j]) { max = buffer[j]; k = i; }
  }
  med = (min + max) >> 1;

  int32_t s = AREF_MV * AXIS_Y / VScale.get_item<int>(); // Q32.12
  if (VType.value == 0) med = ((med * s) >> 12) - (view.height >> 1) - ZLevel.value;
  else med = -ZLevel.value;
  for (int16_t i = 0; i <= POINTES; i++) point[i] = view.max_y + med - ((buffer[i + k] * s) >> 12);

  //////////////////////////

  static uint16_t old;
  lcd.color(Black);
  lcd.w_line(view.min_x, old, view.max_x);
  if (VType.value == 0) old = view.max_y - (view.height >> 1) - ZLevel.value;
  else old = view.max_y + med;
  lcd.color(DarkCyan);
  lcd.w_line(view.min_x, old, view.max_x);
}

///////////////////////////////////////////////////////////////////////////////

void draw()
{
  lcd.viewport(&view);

  uint16_t last = point2[0];
  uint16_t last2 = point[0];
  point2[0] = last2;
  for (uint16_t i = 1; i <= POINTES; i++) {
    lcd.color(Black);
    lcd.h_line(i + view.min_x - 1, last, point2[i]);
    lcd.color(Yellow);
    lcd.h_line(i + view.min_x - 1, last2, point[i]);
    last = point2[i];
    last2 = point2[i] = point[i];
  }

  lcd.color(Blue);
  for (reg i = 1; i <= lcd.max_x() / AXIS_X; i++)
    for (reg j = 1; j <= lcd.max_y() / AXIS_Y; j++)
      lcd.pixel(i * AXIS_X, j * AXIS_Y);
}

///////////////////////////////////////////////////////////////////////////////

void fft()
{
  int32_t s = AREF_MV * AXIS_Y / VScale.get_item<int>(); // Q32.12
  fft(buffer, point, POINTES + 2);
  for (int16_t i = 0; i <= POINTES; i++) point[i] = view.max_y - ((point[i] * s) >> 15);
}

///////////////////////////////////////////////////////////////////////////////

int main(void)
{
  USER_B(IN);
  USER_B(P_GND);
  ADC2(ANALOG);

  T32_1_PS;
  T32_1_E;

  T32_0_PS;
  T32_0_FQ(INT_FQ);
  T32_0_OVF;
  T32_0_IS;
  T32_0_C;
  T32_0_E;

  lcd.init();
  lcd.font(micro_5x6);
  lcd.color(Blue);
  lcd.rect(view.min_x - 1, view.min_y - 1, view.width + 2, view.height + 2);
  dma.adc(DMA::TIMER1, buffer, sizeof(buffer));

  set_csr(mstatus, MSTATUS_MIE);
  sei();

  while (true) {
    if (USER_B(GET)) {
      menu.select();
      while (USER_B(GET));
    }

    sample(((uint32_t)Fq.get_item<int>() << 5) / AXIS_X);
    if (OType.value) fft();
    else osc();
    draw();
    info();
  }
}

///////////////////////////////////////////////////////////////////////////////

extern "C" {
  __attribute__((used, interrupt, section(".trap_text"))) void trap_handler()
  {
    m_sec++;
    enc.scan();
    T32_0_FC;
    EPIC_C;
  }
}
