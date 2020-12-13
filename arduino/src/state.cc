#include "state.h"

#include <Arduino.h>

void state::write(unsigned int address, unsigned char data) {
  /* Wait for completion of previous write */
  while (EECR & (1 << EEPE));
  /* Set up address and Data Registers */
  EEAR = address;
  EEDR = data;
  /* Write logical one to EEMPE */
  EECR |= (1 << EEMPE);
  /* Start eeprom write by setting EEPE */
  EECR |= (1 << EEPE);
}

unsigned char state::read(unsigned int address) {
  /* Wait for completion of previous write */
  while(EECR & (1<<EEPE));
  /* Set up address register */
  EEAR = address;
  /* Start eeprom read by writing EERE */
  EECR |= (1<<EERE);
  /* Return data from Data Register */
  return EEDR;
}
