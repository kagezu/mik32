#include "config.h"
// #include "VS1053/VS1053.h"
#include "font/arial_14.h"
#include "font/standard_5x8.h"
#include "timer/timer.h"

#include <mik32_memory_map.h>
#include <power_manager.h>
#include <pad_config.h>
#include <timer32.h>
#include <epic.h>
// #include <riscv_csr_encoding.h>
#include <scr1_csr_encoding.h>
#include <csr.h>

#define INTERLINE 3
// using namespace Core;


void EnableInterrupts()
{
  set_csr(mstatus, MSTATUS_MIE);
  set_csr(mie, MIE_MEIE);
}

void DisableInterrupts()
{
  clear_csr(mie, MIE_MEIE);
}


#define ISR(func) static int _isr = 0; if (_isr) { func(); return 0; } _isr = 1;

void isr()
{
  GPIO_2->OUTPUT ^= 1 << 7; //Установка значения выходного регистра
  TIMER32_0->INT_CLEAR = TIMER32_INT_OVERFLOW_M; //Сброс флага прерывания таймера
  EPIC->CLEAR = 1 << EPIC_TIMER32_0_INDEX; //Сброс флага прерывания линии таймера
}

extern uint32_t isr_vector;

int main(void)
{
  isr_vector = (uint32_t)(isr);

  Display lcd;
  Core::init_clock();
  lcd.init();
  lcd.background(RGB(0, 32, 64));
  lcd.color(RGB(255, 255, 64));
  lcd.clear();
  // lcd.font(arial_14);
  lcd.font(standard_5x8);
  lcd.set_interline(INTERLINE);

  DisableInterrupts();

  //Включаем тактирование необходимых блоков
  PM->CLK_APB_M_SET |= PM_CLOCK_APB_M_TIMER32_0_M | PM_CLOCK_APB_M_EPIC_M;

  for (volatile int i = 0; i < 10; i++);

  //Настройка вывода (светодиода)
  PAD_CONFIG->PORT_2_CFG &= ~(3 << (7 << 1)); //Установка режима 01 (GPIO) вывода 13
  GPIO_2->DIRECTION_OUT = 1 << 7;
  GPIO_2->OUTPUT = 0; //Установка значения выходного регистра

  //Остановка таймера и сброс настроек до стандартных
  TIMER32_0->ENABLE = TIMER32_ENABLE_TIM_CLR_M;
  TIMER32_0->INT_MASK = 0;
  TIMER32_0->INT_CLEAR = 0x3FF;
  TIMER32_0->CONTROL = 0;

  //Настройка максимального значения счетчика
  //и включение прерывания на переполнение
  TIMER32_0->TOP = 0xffffffff;
  TIMER32_0->INT_MASK = TIMER32_INT_OVERFLOW_M;

  //Сброс настроек контроллера прерываний 
  //и включение линии прерываний таймера
  EPIC->MASK_EDGE_CLEAR = 0xFFFF;
  EPIC->CLEAR = 0xFFFF; //Сброс флагов прерываний
  EPIC->MASK_EDGE_SET = 1 << EPIC_TIMER32_0_INDEX;

  //Разрешение прерываний и запуск таймера
  // EnableInterrupts();
  TIMER32_0->ENABLE = TIMER32_ENABLE_TIM_EN_M;

  while (true) {
    delay_ms(100);
    DisableInterrupts();
    lcd.printf(P("\f\n\n%lx   \n"), TIMER32_0->VALUE);
    EnableInterrupts();
  }
}
