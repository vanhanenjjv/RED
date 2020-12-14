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

unsigned long timer1::start_time = 0;

//Runs every 500 ms
ISR(TIMER1_COMPA_vect) {
  TCNT1 = TL_LOAD;

  if (nfc::card_authentication_requested || nfc::card_requested) {
    led::set_color(255, 255, 0);  // Yellow
  } 
  else led::set_color(0, 0, 0); // Nothing
  
  if (nfc::card_authorized != 0) { // card has been either accepted or declined

    Serial.println("jajaaa");

    if (timer1::start_time == 0) 
      timer1::start_time = millis(); // set the start time

    else { // else compare the start time to current time
      if (millis() - timer1::start_time >= 2000) { // by checking if 2 seconds has passed
        Serial.println("WHYYYYY");
        nfc::card_authorized = 0; // then set the card authorized state back to none
        timer1::start_time = 0;  // and set the start time again to zero
      }
    }

    switch (nfc::card_authorized) {
      case CARD_AUTHORIZED: {
        led::set_color(0, 255, 0);
        break;
      }
      case CARD_UNAUTHORIZED: {
        led::set_color(255, 0, 0);
        break;
      }
    }
  } 

}