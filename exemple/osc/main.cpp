#include "config.h"
#include "adc.h"
#include "timer.h"
#include "dma.h"
#include "lagrange.h"
#include "osc.h"
#include "encoder.h"
#include "fft.h"

constexpr int Lp = 10;            // Узловых точек для интерполяции Лагранжа (чётная)
constexpr int INT_FQ = 1000;      // Hz
constexpr int ADC_CH = 2;         // Номер канала ADC
Encoder enc;
DMA dma(0, DMA::VERY);

// SPI_1
SPI spi;

// Дисплей
constexpr int BORDER_TOP = 20;    // Отступ от верха экрана
constexpr int BORDER_BOTTOM = 1;  // Отступ от низа экрана
constexpr int BORDER_X = 1;       // Бордюр по краям
constexpr int P_SEG = 8;          // Шаг сетки, точек на сегмент
constexpr int SEG = P_SEG * 5;    // Шаг крупной сетки

LCD lcd;
Rect view = Rect(BORDER_X, BORDER_TOP, lcd.max_x() - BORDER_X, lcd.max_y() - BORDER_BOTTOM);
constexpr int POINTES = ((lcd.max_x() + 2) - (BORDER_X << 1));  // Количество точек для отображения
constexpr int SAMPLES = (POINTES << 2) - 2;                     // Количество измерений для анализа
constexpr int END_POINT = SAMPLES - POINTES - P_SEG - 2;        // Последняя точка, с которой может начаться отображение
constexpr int HEIGHT = lcd.max_y() - BORDER_BOTTOM - BORDER_TOP;// Высота области для граф. данных
constexpr int MIDLE_AXIS = (lcd.max_y() - BORDER_BOTTOM - (HEIGHT >> 1));// Расположение оси X

// uint16_t buffer[SAMPLES];
short buffer[SAMPLES];
short points[POINTES + 1];
short points2[POINTES + 1] = {};

FFT<SAMPLES, ADC::DEPTH> fft;
Lagrange<Lp, P_SEG, ADC::DEPTH> L;

///////////////////////////////////////////////////////////////////////////////

volatile uint32_t m_sec = 0;

