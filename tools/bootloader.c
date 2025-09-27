#include "mik32_hal_pcc.h"
#include "mik32_hal_spifi_w25.h"

#include "power_manager.h"
#include "uart.h"
#include "pad_config.h"
#include "mik32_memory_map.h"

#include "riscv_csr_encoding.h"
#include "csr.h"

#include "string.h"

// версия начального загрузчика
#define MAJOR_VER   1
#define MINOR_VER   0
#define BUGFIX_VER  0

#define ACK  0x0F     /* Подтверждение */
#define NACK 0xF0     /* Нет подтверждения */
#define MAX_PACKAGE_SIZE 256 /* максимальный размер пакета */
#define TIMEOUT_VALUE 100000 /* Время ожидания загрузчика до прыжка по умолчанию в RAM 1000000 */

/* Виды команд */
typedef enum {
  PACKAGE_SIZE = 0x30,        /* Команда размера пакета */
  SEND_PACKAGE = 0x60,        /* Команда отправить пакет */
  VERSION = 0x90,        /* Команда получения версии начального загрузчика */
} BotloaderComand;

typedef enum {
  SPIFI_ADDRESS = 0x80000000
} AddressMemory;

/* Виды ошибок */
typedef enum {
  ERROR_NONE = 0,
  ERROR_TIMEOUT = 1,  // Время ожидания истекло
  ERROR_CRC,          // Ошибка при получении строки hex от ПК или при записи страницы данных на flash
} Bootloader_error;

typedef struct {
  uint8_t *address; // Адрес для записи присылаемых байт
  uint16_t size_package; // Размер пакета
  uint8_t error;
  uint8_t command; // Текущая принятая загрузчиком команда
} Bootloader_attributes;

Bootloader_attributes hBootloader = { (uint8_t *)SPIFI_ADDRESS, 0, ERROR_NONE, 0 };
uint32_t timeout = 0;
uint32_t validCommandsTimeout = 0;

void go_to_spifi();
void SPIFI_Wait();


/* Инициализация UART */
void Bootloader_UART_Init()
{
  PM->CLK_APB_P_SET = PM_CLOCK_APB_P_UART_0_M; // Включение тактирования UART0

  // Настройка выводов PORT0.5 и PORT0.6 на последовательный интерфейс
  PAD_CONFIG->PORT_0_CFG |= (0b01 << (5 << 1)) | (0b01 << (6 << 1));
  // Включение притяжки к питанию на линии rx
  PAD_CONFIG->PORT_0_PUPD |= (0b01 << (5 << 1));

  /*
   * Настройки USART:
   * Асинхронный режим. Включен RX и TX;
   * Кадр: 8 бит данных, бит четности выключен, 1 стоп бит;
   * Байт LSB - первый бит нулевой.
   */
  UART_0->CONTROL1 = 0;
  UART_0->CONTROL2 = 0;
  UART_0->CONTROL3 = 0;
  UART_0->DIVIDER = 138; /* Baudrate = 230400 */
  // UART_0->DIVIDER = 34; /* Baudrate = 921600 */
  UART_0->FLAGS = 0xFFFFFFFF;
  UART_0->CONTROL1 = UART_CONTROL1_RE_M | UART_CONTROL1_TE_M | UART_CONTROL1_UE_M;

  /* Ожидание флагов готовности RX и TX */
  while (!(UART_0->FLAGS & (UART_FLAGS_REACK_M | UART_FLAGS_TEACK_M)));
}

void Bootloader_UART_Deinit()
{
  UART_0->CONTROL1 = 0;
  UART_0->CONTROL2 = 0;
  UART_0->CONTROL3 = 0;
  UART_0->DIVIDER = 0x0000; // сброс бодрейта
  UART_0->FLAGS = 0xFFFFFFFF; // сброс всех флагов
  UART_0->TXDATA = 0x00;

  // Настройка выводов PORT0.5 и PORT0.6 на порт общего назначения
  PAD_CONFIG->PORT_0_CFG &= ~((0b11 << (5 << 1)) | (0b11 << (6 << 1)));
  // Отключение притяжки на линии rx
  PAD_CONFIG->PORT_0_PUPD &= ~(0b01 << (5 << 1));

  PM->CLK_APB_P_CLEAR = PM_CLOCK_APB_P_UART_0_M; // Выключение тактирования UART0   
}

