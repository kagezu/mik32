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

LCD lcd;
ADC adc;
DMA dma(0, DMA::VERY);
Rect view = Rect(BORDER_X, BORDER_Y, lcd.max_x() - BORDER_X, lcd.max_y() - 1);

uint16_t buffer[SAMPLES];
int16_t point[POINTES];
int16_t point2[POINTES] = {};

////////////////////////////////////////////////////

uint8_t mode = Normal;
int16_t count[CountMode] = {
  0,    // Normal
  0,    // ViewMode
  320,  // Freq
  0,    // VoltagScale
  0,    // VoltagDiv
  0,    // ZeroLevel
  0     // Threshold
};

void encode()
{
  static bool a0, b0;
  bool a = (bool)ENCODER_A(GET);
  bool b = (bool)ENCODER_B(GET);
  if (a != a0) count[mode] += a ^ b ? -1 : 1;
  if (b != b0) count[mode] += a ^ b ? 1 : -1;
  a0 = a;
  b0 = b;
}

////////////////////////////////////////////////////

void sample(uint16_t tick)
{
  T32_1_TOP(tick);
  T32_1_C;
  adc.init(2, tick < 96 ? tick - 31 : 63);
  adc.start();
  dma.start();
  dma.wait();
  adc.stop();
}

//////////////////////////

void draw()
{
  lcd.viewport(view);
  // for (reg i = 0; i < POINTES / Lx; i++) {
  //   Ly(&buffer[i]);
  //   for (reg j = 0; j < Lx; j++) {
  //     int16_t l = L(j);
  //     point[i * Lx + j] = ((4096 - l) * lcd.max_y()) >> 12;
  //   }
  // }

  uint32_t sum = 0;
  for (reg i = 0; i < POINTES; i++) {
    sum += buffer[i];
    point[i] = lcd.max_y() - ((buffer[i] * (int32_t)view.height) >> 12);
    encode();
  }

  int16_t k = 0;
  while (k++ < POINTES - view.width - 3) if (point[k + view.width / 3] > (view.height >> 1)) break;
  while (k++ < POINTES - view.width - 3) if (point[k + view.width / 3] < (view.height >> 1)) break;

  //////////////////////////

  lcd.color(Blue);
  for (reg i = 1; i <= lcd.max_x() >> 3; i++)
    for (reg j = 1; j <= lcd.max_y() >> 3; j++)
      lcd.pixel(i << 3, j << 3);

  //////////////////////////

  int16_t last = point2[0];
  int16_t last2 = point[k];
  point2[0] = last2;
  for (int16_t i = 1; i < view.width; i++) {
    lcd.color(Black);
    lcd.h_line(i + view.min_x, last, point2[i]);
    lcd.color(Yellow);
    lcd.h_line(i + view.min_x, last2, point[i + k]);
    last = point2[i];
    last2 = point2[i] = point[i + k];
    encode();
  }
}

void info()
{
  lcd.viewport();
  lcd.color(Aqua);

  lcd.printf(P("\f%uus %umV D:%u lvl:%u Thr:%u  "),
    count[Freq],
    count[VoltagScale],
    count[VoltagDiv],
    count[ZeroLevel],
    count[Threshold]);
  lcd.printf(P("Mode: %s     "), mode_text[mode]);
}

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
      if (mode == CountMode) mode = Normal;
      while (USER_B(GET));
    }

    sample(3200);
    draw();
    info();
  }
}
