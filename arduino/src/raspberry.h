#ifndef MESSAGE_H
#define MESSAGE_H

#include "nfc.h"

// For sending data
#define CARD          0x10

// For receiving data
#define NFC_READ_CARD 0x00
#define LED_ON        0x01  // not used
#define LED_OFF       0x02  // not used

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
