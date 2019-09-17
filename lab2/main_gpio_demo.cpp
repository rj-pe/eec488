/**********************************************************************
 file: chasing_leds.cpp - turns led's on and off sequentially
 
 An array of ten led's is controlled by switches.
 The led's light sequentially starting from the right side moving to the left.
 After the leftmost led turns on, the sequence changes directions to the right.
 The six leftmost switches control the speed of the pattern. The switches
 represent a binary number n, and the speed of the pattern is n * 10 ms.
 Flipping switch 0 up initializes the process by turning on the rightmost led, 
 when switch 0 is flipped back down the chase begins. Switch 1 pauses the chase.
 
 **********************************************************************/
#include "de10_baseline.h"
#define elapsed_ms(time_stamp) (now_ms()-(time_stamp))

int main() {
	uint16_t switches = 0;                // Storage for the board's switch state.
	int n = 0;                            // Led index counter for led array.
	int leftDirection = 1;                // Keep track of direction of sequence.
	int ts0;                              // Timestamp when last led switched.
	int ledTime;                          // Frequency of the displayed sequence.
	while (1) {
		
		switches = sw.read();               // read ten switches 
		ledTime = (switches >> 4) * 10;     // record sequence frequency from switch
		if (!(switches & 0x0002)){          // Ensure that pause switch is off      
                                        // and sequence should move to the left.
                                        // Check if it's time to advance led.
			if((elapsed_ms(ts0) > (ledTime)) && leftDirection) {
			ts0 = now_ms();                   // Record the current time.
			led.write(0x001 << n);            // Turn on only the adjacent led.
			n++;                              // Increment the led index counter.
			if(n > 9) {                       // Pattern is at end of led array.
				leftDirection = 0;              // Change sequence direction to right. 
				n = 8;                          // Hard reset of led index counter.
			}
			}
                                        // While moving to the right
                                        // repeat same procedure as above.
			else if((elapsed_ms(ts0) > (ledTime)) && !leftDirection) {
			ts0 = now_ms();
			led.write(0x001 << n);
			n--;
			if(n < 0) {                       // pattern at start of led array
				leftDirection = 1;              // change sequence to left
				n = 1;                          // hard reset of led index counter
			}
			}
		}
		if (switches & 0x0001){             // if the initialize switch is up
			n = 0;                            // hard reset of led index counter
			leftDirection = 1;                // initial sequence direction to left
			led.write(0x001 << n);            // turn on only the righmost led
		}
	}
}
