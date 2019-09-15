/*****************************************************************//**
 * @file madc_core.h
 *
 * @brief retrieve ADC data from DE10 lite MAX on-chip ADC
 *
 * @note: 12-bit resolution
 * @note: adc input range is scaled to 0-5V on DE10 board;
 *        2^12 <=> 5V
 *
 * @author p chu
 * @version v1.0: initial release
 *********************************************************************/

#ifndef _MADC_CORE_H_INCLUDED
#define _MADC_CORE_H_INCLUDED

#include "chu_init.h"


/**
 * madc core driver:
 * - retrieve data from 6 max 10 adc channels
 */
class MadcCore {
public:
	/**
	 * constructor.
	 */
	MadcCore(uint32_t core_base_addr);
	~MadcCore(); // not used

	/**
	 * retrieve raw adc data
	 *
	 * @param n max10 adc channel
	 * @return raw 12-bit data
	 */
	uint16_t read_channel_raw(int n);

	/**
	 * retrieve analog input voltage
	 *
	 * @param n analog channel (0 - 5)
	 * @return voltage 0.0 to 5.0V (due to voltage divider)
	 */
	double read_adc_in(int n);


	/**
	 * retrieve FPGA internal temperature
	 * @return FPGA core temperature in Celsius
	 */
	double read_fpga_temp();

private:
	/* variable to keep track of current status */
	uint32_t base_addr;
}
;

#endif  // _MADC_CORE_H_INCLUDED