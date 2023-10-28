#include "colors.h"

uint32_t asColor(uint32_t masked, uint32_t color) {
  if(masked > 0) {
    return color;
  }

  return COLORS_OFF;
}

uint8_t scaledComp(uint8_t from, uint8_t to, uint8_t scale) {
  long scaleBuffered = scale;
  long fromBuffered = from;
  long delta = ((long)to) - fromBuffered;
  return fromBuffered + (delta * scaleBuffered / 0xFF);
}

uint32_t scaled(uint32_t from, uint32_t to, uint8_t scale) {
  uint8_t rComp = scaledComp(from >> 8, to >> 8, scale);
  uint8_t gComp = scaledComp(from >> 16, to >> 16, scale);
  uint8_t bComp = scaledComp(from, to, scale);

  return bComp | rComp << 8 | gComp << 16;
}

uint32_t test(uint32_t from, uint16_t scale) {
  uint32_t rComp = (from >> 8) & 0xFF;
  uint32_t gComp = (from >> 16) & 0xFF;
  uint32_t bComp = from & 0xFF;

  int32_t rVal = (rComp * scale / 0xFF) & 0xFF;
  int32_t gVal=  (gComp * scale / 0xFF) & 0xFF;
  int32_t bVal = (bComp * scale / 0xFF) & 0xFF;

  return (rVal << 8) | (gVal << 16) | (bVal);
}
 
void colorsTransition(uint32_t* fromPattern, uint32_t* toPattern, uint8_t percent, uint32_t colorFrom, uint32_t colorTo) {
  for (int row = 0; row < PATTERN_SIZE; row++) {
    for (int col = 0; col < PATTERN_SIZE; col++) {
      uint32_t mask = 1 << (PATTERN_SIZE - 1 - col);
      uint32_t overlayFrom = asColor(mask & fromPattern[PATTERN_SIZE - 1 - row], colorFrom);
      uint32_t overlayTo = asColor(mask & toPattern[PATTERN_SIZE - 1 - row], colorTo);  
      displayBufferSet(row, col, scaled(overlayFrom, overlayTo, percent));
    }
  }
  displayBufferShow();
}

void colorsPattern(uint32_t* pattern, uint32_t color) {
  displayBufferReset(COLORS_OFF);
  for (int row = 0; row < PATTERN_SIZE; row++) {
    for (int col = 0; col < PATTERN_SIZE; col++) {
      uint32_t mask = 1 << (PATTERN_SIZE - 1 - col);
      uint32_t overlay = mask & pattern[PATTERN_SIZE - 1 - row];
      if (overlay > 0) {
        displayBufferSet(row, col, color);
      }
    }
  }
  displayBufferShow();
}