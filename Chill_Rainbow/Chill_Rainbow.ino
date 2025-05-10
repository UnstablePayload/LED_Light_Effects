#include <FastLED.h>

#define NUM_LEDS 900
#define LED_PIN 1   // The Nano pin is 2 but the ESP32 pin is 5

CRGB leds[NUM_LEDS];

uint8_t hue = 0;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  FastLED.setCorrection(TypicalPixelString);
}

void loop() {
  
  for (int i = 0; i < NUM_LEDS; i++) {
    //leds[i] = CHSV(hue, 255, 255);
    leds[i] = CHSV(hue + (i * 5), 255, 255);
  }

  EVERY_N_MILLISECONDS(15){
    hue++;
  }

  FastLED.show();
}