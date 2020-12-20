#ifdef DEBUG_SERIAL
#define DEBUG_OUT Serial
#endif
#define PRINTSTREAM_FALLBACK
#include "Debug.hpp"

#include <Arduino.h>
#include <elapsedMillis.h>

#define MQTT_NODENAME "/device/atom-lite-2"

#include <WiFiLib.h>

WiFiLib wifi;

// #include <DNSServer.h>
// #include <WebServer.h>
// #include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

//---------------------------------------------------------------
#include <FastLED.h>

#define NUM_LEDS 1
#define LED_PIN 27

CRGB leds[NUM_LEDS];

void setLed(CRGB colour)
{
  leds[0] = colour;
  FastLED.show();
}

//---------------------------------------------------------------

#include <Button2.h>

Button2 button(39);

//---------------------------------------------------------------

char *parseClickType(int type)
{

  switch (type)
  {
  case SINGLE_CLICK:
    return ("SINGLE_CLICK");
  case DOUBLE_CLICK:
    return ("DOUBLE_CLICK");
  case TRIPLE_CLICK:
    return ("TRIPLE_CLICK");
  case LONG_CLICK:
    return ("LONG_CLICK");
  default:
    return ("unable to parse type");
  }
}

//---------------------------------------------------------------

elapsedMillis sinceLastRelease;
bool bumped = false;

void setButtonCallbacks()
{
  button.setDoubleClickHandler([](Button2 &btn) {
    wifi.publish(MQTT_NODENAME, "button", "doubleclick");
  });
  button.setTripleClickHandler([](Button2 &btn) {
    wifi.publish(MQTT_NODENAME, "button", "tripleclick");
  });
  button.setLongClickHandler([](Button2 &btn) {
    wifi.publish(MQTT_NODENAME, "button", "longclick");
  });
  button.setClickHandler([](Button2 &btn) {
    wifi.publish(MQTT_NODENAME, "button", "click");
  });
  button.setReleasedHandler([](Button2 &btn) {
    sinceLastRelease = 0;
    bumped = false;
  });
}

//---------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  DEBUG("Ready");

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(30);
  setLed(CRGB::Blue);

  wifi.connectToWifi(ssid, pass);
  wifi.connectToMqtt();

  setLed(CRGB::Green);

  wifi.subscribe(MQTT_NODENAME, "test");
  wifi.publish(MQTT_NODENAME, "online", "online");

  setButtonCallbacks();
}

void loop()
{
  button.loop();

  wifi.loop();
}