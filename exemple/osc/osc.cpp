#include "config.h"
#include "adc.h"
#include "timer.h"
#include "dma.h"
#include "lagrange.h"
#include "pin.h"
#include "osc.h"

#define Lp        12  // Узловых точек для интерполяции Лагранжа (чётная)
#define Lh        10  // Шаг интерполяции
#define BORDER_Y  20  // Отступ от верха экрана
#define BORDER_X  1   // Бордюр по краям
#define AXIS_X    10  // Шаг сетки по X
#define AXIS_Y    10  // Шаг сетки по Y
#define AREF_MV   1300  // Опорное напряжение в милливольтах

LCD lcd;
ADC adc;
DMA dma(0, DMA::VERY);

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
int16_t point[POINTES + 1];
int16_t point2[POINTES + 1] = {};

// Коэффициенты Лагранжа
// int16_t li[Lh][Lp];
// int32_t io_bits = 12;
// Lagrange L(li, io_bits);

////////////////////////////////////////////////////

uint8_t mode = Freq;

void encode()
{
  static bool a0, b0;
  bool a = (bool)ENCODER_A(GET);
  bool b = (bool)ENCODER_B(GET);
  if (a != a0) param[mode]->counter += a ^ b ? -1 : 1;
  if (b != b0) param[mode]->counter += a ^ b ? 1 : -1;
  a0 = a;
  b0 = b;
}

////////////////////////////////////////////////////

void info()
{
  lcd.viewport();
  lcd.color(Aqua);
  lcd.printf(P("\f%u us  %u mV  %u Hz  %cC "),
    param[Freq]->get_value(),
    param[VoltageScale]->get_value(),
    (F_CPU >> 2) / (param[Freq]->get_value() * (uint32_t)view.width),
    param[VoltageType]->get_value()
  );
  lcd.printf(P("\f\nВЫБОР:  %s           "), mode_text[mode]);
}

////////////////////////////////////////////////////

void sample(uint32_t tick)
{
  if (tick < 32) tick = 32;
  T32_1_TOP(tick);
  T32_1_C;
  adc.init(2, tick < 96 ? tick - 31 : 63);
  adc.start();
  dma.start();
  while (dma.is_active()) encode();
  // dma.wait();
  adc.stop();
}

////////////////////////////////////////////////////

void draw()
{
  int32_t med = 0;
  int16_t k = 0;

  int16_t min = 1 << 12, max = 0;
  for (reg i = 0; i < END_POINT; i++) {
    reg j = i + (POINTES >> 1);
    if (min > buffer[j]) min = buffer[j];
    if (max < buffer[j]) { max = buffer[j]; k = i; }
  }
  med = (min + max) >> 1;

  int32_t s = AREF_MV * AXIS_Y / param[VoltageScale]->get_value(); // Q32.12

  if (param[VoltageType]->get_value() == 'A')
    med = ((med * s) >> 12) - (view.height >> 1) - param[ZeroLevel]->counter;
  else med = -param[ZeroLevel]->counter;

  for (int16_t i = 0; i <= POINTES; i++)
    point[i] = view.max_y + med - ((buffer[i + k] * s) >> 12);

  //////////////////////////

  lcd.viewport(view);

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
  //////////////////////////

  lcd.color(Blue);
  for (reg i = 1; i <= lcd.max_x() / AXIS_X; i++)
    for (reg j = 1; j <= lcd.max_y() / AXIS_Y; j++)
      lcd.pixel(i * AXIS_X, j * AXIS_Y);

  static uint16_t old;
  lcd.color(Black);
  lcd.w_line(view.min_x, old, view.max_x);

  if (param[VoltageType]->get_value() == 'A')
    old = view.max_y - (view.height >> 1) - param[ZeroLevel]->counter;
  else old = view.max_y + med;

  lcd.color(DarkCyan);
  lcd.w_line(view.min_x, old, view.max_x);
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
  init();

  lcd.init();
  // lcd.font(system_5x7);
  lcd.font(micro_5x6);
  dma.adc(DMA::TIMER1, buffer, sizeof(buffer));

  lcd.color(Blue);
  lcd.rect(view.min_x - 1, view.min_y - 1, view.width + 2, view.height + 2);

  while (true) {
    if (USER_B(GET)) {
      mode++;
      if (mode == CountMode) mode = Freq;
      while (USER_B(GET));
      // info();
    }

    sample(((uint32_t)param[Freq]->get_value() << 5) / AXIS_X);
    draw();
    info();
  }
}
