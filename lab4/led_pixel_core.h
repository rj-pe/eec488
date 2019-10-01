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
   LedPixelCore(uint32_t core_base_addr, int r_ch, int g_ch, int b_ch);
   ~LedPixelCore();
   void set_freq(int freq);
   void set_active_level(int level);
   void set_color(uint8_t r, uint8_t g, uint8_t b);
   void set_brightness(float br);
private:
   uint32_t base_addr;
   uint32_t freq;
   uint8_t r_pwm;
   uint8_t g_pwm;
   uint8_t b_pwm;
   uint8_t r_clr;
   uint8_t g_clr;
   uint8_t b_clr;
   float brightness;
   int active;
};


#endif
