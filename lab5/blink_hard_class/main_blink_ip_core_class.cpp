/**********************************************************************
 file: main_blink_ip_core_class.cpp - test blink ip core w/ class

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
#include "blink_hard.h"

// create an instance
BlinkHard reds(get_slot_addr(BRIDGE_BASE, S4_USER));

int main() {
   int rate, state, led_id;

   while (1) {
      // process configuration data if btn 0 pressed
      // TODO: debug blinking state.
      //       when set, several button presses needed to set led blinking
      if (btn.read_db(0)) {
         // read 10-bit switch
         int s = sw.read();
         // get led0 setting: sw(3:0): blink rate in Hz;
         //sw(4): pause
         rate = s & 0x000f;
         if (rate == 0x0){ // special case
             rate = 3200;
         }
         // sw[5:4] state of led
         // off = 0, on = 1, blinking = 2
         state = (s & 0x0030) >> 4;
         led_id = (s & 0x0300) >> 8; // sw[9:8] led id
         // configure blinking rate
         reds.set_rate(led_id, rate);
         switch(state){
           case 0:
               reds.turn_off(led_id);
               break;
           case 1:
               reds.turn_on(led_id);
               break;
           default:
               reds.blink(led_id);
         }

      }
   }
}
