#include "config.h"
#include "adc.h"
#include "timer.h"
#include "dma.h"
#include "lagrange.h"
#include "osc.h"
#include "encoder.h"
#include "fft.h"

constexpr int Lp = 12;            // Узловых точек для интерполяции Лагранжа (чётная)
constexpr int BORDER_TOP = 20;    // Отступ от верха экрана
constexpr int BORDER_BOTTOM = 1;  // Отступ от низа экрана
constexpr int BORDER_X = 1;       // Бордюр по краям
constexpr int STEP_GREED = 8;     // Шаг сетки
constexpr int BIG_GREED = STEP_GREED * 5;// Шаг крупной сетки
constexpr int AREF_MV = 1300;     // Опорное напряжение в милливольтах
constexpr int INT_FQ = 1000;      // Hz
constexpr int ADC_DEPTH = 12;     // Разрядность АЦП
constexpr int ADC_TIME = 32;      // Время преобразования
constexpr int MAX_TIME = 94;      // Максимальное время цикла

SPI spi;
LCD lcd;
ADC adc(2);
DMA dma(0, DMA::VERY);
Encoder enc;

constexpr int POINTES = ((lcd.max_x() + 2) - (BORDER_X << 1));// Количество точек для отображения
constexpr int SAMPLES = (POINTES << 2) - 2;// Количество измерений для анализа
constexpr int END_POINT = SAMPLES - POINTES - 2;// Последняя точка, с которой может начаться отображение
constexpr int HEIGHT = lcd.max_y() - BORDER_BOTTOM - BORDER_TOP;// Высота области для граф. данных
constexpr int MIDLE_AXIS = (lcd.max_y() - BORDER_BOTTOM - (HEIGHT >> 1));// Расположение оси X

Rect view = Rect(
  BORDER_X,
  BORDER_TOP,
  lcd.max_x() - BORDER_X,
  lcd.max_y() - BORDER_BOTTOM
);
uint16_t buffer[SAMPLES];
int point[POINTES + 1];
int point2[POINTES + 1] = {};

FFT<SAMPLES, ADC_DEPTH> fft;

// Коэффициенты Лагранжа
// int32_t li[STEP_GREED][Lp];
// int32_t io_bits = ADC_DEPTH;
// Lagrange L(li, io_bits);

///////////////////////////////////////////////////////////////////////////////

volatile uint32_t m_sec = 0;

// Вывод на экран текстовой информации
void info()
{
  static int fps = 0;
  static uint32_t time = 0;
  fps = fps - (fps >> 3) + (INT_FQ / (m_sec - time));
  time = m_sec;

  lcd.viewport();
  lcd.color(Aqua);
  lcd.color2(Yellow);
  lcd.printf(
    P("\f\1%u\1us \1%u\1mV %cC %S  "),
    Fq.get_item<int>(),
    VScale.get_item<int>(),
    VType.get_item<char>(),
    OType.get_item<char *>()
  );
  lcd.printf("\f\n");
  menu.print(&lcd);
  lcd.prints("         ");
  lcd.at(lcd.max_x(), 0);
  lcd.printf(P("\b\b\b\b\b\b\bFPS %.1.3q "), fps);
}

///////////////////////////////////////////////////////////////////////////////

enum Trigger :int {
  Maximum,
  Cutoff,
  Front
};

// Поиск временного смещения, по типу триггера
int osc_trigger(int &median, Trigger trig = Front)
{
  int offset = 0;
  int32_t min = 0x7f << 24;
  int32_t max = -min;
  for (int i = 0; i < END_POINT; i++) {
    const int j = i + (POINTES >> 1);
    if (min > buffer[j]) min = buffer[j];
    if (max < buffer[j]) { max = buffer[j]; offset = i; }
  }
  median = (min + max) >> 1;

  if (trig != Maximum) {
    offset = 0;
    while (offset++ < END_POINT) if ((trig == Front) ^ (buffer[offset + (POINTES >> 1)] > median)) break;
    while (offset++ < END_POINT) if ((trig == Front) ^ (buffer[offset + (POINTES >> 1)] < median)) break;
  }

  return offset;
}

// Линейное преобразование массива в координатное пространство дисплея
template<typename I, typename O>
void apply_scale(I in[], O out[], int32_t scale, int32_t offset)
{
  for (int16_t i = 0; i <= POINTES; i++) out[i] = offset - ((in[i] * scale) >> ADC_DEPTH);
}

