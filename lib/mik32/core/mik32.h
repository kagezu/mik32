#pragma once
#include <inttypes.h> 

// Init

void  init_system();


// ISR

void set_isr(void(*)());
void sei();
void cli();

// Delay

void delay_us(uint32_t us);
void delay_ms(uint32_t ms);


#define ISR_TIMER32_0        0
#define ISR_USART_0          1
#define ISR_USART_1          2
#define ISR_SPI_0            3
#define ISR_SPI_1            4
#define ISR_GPIO             5
#define ISR_I2C_0            6
#define ISR_I2C_1            7
#define ISR_WU               8
#define ISR_TIMER16_0        9   
#define ISR_TIMER16_1       10    
#define ISR_TIMER16_2       11  
#define ISR_TIMER32_1       12
#define ISR_TIMER32_2       13
#define ISR_SPIFI           14
#define ISR_RTC             15
#define ISR_EEPROM          16  
#define ISR_WU_P            17
#define ISR_WU_SPIFI        18  
#define ISR_WU_EEPROM       19
#define ISR_DMA             20
#define ISR_VCC             21
#define ISR_AVCC            22
#define ISR_AVCC            23
#define ISR_VCC             24
#define ISR_VCC             25
#define ISR_VCC             26
#define ISR_BROWN_OUT       27  
#define ISR_T               28    
#define ISR_ADC             29
#define ISR_DAC_0           30
#define ISR_DAC_1           31
