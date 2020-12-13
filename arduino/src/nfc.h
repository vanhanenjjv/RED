#ifndef NFC_H
#define NFC_H

#include <Arduino.h>
#include <Adafruit_PN532.h>

namespace nfc {
  struct Card {
    String uid;
  };

  extern Adafruit_PN532 module;
  extern bool card_requested;

  void setup(bool verbose = true);
  Card read_card();
}

#endif
