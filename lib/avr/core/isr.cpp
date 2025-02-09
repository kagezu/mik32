#include "avr.h"

typedef void (*func_ptr) ();

static func_ptr isr_vector[32];

void set_isr(uint8_t isr, void handler())
{
  isr_vector[isr] = handler;
}


ISR(INT0_vect) { isr_vector[INT0_vect_num](); }
ISR(INT1_vect) { isr_vector[INT1_vect_num](); }
ISR(PCINT0_vect) { isr_vector[PCINT0_vect_num](); }
ISR(PCINT1_vect) { isr_vector[PCINT1_vect_num](); }
ISR(PCINT2_vect) { isr_vector[PCINT2_vect_num](); }
ISR(WDT_vect) { isr_vector[WDT_vect_num](); }
ISR(TIMER2_COMPA_vect) { isr_vector[TIMER2_COMPA_vect_num](); }
ISR(TIMER2_COMPB_vect) { isr_vector[TIMER2_COMPB_vect_num](); }
ISR(TIMER2_OVF_vect) { isr_vector[TIMER2_OVF_vect_num](); }
ISR(TIMER1_CAPT_vect) { isr_vector[TIMER1_CAPT_vect_num](); }
ISR(TIMER1_COMPA_vect) { isr_vector[TIMER1_COMPA_vect_num](); }
ISR(TIMER1_COMPB_vect) { isr_vector[TIMER1_COMPB_vect_num](); }
ISR(TIMER1_OVF_vect) { isr_vector[TIMER1_OVF_vect_num](); }
ISR(TIMER0_COMPA_vect) { isr_vector[TIMER0_COMPA_vect_num](); }
ISR(TIMER0_COMPB_vect) { isr_vector[TIMER0_COMPB_vect_num](); }
ISR(TIMER0_OVF_vect) { isr_vector[TIMER0_OVF_vect_num](); }
ISR(SPI_STC_vect) { isr_vector[SPI_STC_vect_num](); }
ISR(USART_RX_vect) { isr_vector[USART_RX_vect_num](); }
ISR(USART_UDRE_vect) { isr_vector[USART_UDRE_vect_num](); }
ISR(USART_TX_vect) { isr_vector[USART_TX_vect_num](); }
ISR(ADC_vect) { isr_vector[ADC_vect_num](); }
ISR(EE_READY_vect) { isr_vector[EE_READY_vect_num](); }
ISR(ANALOG_COMP_vect) { isr_vector[ANALOG_COMP_vect_num](); }
ISR(TWI_vect) { isr_vector[TWI_vect_num](); }
ISR(SPM_READY_vect) { isr_vector[SPM_READY_vect_num](); }

