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

// library
#include "de10_baseline.h"


GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
MadcCore adc(get_slot_addr(BRIDGE_BASE, 5));
PwmCore pwm(get_slot_addr(BRIDGE_BASE, S6_PWM));
DebounceCore btn(get_slot_addr(BRIDGE_BASE, S7_BTN));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, 8));
DdfsCore ddfs(get_slot_addr(BRIDGE_BASE, S12_DDFS));
AdsrCore adsr(get_slot_addr(BRIDGE_BASE, S13_ADSR), &ddfs);
UartCore jtag(get_slot_addr(BRIDGE_BASE, 16));
Ws2812Core neopixel(get_slot_addr(BRIDGE_BASE, 14), 256, 1);


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

