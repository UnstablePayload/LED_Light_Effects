#include <FastLED.h>

#define NUM_LEDS 250
#define LED_PIN 5   

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  FastLED.setCorrection(TypicalPixelString);
}

void loop() {

  EVERY_N_MILLISECONDS(25) {
    
    // Create a new HSV color for led[0]
    leds[0] = CHSV(160, random8(), random8(100, 255));
    
    // Copy each pixel to the next one, starting at the far end
    // thereby 'moving' the pattern along the strip
    for (int i = NUM_LEDS - 1; i > 0; i--) {
      leds[i] = leds[i - 1];
    }
  }
  
  FastLED.show();
}