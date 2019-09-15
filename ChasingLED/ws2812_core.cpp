/**********************************************************************
 file: ws2812_core.cpp - ws2812 driver class implementation

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
 **********************************************************************/
 /*****************************************************************//**
 * @file ws2812_core.cpp
 *
 * @brief implementation of ws2812 core class
 *
 * @author p chu
 * @version v1.0: initial release
 ********************************************************************/

#include "ws2812_core.h"

Ws2812Core::Ws2812Core(uint32_t core_base_addr, int x, int y) {
	base_addr = core_base_addr;
	xmax = x;
	ymax = y;
	set_brightness(0.25);
}
Ws2812Core::~Ws2812Core() {
}
// not used

void Ws2812Core::set_brightness(double b) {
	if (b > 1.0)
		brightness = 1.0;
	else if (b < 0.0)
		brightness = 0.0;
	else
		brightness = b;
	return;
}

void Ws2812Core::wr_pix(int x, int y, int color) {
	uint8_t b = color & 0xff;
	uint8_t g = (color >> 8) & 0xff;
	uint8_t r = (color >> 16) & 0xff;
	wr_pix(x, y, r, g, b);
	return;
}

void Ws2812Core::wr_pix(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
	// calculate pixel address
	int h = (x > (xmax - 1)) ? xmax - 1 : x;
	int v = (y > (ymax - 1)) ? ymax - 1 : y;
	uint32_t addr = xmax * v + h;
	// pack rgb and adjust pixel brightness
	uint32_t ra = (uint32_t) (r * brightness);
	uint32_t ga = (uint32_t) (g * brightness);
	uint32_t ba = (uint32_t) (b * brightness);
	uint32_t color = (ra << 16) | (ga << 8) | ba;
	//uint32_t color = (r << 16) | (g << 8) | b;
	// write frame buffer
	io_write(base_addr, ADDR_REG, addr);
	io_write(base_addr, DATA_REG, color);
	return;
}

void Ws2812Core::clr_matrix(int color) {

	for (int x = 0; x < xmax; x++) {
		for (int y = 0; x < ymax; y++) {
			wr_pix(x, y, color);
		}
	}
}
