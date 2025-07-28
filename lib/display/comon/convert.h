#pragma once

#define RGB_TO_24(r, g, b)  ((r) | (g << 8) |(b << 16))
#define RGB_TO_16(r, g, b)  ((((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b&0xf8) >> 3)))
#define RGB_TO_12(r, g, b)  (((r & 0xf0) << 4) | (g & 0xf0) | (b >> 4))

#define RGB_24_TO_24(c)     (c)
#define RGB_24_TO_16(c)     (((c >> 8) & 0xf100) | ((c >> 5) & 0x7e0) | ((c >> 3) & 0x1f))
#define RGB_24_TO_12(c)     (((c >> 12) & 0xf00) | ((c >> 8) & 0xf0) | ((c >> 4) & 0xf))

#define RGB_16_TO_24(c)     (((c & 0xf800) << 8) | ((c & 0x7e0) << 5) | ((c & 0x1f) << 3))
#define RGB_16_TO_16(c)     (c)
#define RGB_16_TO_12(c)     (((c >> 4) & 0xf00) | ((c >> 3) & 0xf0) | ((c >> 1) & 0xf))

#define RGB_12_TO_24(c)     (((c & 0xf00) << 12) | ((c & 0xf0) << 8) | ((c & 0xf) << 4))
#define RGB_12_TO_16(c)     (((c & 0xf00) << 4) | ((c & 0xf0) << 3) | ((c & 0xf) << 1))
#define RGB_12_TO_12(c)     (c)
