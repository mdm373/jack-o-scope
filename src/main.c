#include <stdio.h>
#include <stdlib.h>

#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include "ws2812.pio.h"
#include "display_buffer.h"
#if PATTERN_SIZE == 7
  #include "patterns_7.h"
#endif


static const uint32_t COLOR_ON = ~0;
static const uint32_t COLOR_OFF = 0;

#define PATTERN_COUNT 2
static uint32_t* PATTERNS[PATTERN_COUNT] = {PATTERN_ANIME, PATTERN_SMILE};

static inline uint32_t showPattern(uint32_t* pattern) {
  displayBufferReset(COLOR_OFF);
  for (int row = 0; row < PATTERN_SIZE; row++) {
    for (int col = 0; col < PATTERN_SIZE; col++) {
      uint32_t mask = 1 << (PATTERN_SIZE - 1 - col);
      uint32_t overlay = mask & pattern[PATTERN_SIZE - 1 - row];
      if (overlay > 0) {
        displayBufferSet(row, col, COLOR_ON);
      }
    }
  }
  displayBufferShow();
}

int main() {
  stdio_init_all();
  PIO pio = pio0;
  uint offset = pio_add_program(pio, &ws2812_program);
  ws2812_program_init(pio, 0, offset, WS2812_PIN, 800000, IS_RGBW);
  int patternIndex = 0;
  while (1) {
    patternIndex++;
    if(patternIndex >= PATTERN_COUNT) {
      patternIndex = 0;
    }
    showPattern(&PATTERNS[patternIndex][0]);
    sleep_ms(3000);
  }
}
