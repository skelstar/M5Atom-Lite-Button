#ifdef DEBUG_SERIAL
#define DEBUG_OUT Serial
#endif
#define PRINTSTREAM_FALLBACK
#include "Debug.hpp"

#include <Arduino.h>
#include <elapsedMillis.h>

#define MQTT_NODENAME "/device/atom-lite-1"

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

// void mqttCallback(char *topic, byte *payload, unsigned int length)
// {
//   payload[length] = '\0';

//   char *command;
//   char *p = (char *)payload;

//   if (strcmp(topic, "/device/atom-lite-1/test") == 0)
//   {
//     DEBUGVAL(topic, (char *)payload);
//   }
//   //   if (strcmp(topic, TOPIC_LED_COMMAND) == 0)
//   //   {
//   //     while ((command = strtok_r(p, "$", &p)) != NULL)
//   //     {
//   //       if (strcmp(command, "LED") == 0)
//   //       {
//   //         char *colour = strtok_r(p, "$", &p);
//   //         if (strcmp(colour, "RED") == 0)
//   //         {
//   //           fireButtonLed[0] = CRGB::Red;
//   //         }
//   //         else if (strcmp(colour, "GREEN") == 0)
//   //         {
//   //           fireButtonLed[0] = CRGB::Green;
//   //         }
//   //         else if (strcmp(colour, "BLUE") == 0)
//   //         {
//   //           fireButtonLed[0] = CRGB::Blue;
//   //         }
//   //         else if (strcmp(colour, "OFF") == 0)
//   //         {
//   //           fireButtonLed[0] = CRGB::Black;
//   //         }
//   //         FastLED.show();
//   //       }
//   //       else
//   //       {
//   //         Serial.printf("command?: %s \n", command);
//   //       }
//   //     }
//   //   }
//   //   else if (strcmp(topic, TOPIC_TIMESTAMP_10S) == 0)
//   //   {
//   //     sinceHeardFromMqtt = 0;
//   //   }
// }

//   subscribe(MQTT_NODENAME, "test");
//   publish(MQTT_NODENAME, "online", "online");

//   Serial.printf("Connected to MQTT! \n");

//   Serial.printf("ip: %s \n", WiFi.localIP());
// }

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

elapsedMillis sinceLastRelease;
bool bumped = false;

void setup()
{
  Serial.begin(115200);
  DEBUG("Ready");

  wifi.connectToWifi(ssid, pass);
  wifi.connectToMqtt();

  wifi.subscribe(MQTT_NODENAME, "test");
  wifi.publish(MQTT_NODENAME, "online", "online");

  // WiFiManager wifiManager;
  // //wifiManager.resetSettings();
  // //set custom ip for portal
  // //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  // //fetches ssid and pass from eeprom and tries to connect
  // //if it does not connect it starts an access point with the specified name
  // //here  "AutoConnectAP"
  // //and goes into a blocking loop awaiting configuration
  // wifiManager.autoConnect("m5-atom-lite");
  // //if you get here you have connected to the WiFi
  // Serial.println("connected...yeey :)");

  // button.setReleasedHandler(toggleMic);

  button.setDoubleClickHandler([](Button2 &btn) {
    wifi.publish(MQTT_NODENAME, "button", "doubleclick");
    DEBUG("--- double click");
  });
  button.setTripleClickHandler([](Button2 &btn) {
    wifi.publish(MQTT_NODENAME, "button", "tripleclick");
    DEBUG("--- triple click!!!");
  });
  button.setLongClickHandler([](Button2 &btn) {
    wifi.publish(MQTT_NODENAME, "button", "longclick");
    DEBUG("--- long click");
  });
  button.setClickHandler([](Button2 &btn) {
    wifi.publish(MQTT_NODENAME, "button", "click");
    DEBUG("--- click");
  });
  button.setReleasedHandler([](Button2 &btn) {
    sinceLastRelease = 0;
    bumped = false;
  });

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);

  FastLED.setBrightness(30);
  setLed(CRGB::Blue);
}

void loop()
{
  button.loop();

  if (sinceLastRelease > 1500 && !bumped)
  {
    bumped = true;
    DEBUG("\n\n\n");
  }

  wifi.loop();
}