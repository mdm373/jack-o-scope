#ifndef DISAPLY_BUFFER_HEADER
#define DISAPLY_BUFFER_HEADER

#include <stdlib.h>
#include "hardware/pio.h"

void displayBufferShow();
void displayBufferReset(uint32_t color);
void displayBufferSet(int row, int col, uint32_t color);

#endif