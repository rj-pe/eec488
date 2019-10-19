/**********************************************************************
 file: main_blink_ip_core_quick.cpp - quick test of blinking-led IP core

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

int main() {
   /***********************************************************
    *  the quick and "dirty" check of a custom I/O core
    ***********************************************************/
   // get the base address of the custom core
   // the blink core is attached to slot 4 (for user core)
   int blink_base = get_slot_addr(BRIDGE_BASE, 4);
   while (1) {
      // set up led #0 of ip core
      uint32_t prd = 1000 / 1;     // blinking rate 10 Hz
      int pause1 = sw.read(0);       // sw(0) pause blinking
      if (!pause1){
         bit_set(prd, 16);// set go bit
      	 }
      else{
      bit_set(prd, 15);
      }        // set go bit
      io_write(blink_base, 0, prd); // write i/o reg 0
      // set up led #1 of ip core
      prd = 1000 / 1;               // blinking rate 1 Hz
      int pause2 = sw.read(1);
      if (!pause2){
         bit_set(prd, 16);// set go bit
      	 }
      else{
      bit_set(prd, 15);
      }
      io_write(blink_base, 1, prd); // write i/o reg 1

      prd = 1000 / 1;               // blinking rate 1 Hz
      int pause3 = sw.read(2);
      if (!pause3){
         bit_set(prd, 16);// set go bit
      	 }
      else{
      bit_set(prd, 15);
      }          // set go bit
      io_write(blink_base, 2, prd); // write i/o reg 1

   }
 }
