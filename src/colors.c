#include "colors.h"

uint32_t asColor(uint32_t masked) {
  if(masked > 0) {
    return COLORS_ON;
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
 
void colorsTransition(uint32_t* fromPattern, uint32_t* toPattern, uint8_t percent) {
  for (int row = 0; row < PATTERN_SIZE; row++) {
    for (int col = 0; col < PATTERN_SIZE; col++) {
      uint32_t mask = 1 << (PATTERN_SIZE - 1 - col);
      uint32_t overlayFrom = asColor(mask & fromPattern[PATTERN_SIZE - 1 - row]);
      uint32_t overlayTo = asColor(mask & toPattern[PATTERN_SIZE - 1 - row]);  
      displayBufferSet(row, col, scaled(overlayFrom, overlayTo, percent));
    }
  }
  
 /*
  uint32_t start = COLORS_OFF;
  uint32_t end = COLORS_ON;
  displayBufferSet(0, 0, scaled(start, end, 16));
  displayBufferSet(0, 1, scaled(start, end, 32));
  displayBufferSet(0, 2, scaled(start, end, 64));
  displayBufferSet(0, 3, scaled(start, end, 128));
  displayBufferSet(0, 4, scaled(start, end, 128+64));
  displayBufferSet(0, 5, scaled(start, end, 128+64+16));
  displayBufferSet(0, 6, scaled(start, end, 255));
  
  displayBufferSet(1, 0, scaled(end, start, 16));
  displayBufferSet(1, 1, scaled(end, start, 32));
  displayBufferSet(1, 2, scaled(end, start, 64));
  displayBufferSet(1, 3, scaled(end, start, 128));
  displayBufferSet(1, 4, scaled(end, start, 128+64));
  displayBufferSet(1, 5, scaled(end, start, 128+64+16));
  displayBufferSet(1, 6, scaled(end, start, 255));
  */
  displayBufferShow();
}

void colorsPattern(uint32_t* pattern) {
  displayBufferReset(COLORS_OFF);
  for (int row = 0; row < PATTERN_SIZE; row++) {
    for (int col = 0; col < PATTERN_SIZE; col++) {
      uint32_t mask = 1 << (PATTERN_SIZE - 1 - col);
      uint32_t overlay = mask & pattern[PATTERN_SIZE - 1 - row];
      if (overlay > 0) {
        displayBufferSet(row, col, COLORS_ON);
      }
    }
  }
  displayBufferShow();
}