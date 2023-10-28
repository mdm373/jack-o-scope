#ifndef COLORS_HEADER
#define COLORS_HEADER

#include "display_buffer.h"

static const uint32_t COLORS_ON = 0xFFFFFF;
static const uint32_t COLORS_OFF = 0x000000;

static const uint32_t COLORS_GREEN = 0xFF0000;
static const uint32_t COLORS_RED = 0x00FF00;
static const uint32_t COLORS_BLUE = 0x0000FF;

void colorsTransition(uint32_t* from, uint32_t* to, uint8_t percent, uint32_t colorFrom, uint32_t colorTo);
void colorsPattern(uint32_t* pattern, uint32_t color);

#endif