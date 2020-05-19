#include <Arduino.h>

#include <WiFi.h>
#include <wificonfig.h>
#include <PubSubClient.h>

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
  payload[length] = '\0';

  char *command;
  char *p = (char *)payload;

  if (strcmp(topic, "/device/atom-lite-1/test") == 0)
  {
    DEBUGVAL(topic, (char *)payload);
  }
  //   if (strcmp(topic, TOPIC_LED_COMMAND) == 0)
  //   {
  //     while ((command = strtok_r(p, "$", &p)) != NULL)
  //     {
  //       if (strcmp(command, "LED") == 0)
  //       {
  //         char *colour = strtok_r(p, "$", &p);
  //         if (strcmp(colour, "RED") == 0)
  //         {
  //           fireButtonLed[0] = CRGB::Red;
  //         }
  //         else if (strcmp(colour, "GREEN") == 0)
  //         {
  //           fireButtonLed[0] = CRGB::Green;
  //         }
  //         else if (strcmp(colour, "BLUE") == 0)
  //         {
  //           fireButtonLed[0] = CRGB::Blue;
  //         }
  //         else if (strcmp(colour, "OFF") == 0)
  //         {
  //           fireButtonLed[0] = CRGB::Black;
  //         }
  //         FastLED.show();
  //       }
  //       else
  //       {
  //         Serial.printf("command?: %s \n", command);
  //       }
  //     }
  //   }
  //   else if (strcmp(topic, TOPIC_TIMESTAMP_10S) == 0)
  //   {
  //     sinceHeardFromMqtt = 0;
  //   }
}

class WiFiLib
{
public:
  WiFiLib()
  {
    _client = new WiFiClient();
    _mqttclient = new PubSubClient(*_client);
  }

  void connectToWifi(const char *ssid, const char *pass)
  {
    int status = 0;

    while (status != WL_CONNECTED)
    {
      Serial.printf("Attempting to connect to WPA SSID: %s\n", ssid);
      status = WiFi.begin(ssid, pass);
      if (status != WL_CONNECTED)
      {
        delay(3000);
      }
      else
      {
        Serial.printf("Connected to wifi!\n");
      }
    }
  }

  void connectToMqtt()
  {
    DEBUG("Connecting to MQTT...");
    _mqttclient->setServer("192.168.1.105", 1883);
    _mqttclient->setCallback(mqttCallback);
    int attempts = 0;
    while (!_mqttclient->connected())
    {
      // Attempt to connect
      _mqttclient->connect(MQTT_NODENAME, "skelstar", pass);
      if (attempts++ > 5)
      {
        Serial.printf("Couldn't connect to MQTT!\n");
        delay(1000);
      }
    }
    Serial.printf("Connected to MQTT!\n");
  }

  void loop()
  {
    _mqttclient->loop();
  }

  void subscribe(char *root, char *subtopic)
  {
    char *topic = _getTopic(root, subtopic);
    _mqttclient->subscribe(topic);
    DEBUGVAL(topic);
  }

  void publish(char *root, char *subtopic, char *payload)
  {
    char *topic = _getTopic(root, subtopic);
    _mqttclient->publish(topic, payload);
    DEBUGVAL(topic, payload);
  }

private:
  WiFiClient *_client;
  PubSubClient *_mqttclient;

  char _topic[40];

  char *_getTopic(char *root, char *subtopic)
  {
    strcpy(_topic, root);
    strcat(_topic, "/");
    strcat(_topic, subtopic);
    return &_topic[0];
  }
};