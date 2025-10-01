#pragma once
#include "mik32.h"
#include "memory/ee-head.h"

u32 ee_write(void *data, u32 w_addr, u32 length);
void ee_erase(u32 w_addr, u32 length);
void ee_write(void *data, EE_Head head);
void ee_erase(EE_Head head);
