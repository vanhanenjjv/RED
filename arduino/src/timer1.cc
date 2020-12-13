#include "timer1.h"

#include <Arduino.h>

#include "led.h"
#include "nfc.h"

#define TL_LOAD (0)
#define TL_COMP (31250)

void timer1::setup() {
  noInterrupts();

  //reset timer1 control reg A
  TCCR1A = 0;

  //prescaler 256
  TCCR1B |= (1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS10);

  //reset timer 1 and set compare value
  TCNT1 = TL_LOAD;
  OCR1A = TL_COMP;

  //enable timer1 compare interrupt
  TIMSK1 = (1 << OCIE1A);

  //enable global interrupt
  interrupts();
}

//Runs every 500 ms
ISR(TIMER1_COMPA_vect) {
  TCNT1 = TL_LOAD;

  led::set_state(nfc::card_requested);  // Turns the LED on if the card is requested
}