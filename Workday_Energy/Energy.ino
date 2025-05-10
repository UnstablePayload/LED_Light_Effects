#include <FastLED.h>

#define NUM_LEDS 900
#define LED_PIN 1   

CRGB leds[NUM_LEDS];


void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  FastLED.setCorrection(TypicalPixelString);
}

void loop() {
  fill_solid(leds, NUM_LEDS, CRGB::White);
  FastLED.show();
}
