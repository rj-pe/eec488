/**********************************************************************
 file: ws2812_core.h - ws2812 driver class definition

 Copyright (C) 2018 p. chu

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/
/*****************************************************************//**
 * @file ws2812_core.h
 *
 * @brief class definition of ws2812 core
 *
 * @author p chu
 * @version v1.0: initial release
 ********************************************************************/
#ifndef _WS2812_H_INCLUDED
#define _WS2812_H_INCLUDED

#include "chu_init.h"

/**
 * ws2812 buffer core driver
 *
 */
class Ws2812Core {
public:
	/**
	 * Register map
	 *
	 */
	enum {
		ADDR_REG = 0, /**< address register */
		DATA_REG = 1 /**< data register */
	};

	/* methods */
	Ws2812Core(uint32_t core_base_addr, int x, int y);
	~Ws2812Core();                  // not used

	/**
	 * control "global" brightness
	 * @param b brightness (between 0.0 to 1.0)
	 * @note ws8212 is very bright and each device consumes 3*20 mA;
	 *       the function can set the brightness of all devices
	 */
	void set_brightness(double b);


	/**
	 * write a pixel to frame buffer
	 * @param x x-coordinate of the pixel (between 0 and xmax)
	 * @param y y-coordinate of the pixel (between 0 and ymax)
	 * @param color 24-bit pixel color (rgb combined)
	 *
	 */
	void wr_pix(int x, int y, int color);

	/**
	 * write a pixel to frame buffer
	 * @param x x-coordinate of the pixel (between 0 and xmax)
	 * @param y y-coordinate of the pixel (between 0 and ymax)
	 * @param r 8-bit red
	 * @param g 8-bit green
	 * @param b 8-bit blue
	 *
	 */
	void wr_pix(int x, int y, uint8_t r, uint8_t g, uint8_t b);

	/**
	 * clear frame buffer (fill the frame with a specific color)
	 * @param color color to fill the frame
	 *
	 */
	void clr_matrix(int color);

private:
	uint32_t base_addr;
	int xmax, ymax; /**< dimension of led matrix */
	double brightness; /**< 0.0 to 1.0 */
};

#endif  // _WS2812_H_INCLUDED
