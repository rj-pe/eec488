/*
 * Michael: turn on count, slow, react
 * RJ: ready, wait, cheat
 * Matt: main, init
 *
 *
 * @note state variable tracks the current state.
 *    0 : initial state
 *    1 : ready state
 *    2 : wait state
 *    3 : turn on & count state
 *    4 : cheat state
 *    5 : react state
 *    6 : slow state
 *
 *
 * Bonus: be able to store top three reaction times keep this in mind while developing.
 * */

#include "de10_baseline.h"
#include "sseg_core.h"
#include "sseg_patterns.h"
#define elapsed_ms(time_stamp) (now_ms()-(time_stamp))

int currentState = 0;
unsigned long rand = 0;
int ts0;
int record[3];

void init(){
	sseg.set_dp(0x00);
	//turn on rightmost led
	led.write(0x001);
	//writes "hi" to the seven segment display
	uint16_t switches = sw.read();
	if(switches & 0x200)
	{
		sseg.write_8ptn((uint8_t*)CLR_PTN);
		int rec = ((switches & 0x0180) >> 7);
		int pos = rec + 1;
		int num = sseg.h2s(pos);
		sseg.write_1ptn(num, 5);
		sseg.set_dp(0x08);
		sseg.i2sseg((int)record[rec] , 10, 4, 0, 1, 0);
	}
	else
	{
		sseg.write_8ptn((uint8_t*)HI_PTN);
	}
}

void ready(/*int* state_pntr*/){
  sseg.write_8ptn((uint8_t*)CLR_PTN);
  led.write(0x001);
  //update global variable
  //currentState = 1;
}

void wait(/*int* state_pntr*/){
  sseg.write_8ptn((uint8_t*)ZERO_PTN);
  sseg.set_dp(0x20);
  led.write(0x000);
  //*state_pntr = 2;
  //currentState = 2;
}

void cheat(/*int* state_pntr*/){
  sseg.set_dp(0x00);
  sseg.write_8ptn((uint8_t*)CHT_PTN);
  led.write(0x000);
  //currentState = 4;
  //*state_pntr = 4;
}

void slow()
{
	sseg.set_dp(0x00);
	led.write(0x000);
	sseg.write_8ptn((uint8_t*)SL_PTN);
}

void turnOnCount() {
    long ms = now_ms() - ts0;
    sseg.i2sseg((int) ms % 1000, 10, 3, 2, 1, 0);


    // if btn(1) = 1 go to react state
    if (btn.read(1)) {
        ts0 =  elapsed_ms(ts0);
        rand = 0;
        currentState = 5;
        int temp = 0;

        for(int i = 0; i < 3; i++) {
        	if(record[i] == 0 && ts0 > 0)
        	{
        		record[i] = ts0;
        		break;
        	}
        	else if(ts0 < record[i])
        	{
        		temp = record[i];
        		record[i] = ts0;
        		for(int j = 0; j < 2; j++)
        		{
        			record[i+1] = temp;
        		}
        		break;
        	}
        }
    }

    // if elapsed time > 0.999 ms go to slow state
    else if (elapsed_ms(ts0) > 999)
    {
    	rand = 0;
        currentState = 6;
    }
}

int main(){
	while(1)
	{

		uint16_t switches = sw.read();
		uint16_t reset = (switches & 0x0001);

		if(reset)
		{
			currentState = 0;
			rand = 0;
		}
		else if(reset == 0 && currentState == 0)
		{
			//switch state to ready
			currentState = 1;
		}

		if(elapsed_ms(ts0) > (rand * 1000) && rand > 0)
		{
			led.write(0x001);
			ts0 = now_ms();
			currentState = 3;
		}



		switch(currentState)
		{
			case 0:
				init();
				break;
			case 1:
				//ready state
				//call ready
				ready();
				//check for btn[0] press
				if(btn.read_db(0) && currentState == 1)
				{
					//go to wait state
					currentState = 2;

				}
				break;
			case 2:
				//wait state
				wait();
				if(rand == 0)
				{
					ts0 = now_ms();
					rand = now_ms() % 5 + 1;
					//currentState = 3;
				}

				if(btn.read_db(1) && elapsed_ms(ts0) < (rand * 1000))
				{
					rand = 0;
					currentState = 4;
				}

				break;
			case 3:
				//turn on and count state
				turnOnCount();
				break;
			case 4:
				//cheat state
				cheat();
				break;
			case 5:
				//react state
				break;
			case 6:
				//slow state
				slow();
				break;
		}
	}
}


