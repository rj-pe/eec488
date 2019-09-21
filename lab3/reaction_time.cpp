/*
 * Michael: turn on count, slow, react
 * RJ: ready, wait, cheat
 * Matt: main, init
 *
 * Bonus: be able to store top three reaction times keep this in mind while developing.
 * */

#include "de10_baseline.h"
#include "sseg_patterns.h" 

////////////////////////
/////// states /////////
////////////////////////

/**
 * Sets the board to ready state.
 * Seven segment display is blank.
 * Led0 is off.
 * @param &state The address of the current state variable.
 */
int ready(uint32_t* state_pntr){
  sseg.write_8ptn(CLR_PTN);
  led.write(1, 0);
  *state = 1;
}


///////////////////////
/////// control ///////
///////////////////////

/**
 * @note state variable tracks the current state.
 *    0 : initial state
 *    1 : ready state
 *    2 : wait state
 *    3 : turn on & count state
 *    4 : cheat state
 *    5 : react state
 *    6 : slow state
 */
int main(){
  uint32_t state;
}
