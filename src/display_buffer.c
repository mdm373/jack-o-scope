
#include "display_buffer.h"

#define NUM_PIXELS PATTERN_SIZE*PATTERN_SIZE
static uint32_t displayBuffer[NUM_PIXELS];

void put_pixel(uint32_t pixel_grb) {
  pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

void displayBufferReset(uint32_t color) {
  for (int i = 0; i < NUM_PIXELS; i++) {
    displayBuffer[i] = color;
  }
}

void displayBufferShow() {
  for (int i = 0; i < NUM_PIXELS; i++) {
    put_pixel(displayBuffer[i]);
  }
}

void displayBufferSet(int row, int col, uint32_t color) {
  int index = row * PATTERN_SIZE;
  bool even = (col % 2) == 0;
  int basis = 0;
  int dir = 1;
  if (!even) {
    basis = PATTERN_SIZE - 1;
    dir = -1;
  }
  displayBuffer[index + basis + (col * dir)] = color;
}
