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
	int leftDirection = 1;
	int ts0;
	int ledTime;
	while (1) {
		// read 10 switches in turn
		switches = sw.read();
		ledTime = (switches >> 4) * 10;
		if (!(switches & 0x0002)){
			if((elapsed_ms(ts0) > (ledTime)) && leftDirection) {
			ts0 = now_ms();
			led.write(0x001 << n);
			n++;
			if(n > 9) {
				leftDirection = 0;
				n = 8;
			}
			}
			else if((elapsed_ms(ts0) > (ledTime)) && !leftDirection) {
			ts0 = now_ms();
			led.write(0x001 << n);
			n--;
			if(n < 0) {
				leftDirection = 1;
				n = 1;
			}
			}
		}
		if (switches & 0x0001){
			n = 0;
			leftDirection = 1;
			led.write(0x001 << n);
		}
	}
}
