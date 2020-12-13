#include "nfc.h"

#include <Arduino.h>
#include <avr/wdt.h>

#include "state.h"
#include "raspberry.h"

#define NFC_VERBOSE 1

#define PN532_IRQ   (2)
#define PN532_RESET (3)

namespace nfc {
  Adafruit_PN532 module(PN532_IRQ, PN532_RESET);
  bool card_requested = state::read(NFC_READ_CARD);

  void setup(bool verbose) {
    module.begin();

    uint32_t versiondata = module.getFirmwareVersion();

    if (!versiondata) {
      Serial.println("Didn't find PN53x board");
      while (1); // halt
    }

#if NFC_VERBOSE
    Serial.print(F("Found chip PN5"));
    Serial.println((versiondata >> 24) & 0xFF, HEX);
    Serial.print(F("Firmware ver. "));
    Serial.print((versiondata >> 16) & 0xFF, DEC);
    Serial.print('.');
    Serial.println((versiondata >> 8) & 0xFF, DEC);
#endif

    module.setPassiveActivationRetries(0x00); // (0x00 to not block)
    module.SAMConfig();
  }

  Card read_card() {  
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
    uint8_t uid_length;
    
    bool read = false;
    while (!read) {
      read = module.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uid_length);
      wdt_reset(); // hackerino
    }

    String uid_str;

    for (int i = 0; i < uid_length; ++i) uid_str.concat(uid[i]);

    Card card { uid_str };

    return card;
  }
}