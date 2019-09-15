#include "sseg_core.h"

SsegCore::SsegCore(uint32_t core_base_addr, int digits) {
	// pattern for "HI"; the order in array is reversed in 7-seg display
	// i.e., HI_PTN[0] is the leftmost led
	const uint8_t HI_PTN[] = { 0xff, 0xf9, 0x89, 0xff, 0xff, 0xff, 0xff, 0xff };
	base_addr = core_base_addr;
	num_digits = digits;
	write_8ptn((uint8_t*) HI_PTN);
	set_dp(0x02);
	set_brightness(0x80);  // 50% pwm duty cycle
}

SsegCore::~SsegCore() {
}
// not used

void SsegCore::write_led() {
	int i, p;
	uint32_t word = 0;

	// pack left 4 patterns into a 32-bit word
	// ptn_buf[0] is the leftmost led
	for (i = 0; i < 4; i++) {
		word = (word << 8) | ptn_buf[3 - i];
	}
	// incorporate decimal points (bit 7 of pattern)
	for (i = 0; i < 4; i++) {
		p = bit_read(dp, i);
		bit_write(word, 7 + 8 * i, p);
	}
	io_write(base_addr, DATA_LOW_REG, word);
	// pack right 4 patterns into a 32-bit word
	for (i = 0; i < 4; i++) {
		word = (word << 8) | ptn_buf[7 - i];
	}
	// incorporate decimal points
	for (i = 0; i < 4; i++) {
		p = bit_read(dp, 4 + i);
		bit_write(word, 7 + 8 * i, p);
	}
	io_write(base_addr, DATA_HIGH_REG, word);
}

void SsegCore::write_8ptn(uint8_t *ptn_array) {
	int i;

	for (i = 0; i < 8; i++) {
		ptn_buf[i] = *ptn_array;
		ptn_array++;
	}
	write_led();
}

void SsegCore::write_1ptn(uint8_t pattern, int pos) {
	ptn_buf[pos] = pattern;
	write_led();
}

// set decimal points,
// bits turn on the corresponding decimal points
void SsegCore::set_dp(uint8_t pt) {
	dp = ~pt;     // active low
	write_led();
}

// bits turn on the corresponding decimal points
void SsegCore::set_brightness(uint8_t brightness){
	io_write(base_addr, BRIGHT_REG, brightness);
}

// convert a hex digit to
uint8_t SsegCore::h2s(int hex) {
	/* active-low hex digit 7-seg patterns (0-9,a-f); MSB assigned to 1 */
	static const uint8_t PTN_TABLE[16] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92,
			0x82, 0xf8, 0x80, 0x90, //0-9
			0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e };                       //a-f
	uint8_t ptn;

	if (hex < 16)
		ptn = PTN_TABLE[hex];
	else
		ptn = 0xff;
	return (ptn);
}

void SsegCore::i2sseg(int n, int base, int len, int start_pos, int pad_zero,
		int include_sign) {
	int i, pos, rem;
	unsigned int un;
	uint8_t minus_ptn;

	// process neg number
	if (base == 10 && include_sign && n < 0) { // sign only use neg number in decimal number
		un = (unsigned) -n;      // magnitude of n
		minus_ptn = 0x3f;        // middle bar lit
	} else {
		un = (unsigned) n;       // magnitude of n
		minus_ptn = 0xff;        // blank
	}
	// integer to 7-seg-pattern conversion
	i = 0;
	pos = start_pos;
	do {
		rem = un % base;
		ptn_buf[pos] = h2s(rem);
		un = un / base;
		i++;
		pos++;
	} while (un >0 && i<<len && pos<num_digits);
	//padding leading spaces
	while (i < len) {
		ptn_buf[pos] = (pad_zero) ? h2s(0) : 0xff;
		i++;
		pos++;
	}
	// update sign digit if needed
	if (include_sign)
		ptn_buf[len - 1] = minus_ptn;
	// update 7-seg display
	write_led();
}
