#include "core.h"

typedef enum : uint8_t {
  Normal,
  ViewMode,
  Freq,
  VoltagScale,
  VoltagDiv,
  ZeroLevel,
  Threshold,
  CountMode
} OSC_MODE;

const char mode_text[][12] =
{
  "Normal",
  "ViewMode",
  "Freq",
  "VoltagScale",
  "VoltagDiv",
  "ZeroLevel",
  "Threshold",
  "CountMode"
};
