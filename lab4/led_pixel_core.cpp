#include "led_pixel_core.h"

LedPixelCore::LedPixelCore(uint32_t core_base_addr, int r_ch, int g_ch, int b_ch)
{
	base_addr = core_base_addr;
	r_pwm = r_ch;
	g_pwm = g_ch;
	b_pwm = b_ch;
	active = 0;
	set_brightness(1.0);
	set_color(0xff, 0xff, 0xff);
	set_freq(1000);
}
LedPixelCore::~LedPixelCore(){}

void LedPixelCore::set_freq(int freq)
{
	uint32_t dvsr;
	dvsr = (uint32_t) SYS_CLK_FREQ * 1000000 / MAX / freq;
	io_write(base_addr, DVSR_REG, dvsr);
}

//specify if the tricolor led is active low or active high.
void LedPixelCore::set_active_level(int level)
{
	this->active = level;
}

void LedPixelCore::set_color(uint8_t r, uint8_t g, uint8_t b)
{

	//active low
	if(active == 0)
	{
		r_clr = (255 - (r * brightness));
		g_clr = (255 - (g * brightness));
		b_clr = (255 - (b * brightness));
	}
	//active high
	else
	{
		r_clr = r * brightness;
		g_clr = g * brightness;
		b_clr = b * brightness;
	}

	io_write(base_addr, DUTY_REG_BASE + r_pwm, (uint16_t)(r_clr * 4));
	io_write(base_addr, DUTY_REG_BASE + g_pwm, (uint16_t)(g_clr * 4));
	io_write(base_addr, DUTY_REG_BASE + b_pwm, (uint16_t)(b_clr * 4));

}

void LedPixelCore::set_brightness(float br)
{
	this->brightness = br;
}




