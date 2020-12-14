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

  attachInterrupt(
    digitalPinToInterrupt(3), 
    []{
      nfc::card_authentication_requested = true;
    }, 
    INPUT_PULLUP);
}

void loop(void) {
  if (nfc::card_authentication_requested) {
    auto card = nfc::read_card();

    raspberry::Message message { NFC_AUTHENTICATE_CARD, &card };
    raspberry::transmit(message);
  }

  if (nfc::card_requested) {
    auto card = nfc::read_card();

    raspberry::Message message { NFC_ADD_CARD, &card };
    raspberry::transmit(message);
  }

  raspberry::receive();

  wdt_reset();
}
