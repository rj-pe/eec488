
/*****************************************************************//**
 * @file blink_hard.h
 *
 * @brief IP core-based blink demo for de10 lite board
 *
 * author: p. chu
 * revision: initial
 ********************************************************************/
#ifndef _BLINK_HARD_H_INCLUDED
#define _BLINK_HARD_H_INCLUDED

#include  "de10_baseline.h"

class BlinkHard {
public:
   /**
    * register map
    *
    */
   enum {
      LED0_REG = 0    /**< starting led data register */
   };

   /**
    * fields
    *
    */
   enum {
      PRD_MASK = 0x00003ff,  /**< 10-bit period field */
      GO_MASK = 0x00010000,  /**< go bit */
	  CTRL_MASK = 0x0000c000 /**< control field*/
   };
   BlinkHard(uint32_t core_base_addr);
   ~BlinkHard();                  // not used

   /**
    * set on interval of led
    * @param rate blinking rate in Hz
    * @param n position of led (0 is the rightmost)
   */
   void set_rate(int n, int rate);

   /**
    * stop blinking
    * @param n position of led (0 is the rightmost)
    */
   void turn_off(int n);

   /**
    * enable blinking
    * @param n position of led (0 is the rightmost)
    */
   void turn_on(int n);
   void blink(int n);

private:
   uint32_t base_addr;
   int prd[3];  // blinking period
   int go_bit[3];
   int ctrl_bit[3];
   /* method */
   void write_reg(int n); // pack prd/go and write i/o reg
};

#endif  // #ifndef _BLINK_HARD_H_INCLUDED

