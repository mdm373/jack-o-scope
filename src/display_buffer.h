#ifndef DISAPLY_BUFFER
#define DISAPLY_BUFFER

#include <stdlib.h>
#include "hardware/pio.h"

void displayBufferShow();
void displayBufferReset(uint32_t color);
void displayBufferSet(int row, int col, uint32_t color);

#endif