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
   int rate, pause;

   while (1) {
      // process configuration data if btn 0 pressed
      if (btn.read_db(0)) {
         // read 10-bit switch
         int s = sw.read();
         // get led0 setting: sw(3:0): blink rate in Hz; sw(4): pause
         rate = s & 0x000f;
         pause = (s & 0x0010) >> 4;
         // configure blinking led0
         reds.set_rate(0, rate);
         if (pause)
            reds.stop(0);
         else
            reds.go(0);
         // get led1 setting: sw(8:9): blink rate in Hz; sw(9): pause
         s = s>>5;
         rate = s & 0x000f;
         pause = (s & 0x0010) >> 4;
         // configure blinking led1
         reds.set_rate(1, rate);
         if (pause)
            reds.stop(1);
         else
            reds.go(1);
      }
   }
}
