#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif
  extern void (*__preinit_array_start[]) (void) __attribute__((weak));
  extern void (*__preinit_array_end[]) (void) __attribute__((weak));
  extern void (*__init_array_start[]) (void) __attribute__((weak));
  extern void (*__init_array_end[]) (void) __attribute__((weak));

  void *__dso_handle;

  int __cxa_atexit(void (*fn) (void *), void *arg, void *d)
  {
    return 0;
  }

  // void __attribute__((naked, used, section("SmallSystemInit")))  __libc_init_array(void);
  // void  __libc_init_array(void)
  void SystemInit()
  {
    uint32_t count;
    uint32_t i;

    count = __preinit_array_end - __preinit_array_start;
    for (i = 0; i < count; ++i)
      __preinit_array_start[i]();

    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++)
      __init_array_start[i]();
  }
#ifdef __cplusplus
}
#endif

Display lcd;

int main(void)
{
  /*
  timer0(100);
  T0_CTC;
  T0_COMPA_ON;
  set_isr(TIMER0_COMPA_vect_num, isr);
*/
// __libc_init_array();
  init_system();
  lcd.init();

  sei();

  uint8_t x = 80;
  while (true) {
    lcd.demo(x++);
  }
}
