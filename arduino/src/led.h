#ifndef LED_H
#define LED_H

namespace led {
  extern bool is_on;

  void setup();
  void set_state(bool is_on);
  void set_color(int r, int g, int b);
}

#endif
