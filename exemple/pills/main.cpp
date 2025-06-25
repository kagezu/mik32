#include "config.h"
#include "type/colors.h"
#include "timer.h"
#include "fixmath.h"

#define RADIUS  8
#define TOP     15
#define BOTTOM  18
#define COUNT   8
#define X       0
#define Y       1
#define VX      2
#define VY      3

Fix16 balls[COUNT][4]; // Q 16.16
RGB colors[COUNT];
LCD lcd;
Rect view(1, TOP + 1, lcd.max_x() - 1, lcd.max_y() - BOTTOM - 1);

uint32_t rnd()
{
  static int64_t next = 1;
  next = next * 1103515245 + 12345;
  return next >> 16;
}

int main(void)
{
  T32_0_PS;
  T32_0_E;
  T32_0_C;

  lcd.init();
  lcd.background(Black);
  lcd.color(Blue);
  lcd.clear();
  // lcd.font(sans_24, 0, 0);
  lcd.font(arial_14, 1, 0);
  lcd.rect(view.min_x - 1, view.min_y - 1, view.width + 2, view.height + 2);

  for (int i = 0; i < COUNT; i++) {
    // Задание начальных значений
    balls[i][X] = Fix16((int16_t)(lcd.max_x() >> 1));
    balls[i][Y] = Fix16((int16_t)(lcd.max_y() >> 1));
    balls[i][VX] = (rnd() & 0x1) ? (fix16_t)(-(rnd() & 0x7ffff)) : (fix16_t)(rnd() & 0x7ffff);
    balls[i][VY] = (rnd() & 0x1) ? (fix16_t)(-(rnd() & 0x7ffff)) : (fix16_t)(rnd() & 0x7ffff);
    colors[i] = rnd();
  }

  Fix16 fixV = 0L;
  for (int i = 0; i < COUNT; i++) {
    fixV += fix16_mul(balls[i][VX], balls[i][VX]);
    fixV += fix16_mul(balls[i][VY], balls[i][VY]);
  }

  // Квадрат минимального расстояния между шарами
  Fix16 D2 = (int16_t)(RADIUS * RADIUS * 4);

loop:

  T32_0_C;
  for (int i = 0; i < COUNT; i++) {

    // Псевдонимы
    Fix16 &X0 = balls[i][X];
    Fix16 &Y0 = balls[i][Y];
    Fix16 &VX0 = balls[i][VX];
    Fix16 &VY0 = balls[i][VY];

    // Стираем текущий объект с экрана
    lcd.viewport(&view);
    lcd.color(Black);
    lcd.circle_fill(X0, Y0, RADIUS);

    // Приращение координат
    X0 += VX0;
    Y0 += VY0;

    // Отображаем объект в новом месте
    lcd.color(colors[i]);
    lcd.circle_fill(X0, Y0, RADIUS);

    // Отражение от стенок, дополнено проверяется,
    // что действительно происходит сближение, а не отскок
    if (X0 > (int16_t)(view.max_x - RADIUS) && VX0 > 0L) VX0 = -VX0;
    if (X0 < (int16_t)(view.min_x + RADIUS) && VX0 < 0L) VX0 = -VX0;
    if (Y0 > (int16_t)(view.max_y - RADIUS) && VY0 > 0L) VY0 = -VY0;
    if (Y0 < (int16_t)(view.min_y + RADIUS) && VY0 < 0L) VY0 = -VY0;

    // Соударения между объектами
    for (int j = i + 1; j < COUNT; j++) {

      // Псевдонимы
      Fix16 &X1 = balls[j][X];
      Fix16 &Y1 = balls[j][Y];
      Fix16 &VX1 = balls[j][VX];
      Fix16 &VY1 = balls[j][VY];

      // Достаточно проверить квадрат расстояния
      Fix16 DX = X1 - X0;
      Fix16 DY = Y1 - Y0;
      Fix16 S2 = DX * DX + DY * DY;
      // Слишком далеко: удар не происходит
      if (S2 > D2) continue;

      // Извлекаем корень, только для сблизившихся объектов
      Fix16 S = S2.sqrt();
      // Проекции скоростей на прямую проходящею через центры масс
      Fix16 V0 = (VX0 * DX + VY0 * DY) / S;
      Fix16 V1 = (VX1 * DX + VY1 * DY) / S;
      // Вычисляем скорость сближения
      Fix16 DV = V1 - V0;
      // Проверка на сближение, чтобы объекты не прилипали
      if (DV > 0L) continue;
      // Находим проекции скоростей сближения на оси координат
      Fix16 DVX = DV * DX / S;
      Fix16 DVY = DV * DY / S;

      // Изменяем скорости сближения, на скорости удаления
      VX0 += DVX;
      VY0 += DVY;
      VX1 -= DVX;
      VY1 -= DVY;
    }

  }

  Fix16 sumV = (int16_t)0;
  for (int i = 0; i < COUNT; i++) {
    sumV += fix16_mul(balls[i][VX], balls[i][VX]);
    sumV += fix16_mul(balls[i][VY], balls[i][VY]);
  }

  uint16_t fps = (F_CPU * 10) / T32_0;
  lcd.color(Aquamarine);
  lcd.viewport();
  lcd.at(10, lcd.max_y() - lcd.get_height());
  lcd.printf("FPS: %u.%u ", fps / 10, fps - (fps / 10) * 10);
  lcd.printf("\fError : %ld  ", fixV.value - sumV.value);

  goto loop;
}