// Вывод на экран текстовой информации
void print_info()
{
  static int fps = 0;
  static uint32_t time = 0;
  fps = fps - (fps >> 3) + (INT_FQ / (m_sec - time));
  time = m_sec;

  lcd.viewport();
  lcd.color(Aqua);
  lcd.color2(Yellow);
  lcd.printf(
    P("\f\1%u\1us \1%u\1mV %S %S  "),
    FqScale.get_item<int>(),
    VScale.get_item<int>(),
    VType.get_item<char *>(),
    AppMode.get_item<char *>()
  );
  lcd.printf("\f\n");
  menu.print(&lcd);
  lcd.prints("             ");
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
void osc_trigger(int &offset, int &median, Trigger trig = Front)
{
  int32_t min = 0x7f << 24;
  int32_t max = -min;
  offset = 0;
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
}

// Линейное преобразование массива в координатное пространство дисплея
template<typename I, typename O>
void transform_to_display(I in[], O out[], int32_t scale, int32_t offset)
{
  for (int16_t i = 0; i <= POINTES; i++) out[i] = offset - (((in[i]) * scale) >> ADC::DEPTH);
}

////////////////////////////////// Axis ///////////////////////////////////////

// Вывод на экран сетки
void greed_draw()
{
  lcd.viewport(&view);
  lcd.color(Teal);

  lcd.h_line((lcd.max_x() >> 1), BORDER_TOP, lcd.max_y() - BORDER_BOTTOM);
  for (int i = 1; i <= lcd.max_x() / (SEG * 2); i++) {
    lcd.h_line((lcd.max_x() >> 1) + i * SEG, BORDER_TOP, lcd.max_y() - BORDER_BOTTOM);
    lcd.h_line((lcd.max_x() >> 1) - i * SEG, BORDER_TOP, lcd.max_y() - BORDER_BOTTOM);
  }

  lcd.w_line(BORDER_X, MIDLE_AXIS, lcd.max_x() - BORDER_X);
  for (int i = 0; i <= (HEIGHT / SEG); i++) {
    lcd.w_line(BORDER_X, MIDLE_AXIS + i * SEG, lcd.max_x() - BORDER_X);
    lcd.w_line(BORDER_X, MIDLE_AXIS - i * SEG, lcd.max_x() - BORDER_X);
  }

  for (int i = 1; i <= lcd.max_x() / (P_SEG * 2); i++)
    for (int j = 1; j <= HEIGHT / (P_SEG * 2); j++) {
      lcd.pixel((lcd.max_x() >> 1) + i * P_SEG, MIDLE_AXIS + (j * P_SEG));
      lcd.pixel((lcd.max_x() >> 1) + i * P_SEG, MIDLE_AXIS - (j * P_SEG));
      lcd.pixel((lcd.max_x() >> 1) - i * P_SEG, MIDLE_AXIS + (j * P_SEG));
      lcd.pixel((lcd.max_x() >> 1) - i * P_SEG, MIDLE_AXIS - (j * P_SEG));
    }
}

///////////////////////////////////////////////////////////////////////////////

// Обновление графика
void data_draw()
{
  lcd.viewport(&view);

  int last = points2[0];
  int last2 = points[0];
  points2[0] = last2;
  for (int i = 1; i <= POINTES; i++) {
    lcd.color(Black);
    lcd.h_line(i + view.min_x - 1, last, points2[i]);
    lcd.color(Yellow);
    lcd.h_line(i + view.min_x - 1, last2, points[i]);
    last = points2[i];
    last2 = points2[i] = points[i];
  }
}

///////////////////////////////////////////////////////////////////////////////

void test_button()
{
  // if (USER_B(GET)) {
  //   menu.select();
  //   while (USER_B(GET));
  // }
  if (!ENCODER_C(GET)) {
    menu.select();
    while (!ENCODER_C(GET));
  }
}

///////////////////////////////////////////////////////////////////////////////

int main(void)
{
  ENCODER_C(IN);
  ENCODER_C(P_VCC);

  USER_B(IN);
  USER_B(P_GND);

  T32_1_PS;
  T32_1_E;

  T32_0_PS;
  T32_0_FQ(INT_FQ);
  T32_0_OVF;
  T32_0_IS;
  T32_0_C;
  T32_0_E;

  lcd.init();
  lcd.font(micro_5x6, 1, 4);
  lcd.color(Blue);
  lcd.rect(view.min_x - 1, view.min_y - 1, view.width + 2, view.height + 2);
  dma.adc(DMA::TIMER1, buffer, sizeof(buffer));
  fft.init();
  ADC::init(ADC_CH);

  set_csr(mie, MIE_MEIE);
  sei();

  while (true) {
    const int32_t scale = (ADC::AREF * P_SEG) / VScale.get_item<int>();// ADC => Display
    const int32_t t_seg = (int32_t)FqScale.get_item<int>() << 5; // Умножаем микросекунды на 32 MHz. [такты на сегмент]
    int t_samp = 0, t_adc = 0, p_samp = 1;

    switch (AppMode.value) {
      case MODE_OSC:
        t_adc = ADC::cycle(t_seg / P_SEG);        // Готовим допустимое значение для АЦП. [тактов на выборку]
        p_samp = (t_adc * P_SEG - 1) / t_seg + 1; // Если больше 1, необходима интерполяция. [точек на выборку]
        t_samp = t_seg * p_samp / P_SEG;          // Уточняем время выборки. [тактов на выборку]
        t_adc = ADC::cycle(t_samp);
        break;

      case MODE_FFT:
      case MODE_SPEC:
        t_adc = ADC::cycle(t_seg);
        t_samp = t_seg;
        break;
    }

    T32_1_TOP(t_samp);  // Выставляем количество тактов между семплами
    T32_1_C;            // Обнуляем таймер, на случай если он уже выше TOP

    cli(); // Запрещаем прерывания
    ADC::delay(t_adc);
    ADC::start();
    dma.start();
    dma.wait();
    ADC::stop();
    sei();

    switch (AppMode.value) {
      case MODE_OSC: {
          if (p_samp > 1) {
            L.init(p_samp);
            // Чтоб немного сэкономить память, пишем в тот же буфер
            L.interpolate(buffer + SAMPLES - SAMPLES / p_samp, buffer, SAMPLES - P_SEG);
          }
          int median, offset;
          osc_trigger(offset, median);

          // Настраиваем смещение по вертикали
          int shift = VType.value == 0 ? MIDLE_AXIS + ((median * scale) >> ADC::DEPTH) : lcd.max_y() - BORDER_BOTTOM;
          shift -= ZeroLevel.value;

          transform_to_display(buffer + offset, points, scale, shift);
          break;
        }

      case MODE_FFT:
      case MODE_SPEC:
        fft.run(buffer);
        if (FType.value == 1) fft.contrast();
        if (FType.value == 2) fft.log(buffer);
        else fft.sqrt(buffer);
        transform_to_display(buffer, points, scale, lcd.max_y() - BORDER_BOTTOM);
        break;
    }

    greed_draw();
    data_draw();
    print_info();
    test_button();
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

