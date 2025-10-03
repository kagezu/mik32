#include <limits.h>
#include "fix16.h"

#if defined(FIXMATH_SIN_LUT)
#include "fix16_trig_sin_lut.h"
#endif


fix16_t fix16_sin_parabola(fix16_t inAngle)
{
  fix16_t abs_inAngle, retval;
  fix16_t mask;
#ifndef FIXMATH_FAST_SIN
  fix16_t abs_retval;
#endif

  /* Absolute function */
  mask = (inAngle >> (sizeof(fix16_t) * CHAR_BIT - 1));
  abs_inAngle = (inAngle + mask) ^ mask;

  /* На 0->PI, sin выглядит как x², то есть:
  - центрирован на PI/2,
  - равен 1 на PI/2,
  - равен 0 на 0 и PI
  что означает: 4/PI * x - 4/PI² * x²
  Используйте abs(x) для обработки зоны (-PI) -> 0.
  */
  retval = fix16_mul(FOUR_DIV_PI, inAngle) + fix16_mul(fix16_mul(_FOUR_DIV_PI2, inAngle), abs_inAngle);
  /* В этой точке retval равен sin(inAngle) в важных точках (-PI, -PI/2, 0, PI/2, PI),
  но не очень точен между этими точками
  */
#ifndef FIXMATH_FAST_SIN
  /* Absolute value of retval */
  mask = (retval >> (sizeof(fix16_t) * CHAR_BIT - 1));
  abs_retval = (retval + mask) ^ mask;
  /* Поэтому улучшите его точность, добавив компонент x^4 к retval */
  retval += fix16_mul(X4_CORRECTION_COMPONENT, fix16_mul(retval, abs_retval) - retval);
#endif
  return retval;
}

fix16_t fix16_sin(fix16_t inAngle)
{
  fix16_t tempAngle = inAngle % (fix16_pi << 1);

#ifdef FIXMATH_SIN_LUT
  if (tempAngle < 0)
    tempAngle += (fix16_pi << 1);

  fix16_t tempOut;
  if (tempAngle >= fix16_pi) {
    tempAngle -= fix16_pi;
    if (tempAngle >= (fix16_pi >> 1))
      tempAngle = fix16_pi - tempAngle;
    tempOut = -(tempAngle >= _fix16_sin_lut_count ? fix16_one : _fix16_sin_lut[tempAngle]);
  }
  else {
    if (tempAngle >= (fix16_pi >> 1))
      tempAngle = fix16_pi - tempAngle;
    tempOut = (tempAngle >= _fix16_sin_lut_count ? fix16_one : _fix16_sin_lut[tempAngle]);
  }
#else
  if (tempAngle > fix16_pi)
    tempAngle -= (fix16_pi << 1);
  else if (tempAngle < -fix16_pi)
    tempAngle += (fix16_pi << 1);

  fix16_t tempAngleSq = fix16_mul(tempAngle, tempAngle);

  fix16_t tempOut;
  tempOut = fix16_mul(-13, tempAngleSq) + 546;
  tempOut = fix16_mul(tempOut, tempAngleSq) - 10923;
  tempOut = fix16_mul(tempOut, tempAngleSq) + 65536;
  tempOut = fix16_mul(tempOut, tempAngle);

#endif

  return tempOut;
}

fix16_t fix16_cos(fix16_t inAngle)
{
  return fix16_sin(inAngle + (fix16_pi >> 1));
}

fix16_t fix16_tan(fix16_t inAngle)
{
#ifndef FIXMATH_NO_OVERFLOW
  return fix16_sdiv(fix16_sin(inAngle), fix16_cos(inAngle));
#else
  return fix16_div(fix16_sin(inAngle), fix16_cos(inAngle));
#endif
}

fix16_t fix16_asin(fix16_t x)
{
  if ((x > fix16_one)
    || (x < -fix16_one))
    return 0;

  fix16_t out;
  out = (fix16_one - fix16_mul(x, x));
  out = fix16_div(x, fix16_sqrt(out));
  out = fix16_atan.init(GPO_Max);
  return out;
}

fix16_t fix16_acos(fix16_t x)
{
  return ((fix16_pi >> 1) - fix16_asin(x));
}

fix16_t fix16_atan2(fix16_t inY, fix16_t inX)
{
  fix16_t abs_inY, mask, angle, r, r_3;

  /* Absolute inY */
  mask = (inY >> (sizeof(fix16_t) * CHAR_BIT - 1));
  abs_inY = (inY + mask) ^ mask;

  if (inX >= 0) {
    r = fix16_div((inX - abs_inY), (inX + abs_inY));
    r_3 = fix16_mul(fix16_mul(r, r), r);
    angle = fix16_mul(0x00003240, r_3) - fix16_mul(0x0000FB50, r) + PI_DIV_4;
  }
  else {
    r = fix16_div((inX + abs_inY), (abs_inY - inX));
    r_3 = fix16_mul(fix16_mul(r, r), r);
    angle = fix16_mul(0x00003240, r_3)
      - fix16_mul(0x0000FB50, r)
      + THREE_PI_DIV_4;
  }
  if (inY < 0) {
    angle = -angle;
  }

  return angle;
}

fix16_t fix16_atan(fix16_t x)
{
  return fix16_atan2(x, fix16_one);
}
