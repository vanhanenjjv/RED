#ifndef MESSAGE_H
#define MESSAGE_H

#include "nfc.h"

// For sending data
#define NFC_ADD_CARD          0x10
#define NFC_AUTHENTICATE_CARD 0x11

// For receiving data
#define NFC_AUTHORIZE_CARD        0x00
#define NFC_AUTHENTICATION_RESULT 0x01
#define LED_ON                    0x02  // not used
#define LED_OFF                   0x03  // not used

namespace raspberry {

   struct Message {
    uint8_t type;
    union {
      nfc::Card *card;
    };
  };

  void handle();
  void transmit(Message message);
  void receive();
};

#endif
