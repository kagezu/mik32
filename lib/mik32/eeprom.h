#pragma once
#include "mik32.h"

#define EE_PAGE_LENGTH  0x20
#define EE_EEA_PAGE_M   0x1F

class EEPROM {
protected:
  // u32 base_adr;
  // u32 size = EEPROM_SIZE;

public:
  EEPROM() {}

  u32 write(u32 data, u32 address, u32 length)
  {
    u32 *src = (u32 *)data;
    length = length & 3 ? (length >> 2) + 1 : length >> 2;

    while (length && address < EEPROM_EEA_ADDR_M) {
      EEPROM_REGS->EECON = EEPROM_EECON_BWE_M;      // Разрешение записи в буфер
      EEPROM_REGS->EEA = EEPROM_EEA_ADDR(address);  // Адрес страницы

      do {
        EEPROM_REGS->EEDAT = *src++;
        address++;
        length--;
      } while (length && address & EE_EEA_PAGE_M);

      EEPROM_REGS->EECON |=
        EEPROM_EECON_OP(EEPROM_EECON_OP_PR) |
        EEPROM_EECON_EX_M;

      while (EEPROM_REGS->EESTA & EEPROM_EESTA_BSY_M);
    }
    return address;
  }

  void erase(u32 address, u32 length)
  {
    length = length & 3 ? (length >> 2) + 1 : length >> 2;

    while (length && address < EEPROM_EEA_ADDR_M) {
      EEPROM_REGS->EECON = EEPROM_EECON_BWE_M;      // Разрешение записи в буфер
      EEPROM_REGS->EEA = EEPROM_EEA_ADDR(address);  // Адрес страницы

      do {
        EEPROM_REGS->EEDAT = -1;
        address++;
        length--;
      } while (length && address & EE_EEA_PAGE_M);

      EEPROM_REGS->EECON |=
        EEPROM_EECON_OP(EEPROM_EECON_OP_ER) |
        EEPROM_EECON_EX_M;

      while (EEPROM_REGS->EESTA & EEPROM_EESTA_BSY_M);
    }
  }
};
