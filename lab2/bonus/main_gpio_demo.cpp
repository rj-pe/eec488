/**********************************************************************
 file: chasing_leds_bonus.cpp - turns two seperate led's on and off sequentially
 
 An array of ten led's is controlled by switches.
 One led sequence lights sequentially starting from the right side moving to the left.
 The other led sequence lights sequentially starting from the left side moving to the right.
 After the leftmost led turns on, the sequence changes directions to the right.
 The six leftmost switches control the speed of the pattern. The switches
 represent a binary number n, and the speed of the pattern is n * 10 ms.
 Flipping switch 0 up initializes the process by turning on the rightmost led, 
 when switch 0 is flipped back down the chase begins. Switch 1 pauses the chase.
 
 **********************************************************************/
#include "de10_baseline.h"
#define elapsed_ms(time_stamp) (now_ms()-(time_stamp))

int main() {
	uint16_t switches = 0;
	int n = 0;
	int m = 9;
	int nLeftDirection = 1;
	int mLeftDirection = 0;
	int ts0;
	int ts1;
	int ledTime;
	while (1) {
		// read 10 switches in turn
		switches = sw.read();
		ledTime = (switches >> 4) * 10;
		if (!(switches & 0x0002)){
			if((elapsed_ms(ts0) > (ledTime)) && nLeftDirection) {
				ts0 = now_ms();
				led.write((0x001 << n)|(0x001 << m));
				n++;
				if(n+1 == m){
					nLeftDirection = !nLeftDirection;
				}
			}
			else if((elapsed_ms(ts0) > (ledTime)) && !nLeftDirection) {
				ts0 = now_ms();
				led.write((0x001 << n)|(0x001 << m));
				n--;
				if(n < 0) {
					nLeftDirection = 1;
					n = 1;
				}
			}
		}
		if (!(switches & 0x000c)){
			if((elapsed_ms(ts1) > (ledTime)) && mLeftDirection) {
				ts1 = now_ms();
				led.write((0x001 << n)|(0x001 << m));
				m++;
			}
			else if((elapsed_ms(ts1) > (ledTime)) && !mLeftDirection) {
				if(m-1 == n){
					mLeftDirection = !mLeftDirection;
				}
				ts1 = now_ms();
				led.write((0x001 << n)|(0x001 << m));
				m--;
			}
			if(m > 9) {
				mLeftDirection = 0;
				m = 8;
			}
		}
		if (switches & 0x0001){
			n = 0;
			nLeftDirection = 1;
			led.write((0x001 << n)|(0x001 << m));
		}
		if (switches & 0x0004){
			m = 9;
			mLeftDirection = 0;
			led.write((0x001 << n)|(0x001 << m));
		}
	}
}
