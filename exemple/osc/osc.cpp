#include "config.h"
#include "adc.h"
#include "timer.h"
#include "dma.h"
#include "lagrange.h"
#include "pin.h"
#include "osc.h"

#define Ln        3   // Степень полинома Лагранжа (нечётная)
#define Lx        1   // Интервал между значениями
#define BORDER_Y  20
#define BORDER_X  1

#define POINTES   ((lcd.max_x() + 1)-(BORDER_X << 1))
#define SAMPLES   ((POINTES << 2) + 10)
#define END_POINT SAMPLES - POINTES - 3

LCD lcd;
ADC adc;
DMA dma(0, DMA::VERY);
Rect view = Rect(BORDER_X, BORDER_Y, lcd.max_x() - BORDER_X, lcd.max_y() - 1);

uint16_t buffer[SAMPLES];
int16_t point[POINTES + 1];
int16_t point2[POINTES + 1] = {};

////////////////////////////////////////////////////

uint8_t mode = Freq;
// Discrete param[CountMode];

void encode()
{
  static bool a0, b0;
  bool a = (bool)ENCODER_A(GET);
  bool b = (bool)ENCODER_B(GET);
  if (a != a0) param[mode].counter += a ^ b ? -1 : 1;
  if (b != b0) param[mode].counter += a ^ b ? 1 : -1;
  a0 = a;
  b0 = b;
}

////////////////////////////////////////////////////

void info()
{
  lcd.viewport();
  lcd.color(Aqua);

  lcd.printf(P("\f%uus / %umV / %uHz    "),
    param[Freq].get_value(),
    param[VoltageScale].get_value(),
    (F_CPU >> 2) / (param[Freq].get_value() * (uint32_t)view.width)
  );
  lcd.printf(P("\nMode:  %s           "), mode_text[mode]);
}

////////////////////////////////////////////////////

void sample(uint32_t tick)
{
  T32_1_TOP(tick);
  T32_1_C;
  adc.init(2, tick < 96 ? tick - 31 : 63);
  adc.start();
  dma.start();
  dma.wait();
  adc.stop();
}

////////////////////////////////////////////////////

void draw()
{
  int32_t med = 0;
  uint16_t k = 0;

  for (reg i = 0; i < SAMPLES; i++) med += buffer[i];
  med /= SAMPLES;

  while (k++ < END_POINT) if (buffer[k + (POINTES >> 1)] < med) break;
  while (k++ < END_POINT) if (buffer[k + (POINTES >> 1)] > med) break;

  med = ((med * 1300) >> 9) / param[VoltageScale].get_value() - (view.height >> 1);
  for (uint16_t i = 0; i <= POINTES; i++)
    point[i] = view.max_y + med - (((int32_t)buffer[i + k] * 1300) >> 9) / param[VoltageScale].get_value();

  // for (reg i = 0; i < POINTES / Lx; i++) {
  //   Ly(&buffer[i]);
  //   for (reg j = 0; j < Lx; j++) {
  //     int16_t l = L(j);
  //     point[i * Lx + j] = ((4096 - l) * lcd.max_y()) >> 12;
  //   }
  // }

  //////////////////////////

  lcd.viewport(view);
  lcd.color(DarkCyan);
  lcd.w_line(view.min_x, view.max_y - (view.height >> 1), view.max_x);
  lcd.color(Blue);
  for (reg i = 1; i <= lcd.max_x() >> 3; i++)
    for (reg j = 1; j <= lcd.max_y() >> 3; j++)
      lcd.pixel(i << 3, j << 3);

  //////////////////////////

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
    encode();
  }
}

////////////////////////////////////////////////////

void init()
{
  ENCODER_A(GPIO); ENCODER_A(IN); ENCODER_A(P_VCC);
  ENCODER_B(GPIO); ENCODER_B(IN); ENCODER_B(P_VCC);
  ENCODER_C(GPIO); ENCODER_C(OUT); ENCODER_C(CLR);
  USER_B(GPIO); USER_B(IN); USER_B(P_GND);
  ADC2(ANALOG);

  T32_1_PS;
  T32_1_E;
}

int main(void)
{
  // param[Freq] = { 0, 6 * 4, {1000, 500, 200, 100, 50, 20, 10, 5, 2, 1} };
  // param[VoltageScale] = { 0, 6 * 4, {100, 50, 20, 10, 5, 2, 1} };

  L_init(Ln, Lx);
  init();

  lcd.init();
  lcd.font(system_5x7);
  dma.adc(DMA::TIMER1, buffer, sizeof(buffer));

  lcd.color(Blue);
  lcd.rect(view.min_x - 1, view.min_y - 1, view.width + 2, view.height + 2);

  while (true) {
    if (USER_B(GET)) {
      mode++;
      if (mode == CountMode) mode = Freq;
      while (USER_B(GET));
    }

    sample((uint32_t)param[Freq].get_value() << 2);
    draw();
    info();
  }
}
