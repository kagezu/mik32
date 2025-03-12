#include "widget.h"

#define GFX_BUFFER_SIZE   100

// static uint16_t _buffer[GFX_BUFFER_SIZE];
// static Buffer<uint16_t> buffer(_buffer, GFX_BUFFER_SIZE);

XBuffer(buffer, uint16_t, GFX_BUFFER_SIZE);

uint16_t Widget::read() { return buffer.read(); }

void Widget::send(uint16_t data)
{
  buffer.write(data);
}

