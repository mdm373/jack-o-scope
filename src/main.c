#include <stdio.h>
#include <stdlib.h>

#include "colors.h"
#include "display_buffer.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include "ws2812.pio.h"
#include PATTERN_PATH

#define REFRESH 33
#define ANIM_DELTA 0x08;
#define PATTERN_COUNT 4
static uint32_t* PATTERNS[PATTERN_COUNT] = {PATTERN_PACMAN, PATTERN_ANIME,
                                            PATTERN_SMILE, PATTERN_TEETH};

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

static bool wasPressed = false;
static bool isButtonPressed() {
  const bool nowPressed = gpio_get(BUTTON_PIN);
  if (nowPressed) {
    wasPressed = true;
    return false;
  }

  if (!wasPressed) {
    return false;
  }

  wasPressed = false;
  return true;
}

static uint16_t fadeOutDelta;
static bool isFadingOut(bool isStart, uint32_t* pattern) {
  if (isStart) {
    fadeOutDelta = 0;
  } else {
    fadeOutDelta = fadeOutDelta + ANIM_DELTA;
  }

  if (fadeOutDelta > 0x0FF) {
    fadeOutDelta = 0x0FF;
    return false;
  }

  colorsTransition(pattern, &PATTERN_BLANK[0], fadeOutDelta);
  return true;
}

static uint16_t fadeInDelta;
static void fadeIn(bool isStart, uint32_t* pattern) {
  if (isStart) {
    fadeInDelta = 0;
  } else {
    fadeInDelta = fadeInDelta + ANIM_DELTA;
  }

  if (fadeInDelta > 0x0FF) {
    fadeInDelta = 0x0FF;
  }

  colorsTransition(&PATTERN_BLANK[0], pattern, fadeInDelta);
}

int main() {
  gpio_init(BUTTON_PIN);
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(BUTTON_PIN, GPIO_IN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  stdio_init_all();

  PIO pio = pio0;
  uint offset = pio_add_program(pio, &ws2812_program);
  ws2812_program_init(pio, 0, offset, WS2812_PIN, 800000, false);

  bool wasFading = false;
  int patternIndex = 0;
  uint32_t* currentPattern = &PATTERNS[patternIndex][0];
  while (1) {
    sleep_ms(REFRESH);
    const bool buttonPressed = isButtonPressed();
    const bool isFading = isFadingOut(buttonPressed, currentPattern);
    if (!isFading && wasFading) {
      patternIndex = (patternIndex + 1) % PATTERN_COUNT;
      currentPattern = &PATTERNS[patternIndex][0];
    }
    if (!isFading) {
      fadeIn(wasFading, &PATTERNS[patternIndex][0]);
    }
    wasFading = isFading;
  }
}
