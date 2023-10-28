#ifndef COLORS_HEADER
#define COLORS_HEADER

#include "display_buffer.h"

static const uint32_t COLORS_ON = 0xFFFFFF;
static const uint32_t COLORS_OFF = 0x000000;

void colorsTransition(uint32_t* from, uint32_t* to, uint8_t percent);
void colorsPattern(uint32_t* pattern);

#endif