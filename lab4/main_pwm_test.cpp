*********************************************************************
 file: main_pwm_test.cpp - demo basic pwm a tricolor led

 Copyright (C) 2018  p. chu

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************

#include "de10_baseline.h"

#define RED_CH5 5  // pwm(5) for red led
#define GRN_CH6 6  // pwm(6) for green led
#define BLU_CH7 7  // pwm(7) for blue led

LedPixelCore pixel(get_slot_addr(BRIDGE_BASE, S6_PWM), 5, 6, 7);

int main() {
   while (1) {
      uint16_t switches = sw.read();   // turn on red led w/ sw(0)
      uint8_t b = (switches & 0x03);
      uint8_t g = (switches & 0x0C);
      uint8_t r = (switches & 0x30);
      float level = (float) ((switches>>6) & 0x0f) / 15.0;

      r = r * 85;
      g = g * 85;
      b = b * 85;
      pixel.set_color(r, g, b);
      pixel.set_brightness(level);


   } //while
} //main

//Dr. Chu's test code
// #include "de10_baseline.h"
// #include "led_pixel_core.h"  // class def header file
// // create an led pixel instance
// // PWM IP core in slot #6; channels 5, 6, 7 used for tricolor LED
// LedPixelCore pixel(get_slot_addr(BRIDGE_BASE, S6_PWM), 5, 6, 7);
// int main() {
//    uint8_t i, r, g, b;
//    pixel.set_active_level(0); // x is 0 or 1 (CA or CC tricolor LED)
//    while (1) {
//       int s = sw.read();
//       // obtain color
//       i = s & 0x03;      // sw(1:0) for blue
//       b = 85*i;          // blue intensity: 0, 85, 170, 255
//       i = (s>>2) & 0x03; // sw(3:2) for green
//       g = 85*i;
//       i = (s>>4) & 0x03; // sw(5:4) for red
//       r = 85*i;
//       // obtain 4‐bit brightness level from sw(9:6)
//       float level = (float) ((s>>6) & 0x0f) / 15.0;
//       // set color and brightness
//       pixel.set_color(r, g, b);
//       pixel.set_brightness(level);
//    }
// }