/* Отправить байт */
void Bootloader_UART_WriteByte(uint16_t Write_Byte)
{
  UART_0->TXDATA = Write_Byte;
  /* Ожидаем успешную передачу */
  while (!(UART_0->FLAGS & UART_FLAGS_TC_M))
    ;
}

/* Ожидание и считывание байта */
uint16_t Bootloader_UART_ReadByte()
{
  timeout = 0;
  while ((!(UART_0->FLAGS & UART_FLAGS_RXNE_M)) && (timeout != TIMEOUT_VALUE)) {
    timeout++;
    validCommandsTimeout++; // Увеличить таймаут валидных команд
  }

  if (timeout == TIMEOUT_VALUE) {
    hBootloader.error = ERROR_TIMEOUT;
  }

  return (uint16_t)UART_0->RXDATA;
}

/* Обработчик ошибок */
void Bootloader_ErrorHandler()
{
  switch (hBootloader.error) {
    case ERROR_TIMEOUT:
      if (UART_0->FLAGS & UART_FLAGS_ORE_M)
        UART_0->FLAGS |= UART_FLAGS_ORE_M;

      go_to_spifi(); // переход в основную программу, если в течение TIMEOUT_VALUE нет принятых данных
      break;
    case ERROR_CRC:
      // отправить nack и перейти в основную программу, если приняли некорректные данные
      Bootloader_UART_WriteByte(NACK);

      if (UART_0->FLAGS & UART_FLAGS_ORE_M)
        UART_0->FLAGS |= UART_FLAGS_ORE_M;
      go_to_spifi();
      break;
  }

  hBootloader.error = ERROR_NONE;
}

SPIFI_HandleTypeDef spifi = { .Instance = SPIFI_CONFIG };

/* Загрузить данные пакета в RAM */
#define SIZE_4K   4096
#define SIZE_32K  32768

// разметка строки в хекс-файле
#define BYTE_COUNT_POS  0   // индекс счетчика байт данных
#define ADDRESS_POS     1   // индекс адреса
#define ADDRESS_QTY     2   // количество байт адреса
#define RECORD_TYPE_POS 3   // индекс типа записи
#define DATA_POS        4   // индекс начала данных в команде

// типы записей в хекс-файле
#define REC_TYPE_DATA           0x00
#define REC_TYPE_EOF            0x01
#define REC_TYPE_EXT_LIN_ADDR   0x04

uint32_t abs_addr = 0; // адрес из хекса
uint32_t rel_addr = 0; // адрес от начала области spifi, по нему определяем, надо ли стирать сектор и какой именно
#define TAIL_SIZE 15             // если попадутся строки хекса, в которых не 16 байт, то мы рискуем записать данные уарта мимо буфера package_data. а если больше 16 байт, то это проблема завтрашнего дня
uint8_t page_data[MAX_PACKAGE_SIZE + TAIL_SIZE]; // сюда собираем распарсенные данные из хекса
uint16_t page_fill_size = 0;  // счетчик, сколько заполнно в page_data. когда page_data заполнена до конца - будем записывать в spifi


#define SPIFI_W25_PROGRAM_BUSY  100000
#define SECTOR_ERASE_32K        0x52

const uint32_t cmd_sector_erase_32k =
SPIFI_DIRECTION_INPUT |
SPIFI_CONFIG_CMD_INTLEN(0) |
SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_FIELDFORM_ALL_SERIAL) |
SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_FRAMEFORM_OPCODE_3ADDR) |
SPIFI_CONFIG_CMD_OPCODE(SECTOR_ERASE_32K);

