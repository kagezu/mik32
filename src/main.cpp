#include "display.h"
// #include "core.h"
// #include <ST7735/pin.h>

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

// Display lcd;

int main()
{
  InitClock(); // Включение тактирования GPIO

  Display lcd;

  // LED(GPIO);
  // L_SCK(GPIO);
  // L_SDA(GPIO);
  // L_RS(GPIO);
  // L_RST(GPIO);
  // L_CS(GPIO);

  // LED(OUT);
  // L_SCK(OUT);
  // L_SDA(OUT);
  // L_RS(OUT);
  // L_RST(OUT);
  // L_CS(OUT);





  uint8_t x = 0;
  // while (true) lcd.demo(x++);

  while (true) {

    lcd.demo(x++);
  }
}
