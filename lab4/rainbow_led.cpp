/**
* file: rainbow_led.cpp
* Generates a rainbow spectrum on an led
* connected to the DE-10 board's Arduino pins.
*/

#include "de10_baseline.h"
#include "LedPixelCore.h"
#define RED_CH5 5   ///< red channel
#define GRN_CH6 6   ///< green channel
#define BLU_CH7 7   ///< blue channel
#define MAX_CLR 255 ///< maximum color value

/**
* Checks whether the user has requested a new state.
* @param &wr the address of the user requested wrapping time.
* @param &br the address of the user requested brightness.
* @return 1 if switches have changed state, 0 if not.
*/
int switch_change(int* previous, int* wr, float* br){
  int current = sw.read();
  int rv = (*previous == current);
  if(!rv){ // a user request occurred
    // switches 0-7 control the requested wrapping time
    *wr = current & 0xff;
    // switches 9-8 control the brightness
    *br = (float) ((current >> 8) + 1) / 4.0;
    *previous = current;
  }
  return !rv;
}

/**
* Adjusts the pixel's brightness to reflect desired brightness.
* Adjusts the system time intervals to reflect desired wrapping time.
*/
void set_pixel(int* wr, float* br, int* intrvl, int* st, LedPixelCore* px){
  px->set_brightness(*br);
  // interval is (20 + wrap*20) milliseconds
  *intrvl = (20 + (*wr) * 20);
  // sleep time is (interval / ???)
  *st = ((*intrvl) / 600);
}

// instantiate an instance of the led pixel core class
int main(){
  LedPixelCore pixel(get_slot_addr(BRIDGE_BASE, S6_PWM), 5, 6, 7);
  pixel.set_active_level(0);
  int wrap;       ///< user requested wrapping time
  int interval;   ///< transformed wrapping time
  int sleep_time; ///< pause for ocular adjustment
  float bright;   ///< led's brightness
  int sw_state;   ///< the current state of the switches
  while(1){
    switch_change(&sw_state, &wrap, &bright);
    set_pixel(&wrap, &bright, &interval, &sleep_time, &pixel);
    // always start at red
    pixel.set_color(255, 0, 0);
    // each loop controls one of six stages
    // stage 1: red=max, green increasing, blue=min
    for(int i = 0; i < MAX_CLR; i++){
      pixel.set_color(i, GRN_CH6);
      if(switch_change(&sw_state, &wrap, &bright)){
        set_pixel(&wrap, &bright, &interval, &sleep_time, &pixel);
      } else {
        sleep_ms(sleep_time);
      }
    }
    // stage 2: green=max, red decreasing, blue=min
    for(int i = MAX_CLR; i > -1; i--){
      pixel.set_color(i, RED_CH5);
      if(switch_change(&sw_state, &wrap, &bright)){
        set_pixel(&wrap, &bright, &interval, &sleep_time, &pixel);
      } else {
        sleep_ms(sleep_time);
      }
    }
    // stage 3: red=min,green=max, blue increasing
    for(int i = 0; i < MAX_CLR; i++){
      pixel.set_color(i, BLU_CH7);
      if(switch_change(&sw_state, &wrap, &bright)){
        set_pixel(&wrap, &bright, &interval, &sleep_time, &pixel);
      } else {
        sleep_ms(sleep_time);
      }
    }
    // stage 4: red=min, green decreasing, blue=max
    for(int i = MAX_CLR; i > -1; i--){
      pixel.set_color(i, GRN_CH6);
      if(switch_change(&sw_state, &wrap, &bright)){
        set_pixel(&wrap, &bright, &interval, &sleep_time, &pixel);
      } else {
        sleep_ms(sleep_time);
      }
    }
    // stage 5: red increasing, green=min, blue=max
    for(int i = 0; i < MAX_CLR; i++){
      pixel.set_color(i, RED_CH5);
      if(switch_change(&sw_state, &wrap, &bright)){
        set_pixel(&wrap, &bright, &interval, &sleep_time, &pixel);
      } else {
        sleep_ms(sleep_time);
      }
    }
    // stage 6: red=max, green=min, blue decreasing
    for(int i = MAX_CLR; i > -1; i--){
      pixel.set_color(i, BLU_CH7);
      if(switch_change(&sw_state, &wrap, &bright)){
        set_pixel(&wrap, &bright, &interval, &sleep_time, &pixel);
      } else {
        sleep_ms(sleep_time);
      }
    }
  }
  return 0;
}
