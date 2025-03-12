// #include "eeprom/eeprom.h"
// Перезагрузка функций с поддержкой буферизации записи

#pragma once
#include <avr.h>

#define EEP_BUFFER_SIZE 32

uint8_t eeprom_read_byte(const uint8_t *__p);
void eeprom_write_byte(uint8_t *__p, uint8_t __value);
