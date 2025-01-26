#include "display.h"
// #include "core.h"

#define PIN_LED 7	 // Светодиод управляется выводом PORT_2_7
#define PIN_BUTTON 6 // Кнопка управляет сигналом на выводе

void InitClock()
{
  PM->CLK_APB_P_SET |= PM_CLOCK_APB_P_UART_0_M | PM_CLOCK_APB_P_GPIO_0_M | PM_CLOCK_APB_P_GPIO_1_M | PM_CLOCK_APB_P_GPIO_2_M; // включение тактирования GPIO
  PM->CLK_APB_M_SET |= PM_CLOCK_APB_M_PAD_CONFIG_M | PM_CLOCK_APB_M_WU_M | PM_CLOCK_APB_M_PM_M;								// включение тактирования блока для смены режима выводов
}

#define PIN_LED 7	 // Светодиод управляется выводом PORT_2_7
#define PIN_BUTTON 6 // Кнопка управляет сигналом на выводе PORT_2_6

#define LED(x)      x(2, 7)
#define BUTTON(x)   x(2, 6)

Display lcd;

int main()
{
  InitClock(); // Включение тактирования GPIO

  // LED(GPIO);
  // BUTTON(GPIO);
  // LED(OUT);
  // BUTTON(IN);

  // while (true) {
  //   if (BUTTON(GET)) {
  //     LED(SET);
  //   }
  //   else {
  //     LED(RES);
  //   }
  // }

  int x = 0;

  // while (true)
  //   lcd.demo(x++);

  L_CS(SET);

  SELA45(GPIO);
  SELA45(OUT);
  SELA45(RES);

  while (true);
}
