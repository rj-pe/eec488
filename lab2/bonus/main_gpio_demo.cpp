//#include "de10_baseline.h"
//int main() {
//   uint32_t pattern;
//   while (1) {
//      // read switch bit 0
//      int turn_on = sw.read(0);
//      // set led pattern
//      if (turn_on)
//         pattern = 0x00003ff;
//      else
//         pattern = 0x0000000;
//      // write led pattern
//      led.write(pattern);
//   }
//}
#include "de10_baseline.h"
#define elapsed_ms(time_stamp) (now_ms()-(time_stamp))

int main() {
	uint16_t switches = 0;
	int n = 0;
	int m = 9;
	int nLeftDirection = 1;
	int mLeftDirection = 0;
	int ledArray = 0x201;
	int ts0;
	int ts1;
	int ledTime;
	while (1) {
		// read 10 switches in turn
		switches = sw.read();
		ledTime = (switches >> 4) * 10;
		if (!(switches & 0x0002)){
			if(n+1 == m){
				nLeftDirection = !nLeftDirection;
				//n--;
			}
			if((elapsed_ms(ts0) > (ledTime)) && nLeftDirection) {
			ts0 = now_ms();
			led.write((0x001 << n)|(0x001 << m));
			n++;
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
			if(m-1 == n){
				mLeftDirection = !mLeftDirection;
				//m++;
			}
			if((elapsed_ms(ts1) > (ledTime)) && mLeftDirection) {
				ts1 = now_ms();
				led.write((0x001 << n)|(0x001 << m));
				m++;
			}
			else if((elapsed_ms(ts1) > (ledTime)) && !mLeftDirection) {
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
			ledArray = ledArray & 0xfff;
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
