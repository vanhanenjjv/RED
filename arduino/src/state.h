#ifndef STATE_H
#define STATE_H

#define NFC_CARD_REQUESTED_STATE 0x00

namespace state {
  void write(unsigned int address, unsigned char data);
  unsigned char read(unsigned int address);
}

#endif
