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
using namespace Core;

Display *lcd;
void trap_handler()
{
  lcd->printf(P("int ~ "));
}


void EnableInterrupts()
{
  set_csr(mstatus, MSTATUS_MIE);
  set_csr(mie, MIE_MEIE);
}

void DisableInterrupts()
{
  clear_csr(mie, MIE_MEIE);
}


int main(void)
{
  Display a;
  lcd = &a;
  init_clock();
  lcd->init();
  lcd->background(RGB(0, 32, 64));
  lcd->color(RGB(255, 255, 64));
  lcd->clear();
  lcd->font(arial_14);
  // lcd->font(standard_5x8);
  lcd->set_interline(INTERLINE);

  DisableInterrupts();

  //Включаем тактирование необходимых блоков
  PM->CLK_APB_M_SET = PM_CLOCK_APB_M_TIMER32_0_M | PM_CLOCK_APB_M_EPIC_M;

  for (volatile int i = 0; i < 10; i++);

  //Настройка вывода (светодиода)
  PAD_CONFIG->PORT_1_CFG = 1 << (13 << 1); //Установка режима 01 (GPIO) вывода 13
  GPIO_1->DIRECTION_OUT = 1 << 13;
  GPIO_1->OUTPUT = 0; //Установка значения выходного регистра

  //Остановка таймера и сброс настроек до стандартных
  TIMER32_0->ENABLE = 0;
  TIMER32_0->INT_MASK = 0;
  TIMER32_0->INT_CLEAR = 0x3FF;
  TIMER32_0->CONTROL = 0;

  //Настройка максимального значения счетчика
  //и включение прерывания на переполнение
  TIMER32_0->TOP = 32000000u;
  TIMER32_0->INT_MASK = TIMER32_INT_OVERFLOW_M;

  //Сброс настроек контроллера прерываний 
  //и включение линии прерываний таймера
  EPIC->MASK_EDGE_CLEAR = 0xFFFF;
  EPIC->CLEAR = 0xFFFF; //Сброс флагов прерываний
  EPIC->MASK_EDGE_SET = 1 << EPIC_TIMER32_0_INDEX;

  //Разрешение прерываний и запуск таймера
  EnableInterrupts();
  TIMER32_0->ENABLE = TIMER32_ENABLE;

  while (true);
}
