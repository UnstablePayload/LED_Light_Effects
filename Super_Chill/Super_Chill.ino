#include <FastLED.h>

#define NUM_LEDS 900
#define LED_PIN 1   

CRGB leds[NUM_LEDS];

uint8_t paletteIndex = 0;


DEFINE_GRADIENT_PALETTE( bhw1_28_gp ) {
    0,  75,  1,221,
   30, 252, 73,255,
   48, 169,  0,242,
  119,   0,149,242,
  170,  43,  0,242,
  206, 252, 73,255,
  232,  78, 12,214,
  255,   0,149,242};


CRGBPalette16 myPal = bhw1_28_gp;

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
