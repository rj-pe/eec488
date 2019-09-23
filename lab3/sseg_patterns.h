#ifndef _SSEG_PATTERN_H_INCLUDED
#define _SSEG_PATTERN_H_INCLUDED
// "--------" a blank screen -- ready state
const uint8_t CLR_PTN[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
// "--HI----" -- initial state
const uint8_t HI_PTN[] = {0xFF, 0xFF, 0xF9, 0x78, 0xFF, 0xFF};
// "--SL----" -- slow state
const uint8_t SL_PTN[] = {0xFF, 0xFF, 0xC7, 0x12, 0xFF, 0xFF};
// "--CH----" -- cheat state
const uint8_t CH_PTN[] = {0xFF, 0xFF, 0x09, 0xB9, 0xFF, 0xFF};
// "0000----" -- (wait state) and (turn on & wait states)
const uint8_t ZERO_PTN[] = {0xFF, 0xFF, 0x40, 0x40, 0x40, 0x40};
// "0123456789" -- index into this pattern to display a single digit
// e.g. sseg.write_1ptn(DIGIT_PTN[3], 0) displays '3' at least significant digit
const unit8_t DIGIT_PTN[] = {0x40, 0xF9, 0x24, 0x30, 0x19, 0x12, 0x02, 0xF8, 0x00, 0x18};
#endif
