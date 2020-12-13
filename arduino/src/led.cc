#include "led.h"

#include <Arduino.h>

#define LED_PIN (7)

namespace led {
  bool is_on;

  void setup() {
    pinMode(LED_PIN, OUTPUT);
  }

  void set_state(bool is_on) {
    led::is_on = is_on;
    digitalWrite(LED_PIN, is_on);
  }
}
