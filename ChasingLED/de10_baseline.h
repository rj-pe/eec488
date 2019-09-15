/*****************************************************************//**
 * @file de10_baseline.h
 *
 * @brief Declare io cores instances for a de10 lite baseline configuration.
 *
 * Detail description:
 *
 *********************************************************************/

/**********************************************************************
 * basic uart and timer functions
 *  - obtain BRIDGE_BASE from chu_io_map.h

 *********************************************************************/

#ifndef _DE10_BASELINE_H_INCLUDED
#define _DE10_BASELINE_H_INCLUDED

// library
#include "chu_init.h"
#include "gpio_cores.h"
#include "madc_core.h"
#include "sseg_core.h"
#include "ddfs_core.h"
#include "adsr_core.h"
#include "ws2812_core.h"

/*
#include "i2c_core.h"
#include "spi_core.h"
//#include "vga_core.h"

#include "adxl345.h"
*/

//  make baseline core instances visible by other code
extern GpoCore led;
extern GpiCore sw;
extern DebounceCore btn;
extern MadcCore adc;
extern PwmCore pwm;
extern SsegCore sseg;
extern DdfsCore ddfs;
extern AdsrCore adsr;
extern UartCore jtag;
extern Ws2812Core neopixel;

/*
extern SpiCore tft_spi(get_slot_addr(BRIDGE_BASE, 4));
extern I2cCore i2c(get_slot_addr(BRIDGE_BASE, S10_I2C));
extern SpiCore acc_spi(get_slot_addr(BRIDGE_BASE, S9_SPI));
// external device instance
extern Adxl345 g_sensor(&acc_spi, 0);
*/

/* // video instances
extern GpvCore bar(get_sprite_addr(BRIDGE_BASE, V7_BAR));
extern GpvCore gray(get_sprite_addr(BRIDGE_BASE, V6_GRAY));
extern SpriteCore ghost(get_sprite_addr(BRIDGE_BASE, V3_GHOST), 1024);
extern SpriteCore mouse(get_sprite_addr(BRIDGE_BASE, V1_MOUSE), 1024);
extern OsdCore osd(get_sprite_addr(BRIDGE_BASE, V2_OSD));
*/

#endif  // __DE10_BASELINE_H_INCLUDED
