#include <FastLED.h>

#define NUM_LEDS 250
#define LED_PIN 5
#define FADE_STEPS 150
#define FADE_TIME 10000
#define BEAM_LENGTH 20
#define BEAM_GAP 20
#define DELAY_TIME 120000

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(225);
  FastLED.setCorrection(TypicalLEDStrip);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

void animateEndsToCenter(CRGB color, unsigned long delayTime) {
  for (int i = 0; i < NUM_LEDS / 2; i++) {
    leds[i] = color;
    leds[NUM_LEDS - 1 - i] = color;
    FastLED.show();
    delay(delayTime);
  }
}

void animateCenterToEnds(CRGB color, unsigned long delayTime) {
  for (int i = (NUM_LEDS / 2) - 1; i >= 0; i--) {
    leds[i] = color;
    leds[NUM_LEDS - 1 - i] = color;
    FastLED.show();
    delay(delayTime);
  }
}

void fadeToColor(CRGB newColor, unsigned long transitionTime) {
  CRGB oldColor = leds[0];
  for (int step = 0; step <= FADE_STEPS; step++) {
    uint8_t blendAmount = map(step, 0, FADE_STEPS, 0, 225);
    CRGB inBetween = blend(oldColor, newColor, blendAmount);
    fill_solid(leds, NUM_LEDS, inBetween);
    FastLED.show();
    delay(transitionTime / FADE_STEPS);
  }
  fill_solid(leds, NUM_LEDS, newColor);
  FastLED.show();
}

void shootBeamsToMiddle(CRGB colorLeft, CRGB colorRight, unsigned long stepDelayMs) {
  for (int i = 0; i <= (NUM_LEDS / 2 - BEAM_LENGTH); i++) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    for (int j = 0; j < BEAM_LENGTH; j++) {
      int leftIndex = i + j;
      if (leftIndex < NUM_LEDS) leds[leftIndex] = colorLeft;
    }
    for (int j = 0; j < BEAM_LENGTH; j++) {
      int rightIndex = (NUM_LEDS - 1 - i) - j;
      if (rightIndex >= 0) leds[rightIndex] = colorRight;
    }
    FastLED.show();
    delay(stepDelayMs);
  }
}

void shootBeamsFromMiddle(CRGB colorLeft, CRGB colorRight, unsigned long stepDelayMs) {
  int leftCenter  = (NUM_LEDS / 2) - 1;
  int rightCenter = (NUM_LEDS / 2);
  for (int i = 0; i <= (NUM_LEDS / 2 + BEAM_LENGTH); i++) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    for (int j = 0; j < BEAM_LENGTH; j++) {
      int index = (leftCenter - i) - j;
      if (index >= 0 && index < NUM_LEDS) leds[index] = colorLeft;
    }
    for (int j = 0; j < BEAM_LENGTH; j++) {
      int index = (rightCenter + i) + j;
      if (index >= 0 && index < NUM_LEDS) leds[index] = colorRight;
    }
    FastLED.show();
    delay(stepDelayMs);
  }
}

void shootThreeBeamsLeft(CRGB color, unsigned long stepDelayMs) {
  int offset2 = BEAM_LENGTH + BEAM_GAP;       // When beam #2 starts
  int offset3 = 2 * (BEAM_LENGTH + BEAM_GAP); // When beam #3 starts
  int maxSteps = (NUM_LEDS - 1 + BEAM_LENGTH) + offset3;
  for (int i = 0; i <= maxSteps; i++) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    int front1 = (NUM_LEDS - 1) - i;
    lightBeam(front1, color);
    if (i >= offset2) {
      int front2 = (NUM_LEDS - 1) - (i - offset2);
      lightBeam(front2, color);
    }
    if (i >= offset3) {
      int front3 = (NUM_LEDS - 1) - (i - offset3);
      lightBeam(front3, color);
    }
    FastLED.show();
    delay(stepDelayMs);
  }
}

void lightBeam(int frontIndex, CRGB color) {
  for (int b = 0; b < BEAM_LENGTH; b++) {
    int idx = frontIndex - b;
    if (idx >= 0 && idx < NUM_LEDS) leds[idx] = color;
  }
}

void shootThreeBeamsRight(CRGB color, unsigned long stepDelayMs) {
  int offset2 = BEAM_LENGTH + BEAM_GAP;
  int offset3 = 2 * (BEAM_LENGTH + BEAM_GAP);
  int maxSteps = (NUM_LEDS - 1 + BEAM_LENGTH) + offset3;
  for (int i = 0; i <= maxSteps; i++) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    if (i >= 0) {
      int front1 = i;
      lightBeamRight(front1, color);
    }
    if (i >= offset2) {
      int front2 = i - offset2;
      lightBeamRight(front2, color);
    }
    if (i >= offset3) {
      int front3 = i - offset3;
      lightBeamRight(front3, color);
    }
    FastLED.show();
    delay(stepDelayMs);
  }
}

void lightBeamRight(int frontIndex, CRGB color) {
  for (int b = 0; b < BEAM_LENGTH; b++) {
    int idx = frontIndex + b;
    if (idx >= 0 && idx < NUM_LEDS) leds[idx] = color;
  }
}