////////////////////////////////// Axis ///////////////////////////////////////

// Вывод на экран сетки
void greed_draw()
{
  lcd.color(Teal);

  lcd.h_line((lcd.max_x() >> 1), BORDER_TOP, lcd.max_y() - BORDER_BOTTOM);
  for (int i = 1; i <= lcd.max_x() / (BIG_GREED * 2); i++) {
    lcd.h_line((lcd.max_x() >> 1) + i * BIG_GREED, BORDER_TOP, lcd.max_y() - BORDER_BOTTOM);
    lcd.h_line((lcd.max_x() >> 1) - i * BIG_GREED, BORDER_TOP, lcd.max_y() - BORDER_BOTTOM);
  }

  lcd.w_line(BORDER_X, MIDLE_AXIS, lcd.max_x() - BORDER_X);
  for (int i = 0; i <= (HEIGHT / BIG_GREED); i++) {
    lcd.w_line(BORDER_X, MIDLE_AXIS + i * BIG_GREED, lcd.max_x() - BORDER_X);
    lcd.w_line(BORDER_X, MIDLE_AXIS - i * BIG_GREED, lcd.max_x() - BORDER_X);
  }

  for (int i = 1; i <= lcd.max_x() / (STEP_GREED * 2); i++)
    for (int j = 1; j <= HEIGHT / (STEP_GREED * 2); j++) {
      lcd.pixel((lcd.max_x() >> 1) + i * STEP_GREED, MIDLE_AXIS + (j * STEP_GREED));
      lcd.pixel((lcd.max_x() >> 1) + i * STEP_GREED, MIDLE_AXIS - (j * STEP_GREED));
      lcd.pixel((lcd.max_x() >> 1) - i * STEP_GREED, MIDLE_AXIS + (j * STEP_GREED));
      lcd.pixel((lcd.max_x() >> 1) - i * STEP_GREED, MIDLE_AXIS - (j * STEP_GREED));
    }
}

///////////////////////////////////////////////////////////////////////////////

// Обновление графика
void draw()
{
  lcd.viewport(&view);

  int last = point2[0];
  int last2 = point[0];
  point2[0] = last2;
  for (int i = 1; i <= POINTES; i++) {
    lcd.color(Black);
    lcd.h_line(i + view.min_x - 1, last, point2[i]);
    lcd.color(Yellow);
    lcd.h_line(i + view.min_x - 1, last2, point[i]);
    last = point2[i];
    last2 = point2[i] = point[i];
  }
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
  fft.init();

  set_csr(mie, MIE_MEIE);
  sei();

  while (true) {
    if (USER_B(GET)) {
      menu.select();
      while (USER_B(GET));
    }
    int32_t scale = (AREF_MV * STEP_GREED) / VScale.get_item<int>(); // Q32.{ADC_DEPTH}

    // Умножаем микросекунды на 32 MHz, получаем такты.
    // Затем делим на шаг сетки, получаем тактов на пиксель
    int32_t tick = (Fq.get_item<int>() << 5) / STEP_GREED;

    if (tick < ADC_TIME) {
      tick = ADC_TIME;
    }

    T32_1_TOP(tick);// Выставляем количество тактов между семплами
    T32_1_C; // Обнуляем таймер, на случай если он уже выше TOP
    if (tick > MAX_TIME) tick = MAX_TIME;
    // if (tick > MAX_TIME) tick >>= 1;

    // Запрещаем прерывания, поскольку
    // ADC не имеет аппаратной связи с DMA
    // и обращение к памяти сбивает синхронизацию
    cli();
    adc.delay(tick - ADC_TIME + 1);
    adc.start();
    dma.start();
    dma.wait();
    adc.stop();
    sei();

    int median = 0;
    int offset = 0;

    if (OType.value) {
      fft.run(buffer);
      fft.contrast();
      fft.sqrt(buffer);
    }
    else {
      offset = osc_trigger(median);
    }

    // Настраиваем смещение по вертикали
    int shift = VType.value == 0 ? MIDLE_AXIS + ((median * scale) >> ADC_DEPTH) : lcd.max_y() - BORDER_BOTTOM;
    // shift -= ZLevel.value;

    apply_scale(buffer + offset, point, scale, shift);
    greed_draw();
    draw();
    info();
  }
}

///////////////////////////////////////////////////////////////////////////////

ISR
{
  m_sec++;
  menu.next(enc.scan());
  T32_0_FC;
  EPIC_C;
}

