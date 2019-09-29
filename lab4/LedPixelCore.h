/**
 * LedPixelCore.h
 */

#ifndef LEDPIXELCORE_H_
#define LEDPIXELCORE_H_

class LedPixelCore {
public:
    // register map
    enum{
        DVSR_REG = 0,
        DUTY_REG_BASE = 0X10
    };
    enum{
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
     * Set frequency of the PWM
     * @param freq the frequency at which to set the PWM
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
     * Set brightness of the tricolor LED pixel between 0.0 & 1.0
     * @param br brightness of the pixel (0.0-1.0) 1.0 is the brightest.
     */
    void set_brightness(float br);

private:
    uint32_t base_addr;
    uint32_t freq;
};

#endif  LEDPIXELCORE_H_
