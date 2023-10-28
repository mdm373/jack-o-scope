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
#define ANIM_DELTA 0x08
#define FORTUNE_DURATION 5000
#define LONG_PRESS_DURATION 750
#define HOLD_DURATION 10000

#define PATTERN_COUNT 5
static uint32_t* PATTERNS[PATTERN_COUNT] = {
    PATTERN_PACMAN, PATTERN_ANIME, PATTERN_SMILE, PATTERN_TEETH, PATTERN_WAT};

#define FORTUNE_COUNT 5
static uint32_t* FORTUNES[FORTUNE_COUNT] = {PATTERN_NO, PATTERN_HEART, PATTERN_YES};

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

#define LONG_PRESS 0x02
#define SHORT_PRESS 0x01
#define NO_PRESS 0x00
#define BUTTON_PRESS int8_t
static bool wasPressed = false;
static bool isLong = false;
static int duration;
static BUTTON_PRESS pollButtonPress() {
  const bool nowPressed = gpio_get(BUTTON_PIN);
  if (nowPressed) {
    if (!wasPressed) {
      isLong = false;
      duration = 0;
    }

    duration = duration + REFRESH;
    if (duration > LONG_PRESS_DURATION) {
      isLong = true;
      duration = 0;  // prevent overflow
    }
    wasPressed = true;
    return NO_PRESS;
  }

  if (!wasPressed) {
    return NO_PRESS;
  }

  wasPressed = false;
  if (isLong) {
    return LONG_PRESS;
  }
  return SHORT_PRESS;
}

static inline void put_pixel(uint32_t pixel_grb) {
  pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

void pattern_sparkle() {
  for (int i = 0; i < (PATTERN_SIZE * PATTERN_SIZE); ++i) put_pixel(rand());
}

static uint16_t fadeOutDelta;
static bool isFadingOut(bool isStart, uint32_t* pattern, uint32_t color) {
  if (isStart) {
    fadeOutDelta = 0;
  } else {
    fadeOutDelta = fadeOutDelta + ANIM_DELTA;
  }

  if (fadeOutDelta > 0x0FF) {
    fadeOutDelta = 0x0FF;
    return false;
  }

  colorsTransition(pattern, &PATTERN_BLANK[0], fadeOutDelta, color, COLORS_OFF);
  return true;
}

static uint16_t fadeInDelta;
static void fadeIn(bool isStart, uint32_t* pattern, uint32_t color) {
  if (isStart) {
    fadeInDelta = 0;
  } else {
    fadeInDelta = fadeInDelta + ANIM_DELTA;
  }

  if (fadeInDelta > 0x0FF) {
    fadeInDelta = 0x0FF;
  }

  colorsTransition(&PATTERN_BLANK[0], pattern, fadeInDelta, COLORS_OFF, color);
}

static uint16_t fortuneDelta;
static uint32_t* fortuneResult;
static uint8_t fortuneColor;
static uint16_t fortuneFade = 0;
static bool isFortuneShow(bool isStart) {
  if (isStart) {
    fortuneDelta = 0;
    fortuneFade = 0;
    int fortuneIndex = rand() % FORTUNE_COUNT;
    fortuneResult = &FORTUNES[fortuneIndex][0];
    fortuneColor = FORTUNES_COLOR[fortuneIndex];
  } else {
    fortuneDelta = fortuneDelta + REFRESH;
  }

  if (fortuneDelta > FORTUNE_DURATION) {
    fortuneDelta = 0x0FF;
    return false;
  }

  int fortuneBase = FORTUNE_DURATION / 8;
  if (fortuneDelta < fortuneBase) {
    pattern_sparkle();
    return true;
  }

  fortuneFade = fortuneFade + ANIM_DELTA;
  if(fortuneFade > 0xFF) {
    fortuneFade = 0xFF;
  }
  colorsTransition(PATTERN_BLANK, fortuneResult, fortuneFade, COLORS_OFF, COLORS_ON);
  return true;
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
  bool wasFortune = false;
  int patternIndex = 0;
  int currentDuration = 0;
  uint32_t* currentPattern = &PATTERNS[patternIndex][0];
  while (1) {
    sleep_ms(REFRESH);
    const BUTTON_PRESS press = pollButtonPress();
    bool isOver = (currentDuration > HOLD_DURATION);
    bool isFading = (press == SHORT_PRESS) || isOver;
    bool isFortune = (press == LONG_PRESS);
    if(isOver) {
      currentDuration = 0;
    }
    isFading = (wasFading || isFading) && isFadingOut(isFading, currentPattern, COLORS_ON);
    isFortune = (isFortune || wasFortune) && isFortuneShow(isFortune);
    if ((!isFading && wasFading) || (!isFortune && wasFortune)) {
      patternIndex = (patternIndex + 1) % PATTERN_COUNT;
      currentPattern = &PATTERNS[patternIndex][0];
    }
    if (!isFading && !isFortune) {
      currentDuration = currentDuration + REFRESH;
      fadeIn(wasFading, &PATTERNS[patternIndex][0], COLORS_ON);
    }
    wasFading = isFading;
    wasFortune = isFortune;
  }
}
