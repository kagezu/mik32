#pragma once
#include "mik32.h"

#define EE_WORD     2
#define EE_2K_M     0x7FF
#define EE_ID_M     0x1FF
#define EE_LEN_S    11
#define EE_FIXED_S  22
#define EE_ID_S     23
#define EE_FIXED_M  (1 << EE_FIXED_S)

class EE_Head {
protected:
  u32 data;
public:
  INLINE u32 addr() { return (((data & EE_2K_M) << EE_WORD) + EEPROM_BASE_ADDRESS); }
  INLINE u32 length() { return ((data >> EE_LEN_S) & EE_2K_M) << EE_WORD; }
  INLINE u32 raw_addr() { return data & EE_2K_M; }
  INLINE u32 raw_length() { return (data >> EE_LEN_S) & EE_2K_M; }
  INLINE u32 id() { return data >> EE_ID_S; }
  INLINE bool is_fixed() { return data & EE_FIXED_M; }
  INLINE void fixed() { data |= EE_FIXED_M; }
  INLINE void addr(u32 arg) { data = (data & ~EE_2K_M) | ((arg >> EE_WORD) & EE_2K_M); }
  INLINE void length(u32 arg) { data = (data & ~(EE_2K_M << EE_LEN_S)) | ((((arg + 1) >> EE_WORD) & EE_2K_M) << EE_LEN_S); }
  INLINE void id(u32 arg) { data = (data & ~(EE_ID_M << EE_ID_S)) | (arg << EE_ID_S); }
};
