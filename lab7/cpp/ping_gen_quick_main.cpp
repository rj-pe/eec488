/**********************************************************************
 file: ping_gen_quick_main.cpp - quick test of ping generator IP core
 **********************************************************************/

#include "de10_baseline.h"

int main(){
   /***********************************************************
    *  the quick and "dirty" check of a custom I/O core
    ***********************************************************/
   // get the base address of the custom core
   // the blink core is attached to slot 4 (for user core)
   int blink_base = get_slot_addr(BRIDGE_BASE, 4);
   while(1){
      // fccw -> register 0, bits 29 - 0
      // dvsr -> register 1, bits 26 - 0
      // trig -> register 2, bit 0
      // set up led #0 of ip core
      uint32_t dvsr = 75000000;   // ping period of 1.5s
      // TODO check that each of the values being written to the registers is correct
      /**
       * --  N: integer := 30;
       * --  SYS_FRQ: integer := 50000000;
       * --  PING_FRQ: integer := 20
       */
      float fc2n = 21.47483; // in Hz
      int ping_freq = 10; // in Hz
      uint32_t fccw = ping_freq * fc2n;
      io_write(blink_base, 0, fccw); // write i/o for reg 0
      io_write(blink_base, 1, dvsr); // write i/o reg 1
      io_write(blink_base, 2, sw.read(0));
//      io_write(blink_base, 2, 1);
//      sleep_ms(2);
//      io_write(blink_base, 2, 0);
//
//      int trig = sw.read(0);
//      if (trig){
//         io_write(blink_base, 2, trig);// set go bit
//      }
   }
}
