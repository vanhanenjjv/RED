#include "raspberry.h"

#include <Arduino.h>
#include <ArduinoJson.h>

#include "state.h"

namespace raspberry {
  void transmit(Message message) {
    StaticJsonDocument<256> document;

    document[F("type")] = message.type;

    switch (message.type) {
      case NFC_ADD_CARD: {
        document[F("card")][F("uid")] = message.card->uid;
        nfc::card_requested = false;
        state::write(NFC_CARD_REQUESTED_STATE, 0);
        break;
      }
      case NFC_AUTHENTICATE_CARD: {
        document[F("card")][F("uid")] = message.card->uid;
        nfc::card_authentication_requested = false;
        break;
      }
    }

    serializeJson(document, Serial);
    Serial.println();
  }

  void handle(StaticJsonDocument<256> &document) {
    uint8_t type = document[F("type")];

    switch (type) {
      case NFC_AUTHORIZE_CARD: {
        nfc::card_requested = true;
        state::write(NFC_CARD_REQUESTED_STATE, 1);
        break;
      }
      case NFC_AUTHENTICATION_RESULT: {
        bool authorized = document["authorized"].as<bool>();
        nfc::card_authorized = authorized ? CARD_AUTHORIZED : CARD_UNAUTHORIZED;
        Serial.println("card authorized = " + authorized);
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
