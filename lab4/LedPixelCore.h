/**
 * LedPixelCore.h
 */

#ifndef LED_PIXEL_CORE_H_
#define LED_PIXEL_CORE_H_

#include "chu_init.h"

class LedPixelCore {
public:
   // register map
   enum {
      DVSR_REG = 0,
      DUTY_REG_BASE = 0x10
   };
   // symbolic constant
   enum {
      RESOLUTION_BITS = 10,
      MAX = 1 << RESOLUTION_BITS
   };
  /**
  * Constructor for the L.E.D. pixel class.
  * @param core_base_addr base address of PWM instance
  * @param r_ch PWM channel controlling the red LED
  * @param g_ch PWM channel controlling the green LED
  * @param b_ch PWM channel controlling the blue LED
  */
   LedPixelCore(uint32_t core_base_addr, int r_ch, int g_ch, int b_ch);
   ~LedPixelCore();
   /**
   * Set frequency of the L.E.D.
   * @param freq the frequency at which to set the L.E.D.
   */
   void set_freq(int freq);
   /**
   * Specify whether the tricolor LED is active-high or active-low
   * @note our lab's tricolor LED is common anode (active low)
   * @param level 0 for active-low; and non-zero for active high
   */   
   void set_active_level(int level);
   /**
   * Set the pixel color.
   * @param r 8-bit unsigned number indicating the intensity of red (0-255)
   * @param g 8-bit unsigned number indicating the intensity of green (0-255)
   * @param b 8-bit unsigned number indicating the intensity of blue (0-255)
   */   
   void set_color(uint8_t r, uint8_t g, uint8_t b);
   /**
   * Set the color of only one channel.
   * @param clr_level 8-bit unsigned number indicating the intensity of color
   * @param channel PWM channel controlling the color's LED.
   */
   void LedPixelCore::set_color(uint8_t clr_level, uint8_t channel)
   /**
   * Set brightness of the tricolor LED pixel between 0.0 & 1.0
   * @param br brightness of the pixel (0.0-1.0) 1.0 is the brightest.
   */   
   void set_brightness(float br);
private:
   uint32_t base_addr; ///< The base address of the PWM core
   uint32_t freq;      ///< The frequency of the LED 
   uint8_t r_pwm;      ///< The red channel register
   uint8_t g_pwm;      ///< The green channel register
   uint8_t b_pwm;      ///< The blue channel register
   uint8_t r_clr;      ///< The red color level
   uint8_t g_clr;      ///< The green color level
   uint8_t b_clr;      ///< The blue color level
   float brightness;   ///< The brightness of the LED
   int active;         ///< Whether the LED is CC or CA
};


#endif LEDPIXELCORE_H_