void mem_write()
{
  // если адрес дошел до начала нового сектора, стираем новый сектор
  rel_addr = (uint32_t)(hBootloader.address) - 0x80000000;
  if ((rel_addr % SIZE_32K) == 0) {
    // HAL_SPIFI_W25_SectorErase4K(&spifi, rel_addr);

    HAL_SPIFI_W25_WriteEnable(&spifi);
    HAL_SPIFI_SendCommand_LL(&spifi, cmd_sector_erase_32k, rel_addr, 0, 0, 0, 0, HAL_SPIFI_TIMEOUT);
    HAL_SPIFI_W25_WaitBusy(&spifi, SPIFI_W25_PROGRAM_BUSY);
  }

  // записываем страницу в 256 байт в spifi
  HAL_SPIFI_W25_PageProgram(&spifi, (uint32_t)hBootloader.address, MAX_PACKAGE_SIZE, page_data);

  // увеличиваем адреса, по которым писать и стирать
  hBootloader.address += MAX_PACKAGE_SIZE;
  // очищаем часть буфера, которая была записана в память и уменьшаем счетчик заполнения буфера на столько, сколько было записано
  if (page_fill_size <= MAX_PACKAGE_SIZE)
    page_fill_size = 0;
  else
    page_fill_size -= MAX_PACKAGE_SIZE;
  memset(&page_data[0], 0xFF, MAX_PACKAGE_SIZE);
  // хвост копируем в начало буфера, чтобы записать его в следующий раз
  memcpy(&page_data[0], &page_data[MAX_PACKAGE_SIZE], TAIL_SIZE);
  // а сам хвост очищаем 
  memset(&page_data[MAX_PACKAGE_SIZE], 0xFF, TAIL_SIZE);
}

void Bootloader_parseHexAndLoadInMemory(uint8_t rx_data[])
{
  // из принятых данных вытаскиваем тип записи
  uint8_t rec_type = rx_data[RECORD_TYPE_POS];
  switch (rec_type) {
    case REC_TYPE_EXT_LIN_ADDR:
      // если так получилось, что нам слали данные, буфер на 256 не заполнился, а тут прилетела команда смены адреса, то пишем сколько есть
      if (page_fill_size != 0)
        mem_write();
      // собираем адрес, с которого начинаем писать из данных команды смены адреса. нам присылают только 2 старших байта адреса
      abs_addr = (rx_data[DATA_POS] << 24) + (rx_data[DATA_POS + 1] << 16);
      hBootloader.address = (uint8_t *)abs_addr;
      break;

    case REC_TYPE_DATA:
      // перекладываем из приемного буфера данные команды в буфер для записи
      memcpy(&page_data[page_fill_size], &rx_data[DATA_POS], rx_data[BYTE_COUNT_POS]);
      // указываем, на сколько заполнился буфер
      page_fill_size += rx_data[BYTE_COUNT_POS];
      // если пора записывать целую страницу - пишем
      if (page_fill_size >= 256)
        mem_write();
      break;

    case REC_TYPE_EOF: // конец прошивки
      Bootloader_UART_WriteByte(ACK);
      // если есть недозаполненная страница, записываем ее как есть
      if (page_fill_size != 0) {
        mem_write();
        // если при записи остатков возникли ошибки, здесь в основную программу не будем переходить
        if (hBootloader.error)
          return;
      }
      // и идем в записанную программу
      go_to_spifi();
      break;
  }
}
uint8_t uart_data[MAX_PACKAGE_SIZE];// = { 0 }; // Массив данных из полученного пакета
uint32_t crc_acc = 0;
uint8_t crc = 0;

