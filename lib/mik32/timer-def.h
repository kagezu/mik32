#pragma once

#define TIM_MODE_DIR  0b00
#define TIM_MODE_REV  0b01
#define TIM_MODE_BI   0b10

enum class TIM_MODE {
  CMP,
  PWM,
  CAP,
  DIR,
  REV,
  BI,
  ENC
};
