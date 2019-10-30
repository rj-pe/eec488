/**********************************************************************
 file: ping_gen_quick_main.cpp - quick test of ping generator IP core
 **********************************************************************/

#include "de10_baseline.h"

int main(){
   /***********************************************************
    ** a quick and "dirty" check of a ping generator IP core **
    ***********************************************************/
   // get the base address of the custom core
   // the blink core is attached to slot 4 (for user core)
   int ping_base = get_slot_addr(BRIDGE_BASE, 4);
   while(1){
      //////////// register map//////////
      /// fccw -> register 0, bits 29 - 0
      /// dvsr -> register 1, bits 26 - 0
      /// trig -> register 2, bit 0
      ///////////////////////////////////

      uint32_t dvsr = 75000000;            ///< ping period of 1.5s

      float fc2n = 21.47483;               ///< f_c / (2^N) = 50e6 / 2^30
      int ping_freq = 10;                  ///< desired ping frequency in Hz
      uint32_t fccw = ping_freq * fc2n;
      io_write(ping_base, 0, fccw);        ///< write i/o for reg 0
      io_write(ping_base, 1, dvsr);        ///< write i/o reg 1
      io_write(ping_base, 2, sw.read(0));  ///< send trigger from switch to ping
   }
}