void Bootloader_UART_ReadPackage()
{
  crc_acc = 0;
  for (uint32_t counter = 0; counter < hBootloader.size_package; counter++) {
    timeout = 0;
    while ((!(UART_0->FLAGS & UART_FLAGS_RXNE_M)) && (timeout != TIMEOUT_VALUE)) // Ожидание байта пакета
      timeout++;

    if (timeout == TIMEOUT_VALUE) {
      hBootloader.error = ERROR_TIMEOUT;
      break;
    }
    uart_data[counter] = UART_0->RXDATA;

    // контрольная сумма идет последним байтом в пакете. Суммируем всё, кроме нее
    if (counter < (hBootloader.size_package - 1))
      crc_acc += uart_data[counter];
    else {
      // посчитать контрольную сумму и сравнить
      crc = (256 - crc_acc % 256) % 256;
      if (crc != uart_data[counter])
        hBootloader.error = ERROR_CRC;
    }
  }

  if (hBootloader.error == ERROR_NONE)
    Bootloader_parseHexAndLoadInMemory(uart_data);
}

uint8_t eraseChipBufferIndex = 0;  // Индекс для накопления команды erase chip
void Bootloader_Commands()
{
  while (1) {
    hBootloader.command = Bootloader_UART_ReadByte(); // Ожидание и считывание команды

    // если долго не приходили валидные команды, переход в основную программу
    if (validCommandsTimeout >= TIMEOUT_VALUE)
      go_to_spifi();

    if (hBootloader.error)
      Bootloader_ErrorHandler(); // Обработчик ошибок
    else {
      switch (hBootloader.command) {
        case VERSION:
          validCommandsTimeout = 0;           // Сброс таймаута валидных команд
          eraseChipBufferIndex = 0;
          Bootloader_UART_WriteByte(ACK);     // Подтвердить команду
          // отправить все составляющие версии загрузчика
          Bootloader_UART_WriteByte((uint16_t)MAJOR_VER);
          Bootloader_UART_WriteByte((uint16_t)MINOR_VER);
          Bootloader_UART_WriteByte((uint16_t)BUGFIX_VER);
          break;
        case PACKAGE_SIZE:
          validCommandsTimeout = 0;           // Сброс таймаута валидных команд
          eraseChipBufferIndex = 0;
          Bootloader_UART_WriteByte(ACK);     // Подтвердить команду
          hBootloader.size_package = Bootloader_UART_ReadByte() + 1; // Прочитать размер пакета
          Bootloader_UART_WriteByte(ACK);     // Подтвердить
          break;
        case SEND_PACKAGE:
          validCommandsTimeout = 0;           // Сброс таймаута валидных команд
          eraseChipBufferIndex = 0;
          Bootloader_UART_WriteByte(ACK);     // Подтвердить команду
          Bootloader_UART_ReadPackage();      // Получить пакет и скопировать его в RAM
          if (hBootloader.error)
            Bootloader_ErrorHandler();      // Обработчик ошибок
          else
            Bootloader_UART_WriteByte(ACK); // Подтвердить считывание и копирования пакета в RAM
          break;
      }
    }
  }
}

