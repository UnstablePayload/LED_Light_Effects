#include <FastLED.h>

#define NUM_LEDS 31
#define LED_PIN 1   

CRGB leds[NUM_LEDS];

uint8_t paletteIndex = 0;

DEFINE_GRADIENT_PALETTE( Sunset_Real ) {
    0, 120,  0,  0,
   22, 179, 22,  0,
   51, 255,104,  0,
   85, 167, 22, 18,
  135, 100,  0,103,
  198,  16,  0,130,
  255,   0,  0,160
};

CRGBPalette16 myPal = Sunset_Real;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  FastLED.setCorrection(TypicalPixelString);
}

void loop() {
  fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, myPal, 255, LINEARBLEND);

  EVERY_N_MILLISECONDS(50){
    paletteIndex++;
  }
  FastLED.show();
}