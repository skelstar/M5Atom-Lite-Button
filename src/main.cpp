#ifdef DEBUG_SERIAL
#define DEBUG_OUT Serial
#endif

#include <Arduino.h>
#include <elapsedMillis.h>
#include <Button2.h>

//---------------------------------------------------------------

#include <RF24Network.h>
#include <NRF24L01Lib.h>

NRF24L01Lib nrf24;

RF24 radio(SPI_CE, SPI_CS);
RF24Network network(radio);

//---------------------------------------------------------------

void clickHandler(Button2 &btn)
{
}

Button2 button(39);

//---------------------------------------------------------------

void packetAvailable_cb(uint16_t from_id, uint8_t t)
{
  Serial.printf("Rx packet from %d!\n", from_id);

  RF24NetworkHeader header;
  uint8_t len = sizeof(uint16_t);
  uint8_t buff[len];
  network.read(header, buff, len);
}

//---------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  Serial.printf("\n\nReady\n");

  nrf24.begin(&radio, &network, 02, packetAvailable_cb);
}

void loop()
{
  nrf24.update();
  vTaskDelay(10);
}