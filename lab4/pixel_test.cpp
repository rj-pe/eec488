*********************************************************************
 file: main_pwm_test.cpp - demo basic pwm a tricolor led
*********************************************************************

//Dr. Chu's test code
#include "de10_baseline.h"
#include "led_pixel_core.h"  // class def header file
// create an led pixel instance
// PWM IP core in slot #6; channels 5, 6, 7 used for tricolor LED
LedPixelCore pixel(get_slot_addr(BRIDGE_BASE, S6_PWM), 5, 6, 7);
int main() {
   uint8_t i, r, g, b;
   pixel.set_active_level(0); // x is 0 or 1 (CA or CC tricolor LED)
   while (1) {
      int s = sw.read();
      // obtain color
      i = s & 0x03;      // sw(1:0) for blue
      b = 85*i;          // blue intensity: 0, 85, 170, 255
      i = (s>>2) & 0x03; // sw(3:2) for green
      g = 85*i;
      i = (s>>4) & 0x03; // sw(5:4) for red
      r = 85*i;
      // obtain 4‐bit brightness level from sw(9:6)
      float level = (float) ((s>>6) & 0x0f) / 15.0;
      // set color and brightness
      pixel.set_color(r, g, b);
      pixel.set_brightness(level);
   }
}

