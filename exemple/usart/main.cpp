// #include "lcd.h"
// #include "inc/spi.h"
// #include "adc.h"
// #include "pinout.h"
// #include "timer.h"

// LCD lcd;

// int main(void)
// {
//   T32_0_PS;
//   T32_0_EN;
//   T32_0_C;

//   USER_B(GPIO);
//   USER_B(IN);

//   lcd.init();
//   lcd.font(sans_24, 0, 0);

//   int x = 1;

//   while (true) {
//     T32_0_C;
//     if (USER_B(GET)) {
//       lcd.color(color[x & 0x7F]);
//       lcd.background(~color[x++ & 0x7F]);
//       lcd.clear();
//     }
//     else
//       lcd.demo(x++);

//     uint16_t fps = (F_CPU << 4) / T32_0;
//     lcd.at(10, lcd.max_y() - lcd.get_height());
//     lcd.printf(P("FPS: %.2.4q"), fps);
//   }
// }



#include "mik32_hal_usart.h"
/*
 * Данный пример демонстрирует возможности приема и передачи данных
 * по интерфейсу USART.
 *
 * При передаче на устройство (микроконтроллер) строки текста, символы
 * загружаются в буфер, пока не будет обнаружен символ '\n' возврата
 * каретки. Он заменяется на символ '\0' конца строки, а затем содержимое
 * буфера печатается функцией HAL_USART_Print.
 */
 /* Value must be less than 255 */
#define BUFFER_LENGTH   50

USART_HandleTypeDef husart0;

void USART_Init();
int main()
{
  USART_Init();
  HAL_USART_Print(&husart0, "Start\n", USART_TIMEOUT_DEFAULT);
  char buf[BUFFER_LENGTH];
  uint8_t buf_pointer = 0;
  while (1) {
    HAL_USART_Receive(&husart0, buf + buf_pointer, USART_TIMEOUT_DEFAULT);
    if (buf[buf_pointer] == '\n') {
      buf[buf_pointer] = '\0';
      buf_pointer = 0;
      HAL_USART_Print(&husart0, buf, USART_TIMEOUT_DEFAULT);
      HAL_USART_Transmit(&husart0, '\n', USART_TIMEOUT_DEFAULT);
    }
    else {
      if (++buf_pointer >= BUFFER_LENGTH) buf_pointer = 0;
    }
  }
}

void USART_Init()
{
  husart0.Instance = UART_0;
  husart0.transmitting = Enable;
  husart0.receiving = Enable;
  husart0.frame = Frame_8bit;
  husart0.parity_bit = Disable;
  husart0.parity_bit_inversion = Disable;
  husart0.bit_direction = LSB_First;
  husart0.data_inversion = Disable;
  husart0.tx_inversion = Disable;
  husart0.rx_inversion = Disable;
  husart0.swap = Disable;
  husart0.lbm = Disable;
  husart0.stop_bit = StopBit_1;
  husart0.mode = Asynchronous_Mode;
  husart0.xck_mode = XCK_Mode3;
  husart0.last_byte_clock = Disable;
  husart0.overwrite = Disable;
  husart0.rts_mode = AlwaysEnable_mode;
  husart0.dma_tx_request = Disable;
  husart0.dma_rx_request = Disable;
  husart0.channel_mode = Duplex_Mode;
  husart0.tx_break_mode = Disable;
  husart0.Interrupt.ctsie = Disable;
  husart0.Interrupt.eie = Disable;
  husart0.Interrupt.idleie = Disable;
  husart0.Interrupt.lbdie = Disable;
  husart0.Interrupt.peie = Disable;
  husart0.Interrupt.rxneie = Disable;
  husart0.Interrupt.tcie = Disable;
  husart0.Interrupt.txeie = Disable;
  husart0.Modem.rts = Disable; //out
  husart0.Modem.cts = Disable; //in
  husart0.Modem.dtr = Disable; //out
  husart0.Modem.dcd = Disable; //in
  husart0.Modem.dsr = Disable; //in
  husart0.Modem.ri = Disable;  //in
  husart0.Modem.ddis = Disable;//out
  husart0.baudrate = 9600;
  HAL_USART_Init(&husart0);
}
