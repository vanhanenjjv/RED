#define LED_DEBUG 0

#include "led.h"

#include <Arduino.h>

#define RED_PIN   (9)
#define GREEN_PIN (11) // ????
#define BLUE_PIN  (10) // ?????????

namespace led {
  void setup() {
    // DDRB |= 0b1110; // set pins as output

    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);

#if LED_DEBUG
    set_color(255, 0, 0);
    delay(1000);
    set_color(0, 255, 0);
    delay(1000);
    set_color(0, 0, 255);
    delay(1000);
#endif
  }

  void set_color(int r, int g, int b)
  {
    analogWrite(RED_PIN, r);
    analogWrite(GREEN_PIN, g);
    analogWrite(BLUE_PIN, b);
  }
}
