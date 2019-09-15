/*****************************************************************//**
 * @file madc_core.cpp
 *
 * @brief implementation of MadcCore class
 *
 * @author p chu
 * @version v1.0: initial release
 ********************************************************************/

#include "madc_core.h"

MadcCore::MadcCore(uint32_t core_base_addr) {
	base_addr = core_base_addr;
}

MadcCore::~MadcCore() {
}

uint16_t MadcCore::read_channel_raw(int n) {
	uint16_t rd_data;

	rd_data = (uint16_t) io_read(base_addr, n) & 0x00000fff;
	return (rd_data);
}

double MadcCore::read_adc_in(int n) {
	double raw;
	double v;

	raw = (double) read_channel_raw(n);
	v = (raw / 4096.0) * 5.0;
	return (v);
}


//TODO finish this
double MadcCore::read_fpga_temp() {
	//double v, t;

	//v = read_channel_voltage(TMP_REG);
	//t = v * 503.975 - 273.15;
	return (25.0);
}


