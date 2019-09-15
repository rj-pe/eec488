// $DISCLAIMER$

// $Id$

/*****************************************************************//**
 * @file timer_core.h
 *
 * @brief control and retrieve clock count from MMIO timer core
 *
 * Detailed description:
 * - writing clear bit to timer-core hardware generate a 1-clock
 *   pulse to clear the counter;
 * - ctrl variable does not keep the clear bit state
 *
 *
 * $Author$
 * $Date$
 * $Revision$
 ********************************************************************/

#ifndef _TIMER_H_INCLUDED
#define _TIMER_H_INCLUDED

#include "chu_io_rw.h"
#include "chu_io_map.h"      /* to obtain system clock rate  */

/**
 * timer core driver:
 *  - control and retrieve clock count from MMIO timer core.
 *
 */
class TimerCore {
public:
   /**
    * Register map
    *
    */
   enum {
      COUNTER_LOWER_REG = 0, /**< lower 32 bits of counter */
      COUNTER_UPPER_REG = 1, /**< upper 16 bits of counter */
      CTRL_REG = 2 /**< control register */
   };
   /**
   * Field masks
   *
   */
   enum {
      GO_FIELD = 0x00000001, /**< bit 0 of ctrl_reg; enable bit  */
      CLR_FIELD = 0x00000002 /**< bit 1 of ctrl_reg; clear bit */
   };
   /* methods */
   /**
    * Constructor.
    *
    */
   TimerCore(uint32_t core_base_addr);
   ~TimerCore();                  // not used

   /**
    * pause counter
    *
    */
   void pause();

   /**
    * enable timing counter
    *
    */
   void go();

   /**
    * clear timing counter to 0
    *
    * note: write clear bit but no effect on ctrl;
    * timer will pause/go as before
    *
    */
   void clear();

   /**
    * read current timing counter value (# clocks elapsed from last clear)
    *
    */
   uint64_t read_tick();

   /**
    * read current time (microsecond elapsed from last clear)
    *
    */
   uint64_t read_time();

   /**
    * idle (busy waiting) for us microsecond
    *
    * @param us idle time in micro second
    * @note will block the program execution
    *
    */
   void sleep(uint64_t us);

private:
   uint32_t base_addr;
   uint32_t ctrl;    // current state of control register
};

#endif  // _TIMER_H_INCLUDED