void shootThreeBeamsOppositeBounce(CRGB colorLeft, CRGB colorRight, unsigned long stepDelayMs) {
  struct Beam {
    int front;
    int dir;
    CRGB color;
    bool active;
  };
  Beam beams[6];
  for (int i = 0; i < 3; i++) {
    beams[i].front = -i * (BEAM_LENGTH + BEAM_GAP);
    beams[i].dir = 1;
    beams[i].color = colorLeft;
    beams[i].active = true;
  }
  for (int i = 3; i < 6; i++) {
    beams[i].front = NUM_LEDS + (i - 3) * (BEAM_LENGTH + BEAM_GAP);
    beams[i].dir = -1;
    beams[i].color = colorRight;
    beams[i].active = true;
  }
  bool running = true;
  while (running) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    for (int i = 0; i < 6; i++) {
      if (beams[i].active) beams[i].front += beams[i].dir;
    }
    for (int l = 0; l < 3; l++) {
      for (int r = 3; r < 6; r++) {
        if (beams[l].active && beams[r].active && beams[l].front >= beams[r].front) {
          beams[l].dir = -1;
          beams[r].dir = 1;
          beams[l].front = beams[r].front;
        }
      }
    }
    for (int i = 0; i < 6; i++) {
      if (beams[i].active) drawBeam(beams[i].front, beams[i].color, beams[i].dir);
    }
    FastLED.show();
    delay(stepDelayMs);
    for (int i = 0; i < 6; i++) {
      if (beams[i].active) {
        if (beams[i].dir > 0) {
          int tail = beams[i].front - (BEAM_LENGTH - 1);
          if ((beams[i].front < 0 && tail < 0) || (beams[i].front >= NUM_LEDS && tail >= NUM_LEDS)) beams[i].active = false;
        } else {
          int tail = beams[i].front + (BEAM_LENGTH - 1);
          if ((beams[i].front >= NUM_LEDS && tail >= NUM_LEDS) || (beams[i].front < 0 && tail < 0)) beams[i].active = false;
        }
      }
    }
    running = false;
    for (int i = 0; i < 6; i++) {
      if (beams[i].active) {
        running = true;
        break;
      }
    }
  }
}

void drawBeam(int front, CRGB color, int direction) {
  for (int b = 0; b < BEAM_LENGTH; b++) {
    int idx = (direction > 0) ? front - b : front + b;
    if (idx >= 0 && idx < NUM_LEDS) leds[idx] = color;
  }
}

void loop() {
  animateEndsToCenter(CRGB::Aqua, 20);
  delay(DELAY_TIME);
  animateCenterToEnds(CRGB::Purple, 20);
  delay(DELAY_TIME);
  animateEndsToCenter(CRGB::Chartreuse, 20);
  delay(DELAY_TIME);
  animateCenterToEnds(CRGB::Blue, 20);
  delay(DELAY_TIME);
  animateEndsToCenter(CRGB::DarkOrange, 20); 
  delay(DELAY_TIME);
  animateCenterToEnds(CRGB::DeepPink, 20);
  delay(DELAY_TIME);
  fadeToColor(CRGB::Aqua, FADE_TIME);
  delay(DELAY_TIME);
  fadeToColor(CRGB::Purple, FADE_TIME);
  delay(DELAY_TIME);
  fadeToColor(CRGB::Chartreuse, FADE_TIME);
  delay(DELAY_TIME);
  fadeToColor(CRGB::Blue, FADE_TIME);
  delay(DELAY_TIME);
  fadeToColor(CRGB::DarkOrange, FADE_TIME);
  delay(DELAY_TIME);
  fadeToColor(CRGB::DeepPink, FADE_TIME);
  delay(DELAY_TIME);
  shootBeamsToMiddle(CRGB::Aqua, CRGB::Aqua, 20);
  shootBeamsFromMiddle(CRGB::Purple, CRGB::Purple, 20);
  shootBeamsToMiddle(CRGB::Chartreuse, CRGB::Chartreuse, 20);
  shootBeamsFromMiddle(CRGB::Blue, CRGB::Blue, 20);
  shootBeamsToMiddle(CRGB::DarkOrange, CRGB::DarkOrange, 20);
  shootBeamsFromMiddle(CRGB::DeepPink, CRGB::DeepPink, 20);
  shootThreeBeamsLeft(CRGB::Aqua, 20);
  shootThreeBeamsRight(CRGB::Purple, 20);
  shootThreeBeamsLeft(CRGB::Chartreuse, 20);
  shootThreeBeamsRight(CRGB::Blue, 20);
  shootThreeBeamsLeft(CRGB::DarkOrange, 20);
  shootThreeBeamsRight(CRGB::DeepPink, 20);
  shootThreeBeamsOppositeBounce(CRGB::Aqua, CRGB::Purple, 20);
  shootThreeBeamsOppositeBounce(CRGB::Chartreuse, CRGB::Blue, 20);
  shootThreeBeamsOppositeBounce(CRGB::DarkOrange, CRGB::DeepPink, 20);
}