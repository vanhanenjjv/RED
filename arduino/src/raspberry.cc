#include "raspberry.h"

#include <Arduino.h>
#include <ArduinoJson.h>

#include "state.h"

namespace raspberry {
  void transmit(Message message) {
    StaticJsonDocument<256> document;

    document[F("type")] = message.type;

    switch (message.type) {
      case CARD: {
        document[F("card")][F("uid")] = message.card->uid;
        nfc::card_requested = false;
        state::write(NFC_READ_CARD, 0);
        break;
      }
    }

    serializeJson(document, Serial);
    Serial.println();
  }

  void handle(StaticJsonDocument<256> &document) {
    uint8_t type = document[F("type")];

    switch (type) {
      case NFC_READ_CARD: {
        nfc::card_requested = true;
        state::write(NFC_READ_CARD, 1);
        break;
      }
    }
  }

  void receive() {
    while (Serial.available()) {
      StaticJsonDocument<256> document;
      deserializeJson(document, Serial);
      
      handle(document);
    }
  }
};
