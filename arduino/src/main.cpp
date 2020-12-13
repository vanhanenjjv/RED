#include <Arduino.h>
#include <ArduinoJson.h>
#include <avr/wdt.h>

#include "nfc.h"
#include "timer1.h"
#include "led.h"
#include "raspberry.h"
#include "state.h"

#define BAUD_RATE (115200)

void setup(void) {
  wdt_enable(WDTO_8S);

  Serial.begin(BAUD_RATE);

  led::setup();
  nfc::setup(true);
  timer1::setup();  
}

void loop(void) {
  if (nfc::card_requested) {
    auto card = nfc::read_card();

    raspberry::Message message { CARD, &card };
    raspberry::transmit(message);
  }

  raspberry::receive();

  wdt_reset();
}
