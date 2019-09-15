// $DISCLAIMER$

// $Id$

/*****************************************************************//**
 * @file sseg_core.h
 *
 * @brief Write 7-segment LED display.
 *
 * Detailed description:
 * - An 8-element buffer (ptn_buf[]) stores the 8 7-seg patterns.
 * - dp stores the decimal point pattern
 * - the 7-seg pattern and dp combined in write_led()
 * - will work for 4-digit 7-seg display (ignoring upper 4 digits)
 * - if modified for an 8-by-8 LED matrix, dp portion should be removed
 *
 * $Author$
 * $Date$
 * $Revision$
 * revison 2:
 *   - add brightness control (if IP core constructed with PWM)
 *   - add i2sseg() method
 *   - add #digits in display; check error in i2sseg ()
 *   - update constructor
 *********************************************************************/

#ifndef _SSEG_CORE_H_INCLUDED
#define _SSEG_CORE_H_INCLUDED

#include "chu_init.h"

/**
 * seven-segment LED core driver
 *   - control 8/4-digit seven-segment LED display.
 */
class SsegCore {
public:
   /**
    * Register map
    */
   enum {
      DATA_LOW_REG = 0, /**< 32-bit data for right 4 digits */
      DATA_HIGH_REG = 1, /**<  32-bit data for left 4 digits */
      BRIGHT_REG = 2    /**<  8-bit data for pwm dutcy cycle */
   };

   /**
    * Constructor.
    *
    * @note blank 7-segment LED and then display "HI."
    * @param digits # digits in 7-seg display
    */
   SsegCore(uint32_t core_base_addr, int digits=8);
   ~SsegCore(); // not used

   /**
    * convert a hexadecimal digit to 7-seg pattern
    * @param hex a hexadecimal number (0 to 15)
    * @return 7-seg pattern w/ MSB equal to 1
    * @note return 0xff if hex exceeds 15
    */
   uint8_t h2s(int hex);

   /**
    * write one 7-seg pattern to a specific position
    * @param pattern 7-seg pattern
    * @param pos digit position (0 is least significant digit)
    */
   void write_1ptn(uint8_t pattern, int pos);

   /**
    * write 8 7-seg patterns
    * @param ptn_array pointer to an 8-element pattern array
    */
   void write_8ptn(uint8_t *ptn_array);

   /**
    * set decimal points.
    * @param pt decimal point patterns
    * @note  each bit of pt control a decimal point of a 7-seg led.
    * Decimal point turned on when the bit is 1 (active high).
    * The LSB controls digit 0 of the display.
    *
    */
   void set_dp(uint8_t pt);

   /**
    * set LED brightness
    * @param brightness 0-255
    * @note set duty cycle of IP core's pwm control
    *
    */
   void set_brightness(uint8_t brightness);

   /**
    * display a number in a a specific position
    * @param n number to be displayed
    * @param base 10 or 16 for decimal/hex number
    * @param len length (# 7-seg digits)of number
    * @param start_pos left_most digit position on 7-seg display
    * @param pad_zero 1/0 for "0" or blank padding patterns in front
    * @param include_sign 1/0 for include a sign digit (blank/- for plus/minus)
    * @note  each bit of pt control a decimal point of a 7-seg led.
    * Decimal point turned on when the bit is 1 (active high).
    * The LSB controls digit 0 of the display.
    *
    */
   void i2sseg(int n, int base, int len, int start_pos, int pad_zero, int include_sign);

private:
   /* variable to keep track of current status */
   uint32_t base_addr;
   uint8_t ptn_buf[8];    // led pattern buffer
   uint8_t dp;            // decimal point
   uint8_t num_digits;     // #digits in display
   /* methods */
   void write_led();      // write patterns to reg
}
;

#endif  // _SSEG_CORE_H_INCLUDED