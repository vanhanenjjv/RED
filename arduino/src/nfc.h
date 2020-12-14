#ifndef NFC_H
#define NFC_H

#include <Arduino.h>
#include <Adafruit_PN532.h>

#define CARD_AUTHORIZED   1
#define CARD_UNAUTHORIZED 2

namespace nfc {
  struct Card {
    String uid;
  };

  extern Adafruit_PN532 module;
  extern bool card_requested;
  extern bool card_authentication_requested;
  extern int card_authorized;

  void setup(bool verbose = true);
  Card read_card();
}

#endif
