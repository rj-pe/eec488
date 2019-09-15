// $DISCLAIMER$

// $Id$

/*****************************************************************//**
 * @file gpio_cores.h
 *
 * @brief Contain classes of simple i/o related cores
 *
 * Detailed description:
 * -
 *
 * $Author$
 * $Date$
 * $Revision$
 ********************************************************************/
#ifndef _GPIO_H_INCLUDED
#define _GPIO_H_INCLUDED

#include "chu_init.h"

/**********************************************************************
 * gpi (general-purpose input) core driver
 **********************************************************************/
/**
 *  gpi (general-purpose input) core driver
 *
 * MMIO subsystem HDL parameter:
 *  - W (not used in driver): # bits of input register
 *   (unused bits return 0's)
 */
class GpiCore {
   /**
    * Register map
    *
    */
   enum {
      DATA_REG = 0 /**< input data register */
   };
public:
   /**
    * Constructor.
    *
    */
   GpiCore(uint32_t core_base_addr);
   ~GpiCore();                  // not used

   /* methods */
   /**
    * read a 32-bit word
    * @return 32-bit read data word
    * @note unused bits return 0's
    */
   uint32_t read();

   /**
    * read a bit at a specific position
    *
    * @param bit_pos bit position
    * @return 1-bit read data
    *
    */
   int read(int bit_pos);

private:
   uint32_t base_addr;
};

/**********************************************************************
 * gpo (general-purpose output) core driver
 **********************************************************************/
/**
 * gpo (general-purpose output) core driver
 *
 * MMIO subsystem HDL parameter:
 *  - W (not used in driver): # bits of output register
 *   (unused bits have no effect)
 */
class GpoCore {
   /**
    * Register map
    *
    */
   enum {
      DATA_REG = 0 /**< output data register */
   };
public:
   /**
    * Constructor.
    *
    */
   GpoCore(uint32_t core_base_addr);
   ~GpoCore();                  // not used

   /**
    * write a 32-bit word
    * @param data 32-bit data
    *
    */
   void write(uint32_t data);

   /**
    * write a bit at a specific position
    *
    * @param bit_value value
    * @param bit_pos bit position
    *
    */
   void write(int bit_value, int bit_pos);

private:
   uint32_t base_addr;
   uint32_t wr_data;      //  //same as GPO core data reg
};

/**********************************************************************
 * pwm core driver
 **********************************************************************/
/**
 *   pwm (pulse-coded modulation) core driver
 *
 * MMIO subsystem HDL parameters:
 *  - R => RESOLUTION_BITS : # bits of resolution
 *  - W: number of PWM channels
 */
class PwmCore {
   /**
    * Register map
    * @note set the default pwm frequency to 1K Hz
    *
    */
   enum {
      DVSR_REG = 0,         /**< pwm divisor register */
      DUTY_REG_BASE = 0x10  /**< channel 0 duty cycle register */
   };
   /**
    * Symbolic constant
    *
    */
   enum {
      RESOLUTION_BITS = 10, /**< # resolution bits defined in HDL */
      MAX = 1 << RESOLUTION_BITS /**< # max levels in duty cycle (= 2^ESOLUTION_BITS; 100% duty cycle) */
   };
public:
   /**
    * Constructor.
    *
    */
   PwmCore(uint32_t core_base_addr);
   ~PwmCore();

   /* methods */
   /**
    * set pwm switching frequency
    *
    * @param freq pwm switching frequency
    *
    */
   void set_freq(int freq);

   /**
    * set duty cycle in unsigned format (between 0 and MAX)
    *
    * @param duty duty cycle (between 0 and MAX)
    * @param channel pwm channel number
    *
    */
   void set_duty(int duty, int channel);

   /**
    * set duty cycle in real format (between 0.0 and 1.0)
    *
    * @param f duty cycle % (between 0.0 and 1.0)
    * @param channel pwm channel number
    *
    */
   void set_duty(double f, int channel);

private:
   uint32_t base_addr;
   uint32_t freq;
};


/**********************************************************************
 * Debouce core driver
 **********************************************************************/
/**
 * debounce core driver:
 *  - retrieved data from MMIO debounce core.
 *  - play a music note
 *
 *  - W (not used in driver): # bits of input register
 *   (unused bits return 0's)
 *
 */
class DebounceCore {
   /**
    * Register map
    *
    */
   enum {
      NORMAL_DATA_REG = 0, /**< un-treated input data register */
      DB_DATA_REG = 1      /**< debounced input data register */
   };
public:
   /**
    * Constructor.
    *
    */
   DebounceCore(uint32_t core_base_addr);
   ~DebounceCore();                  // not used

   /* methods */
   /**
    * read a 32-bit un-debounced word
    * @return 32-bit read data word
    * @note same as read() of GPI core
    *
    */
   uint32_t read();

   /**
    * read an un-debounced bit at a specific position
    *
    * @param bit_pos bit position
    * @return 1-bit read data
    * @note same as GPI core
    *
    */
   int read(int bit_pos);

   /**
    * read a 32-bit debounced word
    * @return 32-bit debouncread data word
    */
   uint32_t read_db();

   /**
    * read a debounced bit at a specific position
    *
    * @param bit_pos bit position
    * @return debounced 1-bit read data
    */
   int read_db(int bit_pos);
private:
   uint32_t base_addr;
};


#endif  // _GPIO_H_INCLUDED
