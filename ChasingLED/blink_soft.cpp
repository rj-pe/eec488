#include "blink_soft.h"

BlinkSoft::BlinkSoft(int pos) {
   _i = pos;
   _state = 0;
   _ts = now_ms();
   _pattern[2];
   set_rate(10);  // 10 Hz
   go();
}

BlinkSoft::BlinkSoft() {
   _state = 0;
   _ts = now_ms();
   _pattern[2];
   set_rate(10);  // 10 Hz
   go();
}

BlinkSoft::~BlinkSoft() {
}

void BlinkSoft::set_rate(int rate) {
   _prd = 1000 / rate;
}

void BlinkSoft::go() {
   _pause = 0;
}

void BlinkSoft::stop() {
   _pause = 1;
}

void BlinkSoft::SetPosition(int pos) {
	_i = pos;
}

void BlinkSoft::SetPattern(uint8_t speed) {
	_pattern[0] = ((9 - _i) * 2) * (_prd + 1);
	_pattern[1] = (_i * 2) * (_prd + 1);
}

void BlinkSoft::pause() {
	led.write( _state,_i);
}

void BlinkSoft::start() {
	led.write(1, 0);
}

void BlinkSoft::poll_and_toggle() {
   if (_pause) {
      _state = 1;
       led.write(_state, _i);
   } else {
      if ((now_ms() - _ts) > _prd * 0.5) {
         _ts = now_ms();
         _state = !_state;
         led.write(_state, _i);
      }
   }
}

