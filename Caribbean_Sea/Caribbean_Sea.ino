#include <FastLED.h>

#define NUM_LEDS 900
#define LED_PIN 1   

CRGB leds[NUM_LEDS];

uint8_t paletteIndex = 0;


DEFINE_GRADIENT_PALETTE( caribbean_sea ) {
    0,   1, 33, 95,
   38,   1,107, 37,
   76,  42,255, 45,
  127, 255,255, 45,
  178,  42,255, 45,
  216,   1,107, 37,
  255,   1, 33, 95};



CRGBPalette16 myPal = caribbean_sea;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  FastLED.setCorrection(TypicalPixelString);
}

void loop() {
  fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, myPal, 255, LINEARBLEND);

  EVERY_N_MILLISECONDS(75){
    paletteIndex++;
  }
  FastLED.show();
}