int main()
{  // Батарейный домен
  WU->CLOCKS_BU =
    WU_CLOCKS_BU_LSI32K_EN_M |              // Отключение LSI32К (0 - включение)
    WU_CLOCKS_BU_RTC_CLK_MUX_OSC32K_M;      // Выбрать внешний OSC32К

  // Системный домен
  WU->CLOCKS_SYS =
    WU_CLOCKS_SYS_HSI32M_EN_M |             // Отключить внутренний HSI32M
    WU_CLOCKS_SYS_FORCE_32K_CLK_OSC32K_M;   // Принудительно выбрать OSC32K

  // Выбор источника тактирования системы
  PM->AHB_CLK_MUX = PM_AHB_CLK_MUX_OSC32M_M;// 0 – внешний OSC32M
  PM->DIV_AHB = 0;    // Задает значение делителя шины AHB.
  PM->DIV_APB_M = 0;  // Задает значение делителя шины APB_M.
  PM->DIV_APB_P = 0;  // Задает значение делителя шины APB_P.

  HAL_SPIFI_MspInit(&spifi);

  // Выйти из XIP
  SPIFI_CONFIG->STAT = SPIFI_CONFIG_STAT_RESET_M;

  // Если QPI перейти в нормальный режим
  SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;
  SPIFI_CONFIG->ADDR = 0;
  SPIFI_CONFIG->IDATA = 0x0;
  SPIFI_CONFIG->CMD =
    SPIFI_CONFIG_CMD_DATALEN(1) |
    SPIFI_CONFIG_CMD_INTLEN(1) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_CONFIG_CMD_FIELDFORM_ALL_PARALLEL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_CONFIG_CMD_FRAMEFORM_NOOPCODE_3ADDR) |
    SPIFI_CONFIG_CMD_OPCODE(0xEB);
  SPIFI_CONFIG->DATA8;
  SPIFI_Wait();

  // Если QPI перейти в SPI
  SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;
  SPIFI_CONFIG->CMD =
    SPIFI_CONFIG_CMD_INTLEN(0) |
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_CONFIG_CMD_FIELDFORM_ALL_PARALLEL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_NOADDR) |
    SPIFI_CONFIG_CMD_OPCODE(0xFF);
  SPIFI_Wait();

  Bootloader_UART_Init(); // Инициализация UART. Настройка выводов и тактирования
  Bootloader_Commands(); // Обработка и ожидание команд
}

void SPIFI_Wait() { while (!(SPIFI_CONFIG->STAT & SPIFI_CONFIG_STAT_INTRQ_M)); }

void SPIFI_Init()
{
  // Выйти из XIP
  SPIFI_CONFIG->STAT = SPIFI_CONFIG_STAT_RESET_M;

  // Enable QPI (38h) (передача всего по 4 линиям)
  SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;
  SPIFI_CONFIG->CMD =
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_CONFIG_CMD_FIELDFORM_ALL_SERIAL) |
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_NOADDR) |// только код команды
    SPIFI_CONFIG_CMD_OPCODE(0x38);
  SPIFI_Wait();

  // Настройка Fast Read Quad I/O на работу с передачей исключительно адреса
  SPIFI_CONFIG->STAT |= SPIFI_CONFIG_STAT_INTRQ_M;
  SPIFI_CONFIG->ADDR = 0;
  SPIFI_CONFIG->IDATA = 0x20; // содержимое первого dummy байта команды Fast Read Quad I/O (EBh)
  SPIFI_CONFIG->CMD =
    SPIFI_CONFIG_CMD_DATALEN(1) |           // прочитаем один байт
    SPIFI_CONFIG_CMD_INTLEN(1) |            // один dummy байт 0x0 для режима QPI
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_CONFIG_CMD_FIELDFORM_ALL_PARALLEL) |   // всё по четырём
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_CONFIG_CMD_FRAMEFORM_OPCODE_3ADDR) |    // код команды и три байта адреса
    SPIFI_CONFIG_CMD_OPCODE(0xEB);

  // читаем один байт
  SPIFI_CONFIG->DATA8;
  SPIFI_Wait();
}

void go_to_spifi()
{
  Bootloader_UART_Deinit();
  SPIFI_Init();

  SPIFI_CONFIG->CLIMIT = 0x02000000;  // установить новый CLIMIT 
  SPIFI_CONFIG->CTRL = 0x4030FFFF;
  SPIFI_CONFIG->MCMD =
    SPIFI_CONFIG_CMD_INTLEN(1) |          // один dummy байт 0x00 для режима QPI и только адреса
    SPIFI_CONFIG_CMD_FIELDFORM(SPIFI_CONFIG_CMD_FIELDFORM_ALL_PARALLEL) |     // всё по четырём
    SPIFI_CONFIG_CMD_FRAMEFORM(SPIFI_CONFIG_CMD_FRAMEFORM_NOOPCODE_3ADDR) |   // код команды, три байта адреса
    SPIFI_CONFIG_CMD_OPCODE(0xEB);

  write_csr(mtvec, 0x80000000);                         \
    asm volatile(
      "la ra, 0x80000000\n\t"
      "jalr ra"
      );
}
