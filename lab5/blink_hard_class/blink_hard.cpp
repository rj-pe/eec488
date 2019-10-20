#include "blink_hard.h"

BlinkHard::BlinkHard(uint32_t core_base_addr) {
   base_addr = core_base_addr;
   //set_rate(10, 0);
   //go(0);
   //set_rate(20, 1);
   //go(1);
}

BlinkHard::~BlinkHard() {
}

void BlinkHard::set_rate(int n, int rate) {
   prd[n] = 1000 / rate;
   write_reg(n);
}

void BlinkHard::turn_on(int n) {
   ctrl_bit[n] = 0;
   write_reg(n);
}

void BlinkHard::turn_off(int n) {
   ctrl_bit[n] = 1;
   write_reg(n);
}

void BlinkHard::blink(int n) {
	go_bit[n] = !go_bit[n];
	write_reg(n);
}

void BlinkHard::write_reg(int n) {
   uint32_t data = prd[n] & PRD_MASK;
   if (go_bit[n])
      data = data | GO_MASK;    // set go bit to 1
   else
      data = data & ~GO_MASK;  // clear go bit to 0
   if(ctrl_bit[n]){ // turn on led
      data = data | CTRL_MASK;
   }else{
      data = data & ~CTRL_MASK;
    }
   io_write(base_addr, LED0_REG + n, data);
}
