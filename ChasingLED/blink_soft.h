
/*****************************************************************//**
 * @file blink_soft.h
 *
 * @brief software-based blink demo for de10 lite board
 *
 * @ note use an instanitaed "led" instance of GpoCOre class
 *
 * author: p. chu
 * revision: initial
 ********************************************************************/
#ifndef _BLINK_SOFT_H_INCLUDED
#define _BLINK_SOFT_H_INCLUDED

#include  "de10_baseline.h"


class BlinkSoft {

public:
   /* Constructor.
    * @param pos position of led (0 for rightmost)
    */
   BlinkSoft(int pos);
   BlinkSoft();
   ~BlinkSoft();                  // not used

   /**
    * set on interval of led
    * @param blinking rate in Hz
    */
   void set_rate(int rate);

   /**
    * stop blinking
    *
    */
   void stop();

   /**
    * enable blinking
    *
    */
   void go();

   /**
    * check elapsed time and toggle as needed
    *
    * @note poll_and_toggle()
    * @note check the time stamp when called; toggle led state as needed
    * @note assume that "off" period is the same as "on" period
    */
   void poll_and_toggle();
   void SetPosition(int pos);

private:
   int _i;       // led bit i
   int _state;   // on-off state of led
   int _ts;      // time stamp
   int _prd;     // "on" period of an led
   int _pause;
   int _pattern[2]; //pattern for blinking
};

#endif  // #ifndef _BLINK_SOFT_H_INCLUDED

