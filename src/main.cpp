#ifdef DEBUG_SERIAL
#define DEBUG_OUT Serial
#endif
#define PRINTSTREAM_FALLBACK
#include "Debug.hpp"

#include <Arduino.h>
#include <elapsedMillis.h>

#include <FastLED.h>

#define NUM_LEDS 1
#define LED_PIN 27

CRGB leds[NUM_LEDS];

//---------------------------------------------------------------
#define DOUBLECLICK_MS 300
#define LONGCLICK_MS 1000

#include <Button2.h>

Button2 button(39);

void setLed(CRGB colour)
{
  leds[0] = colour;
  FastLED.show();
}
//---------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  DEBUG("Ready");

  // button.setReleasedHandler(toggleMic);
  button.setDoubleClickHandler([](Button2 &btn) {
    DEBUG("double click");
  });
  button.setTapHandler([](Button2 &btn) {
    DEBUG("tap");
  });
  button.setLongClickHandler([](Button2 &btn) {
    DEBUG("long click");
  });

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);

  FastLED.setBrightness(30);
  setLed(CRGB::Blue);
}

void loop()
{
  button.loop();
}