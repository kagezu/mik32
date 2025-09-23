#pragma once

constexpr u32

// CHx_CNTRL

TIM_IDLE = 0x00,    // Вывод не активен
TIM_EN = 0x80,      // Включить вывод

TIM_INV = 0x10,     // Инвертировать захват
TIM_OVR = 0x20,     // Сравнение
TIM_CAP = 0x40,     // Захват
TIM_PWM = 0x60,     // ШИM
TIM_PWM_INV = 0x160,// Инвертировать ШИM

// Direct

TIM_REV = 1,        // Обратный счёт
TIM_BI = 2,         // Двунаправленный


// temp

TIM_ENC1 = 0x00,
TIM_ENC2 = 0x00,
TIM_ENC3 = 0x00,



// DMA/Interrupt Enable Register

TIM_INT = 0x0001,   // CCxIE  Прерывание
TIM_DMA = 0x0100,   // CCxDE  Запрос DMA

TIM_H_CMP = 0x00,   // При сравнении 1
TIM_L_CMP = 0x00,   // При сравнении 0
TIM_H = 0x00,       // Принудительно 1
TIM_L = 0x00,       // Принудительно 0

TIM_IT1 = 0x00,
TIM_IT2 = 0x00,
TIM_IT3 = 0x00,
TIM_IT4 = 0x00,
TIM_TI1 = 0x00,
TIM_TI2FP1 = 0x00,
TIM_TI2FP2 = 0x00,
TIM_ETRF = 0x00,

TIM_RST = 0x00,
TIM_GAT = 0x00,
TIM_TRGI = 0x00,
TIM_TCLK = 0x00,

// Master mode selection Источник тригира

TIM_EGR = 0x00,     // Ресет / бит UG
TIM_CNT_EN = 0x00,  // Постоянно бит CNT_EN
TIM_OVF = 0x00,     // Переполнение (использовать таймер как прдделитель)
TIM_CMP = 0x00,     // Импульс сравнения/захвата
TIM_OC1REF = 0x00,  //
TIM_OC2REF = 0x00,  //
TIM_OC3REF = 0x00,  //
TIM_OC4REF = 0x00;  //
