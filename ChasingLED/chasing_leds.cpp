/**********************************************************************
 file: main_blink_2led_class.cpp - blinking 2 leds with a blink class

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
 **********************************************************************/

#include "de10_baseline.h"
#include "blink_soft.h"
#define elapsed_ms(time_stamp) (now_ms()-(time_stamp))


int main() {
   // create 2 instances for 2 rightmost red leds
   BlinkSoft leds[9];
   int rate, pause;
   for(int i = 0; i < 10; i++)
   {
	   leds[i].SetPosition(i);
   }

   while (1) {
      // process configuration data if btn 0 pressed
	  uint16_t input = sw.read();
	  uint8_t speed = (input & 0x03F0);
	  uint4_t start = (input & 0x0001);
	  uint4_t pause = ((input & 0x0002) >> 1);

	  if(pause) {
		  for(int i = 0; i < 10; i++){
			  leds[i].pause();
		  }
	  } else {
		  for(int i = 0; i < 10; i++){
			  leds[i].go();
		  }
	  }

	  if(start) {
		  for(int i = 1; i < 9; i++){
			  led.write(0, i);
		  }
	  led.write(1, 0);

	  }
   }
}
