/**
* This application creates reaction timer.
* After a random time interval the user sees an l.e.d. flash on.
* The reaction time (in milliseconds) is the time elapsed from when the
* l.e.d. flashes on and when the user presses the stop button.
* After the user reacts, her reaction time is displayed.
* If the user does not react within one second "SL" is displayed.
* If the user cheats by pressing the button before the l.e.d. turns on
* "CH" is displayed.
*/

#include "de10_baseline.h"
#include "sseg_core.h"
#include "sseg_patterns.h"
#define elapsed_ms(time_stamp) (now_ms()-(time_stamp))

///////////////////////
/////// STORAGE ///////
///////////////////////

/**
* The currentState variable tracks the progress of the application.
*    0 : initial state
*    1 : ready state
*    2 : wait state
*    3 : turn on & count state
*    4 : cheat state
*    5 : react state
*    6 : slow state
*/
int currentState = 0;  
unsigned long rand = 0;       // Holds a "random" between one and six inclusive.
int ts0;                                                   // Holds a timestamp.
int record[3];                        // Holds the three fastest reaction times.

////////////////////////
/////// STATES /////////
////////////////////////

/**
* Sets the board to the initialize state.
* Clears the hex display and writes "HI".
* l.e.d. 0 is on.
*/
void init(){
  sseg.set_dp(0x00);
  led.write(0x001);                                 // Turn on rightmost l.e.d.
  sseg.write_8ptn((uint8_t*)HI_PTN);          // Writes "HI" to the hex display.
}
/**
 * Sets the board to ready state.
 * Seven segment display is blank.
 * l.e.d. 0 is off.
 */
void ready(){
  sseg.write_8ptn((uint8_t*)CLR_PTN);           // Writes "" to the hex display.
  led.write(0x001);                                  // Turn on rightmost l.e.d.
}
/**
 * Sets the board to wait state.
 * Seven segment displays zeros.
 * l.e.d. 0 is off.
 */
void wait(){
  sseg.write_8ptn((uint8_t*)ZERO_PTN);      // Writes "0000" to the hex display.
  sseg.set_dp(0x20);                               // Turn on the decimal point.
  led.write(0x000);                                    // Turn off all l.e.d.'s.
}
/**
 * Sets the board to cheat state.
 * Clears the hex display, and writes "CH".
 */
void cheat(){
  sseg.set_dp(0x00);                              // Turn off the decimal point.
  sseg.write_8ptn((uint8_t*)CHT_PTN);         // Writes "CH" to the hex display.
  led.write(0x000);                                    // Turn off all l.e.d.'s.
}
/**
* Sets the board to the slow state.
* Clears the hex display, and writes "SL".
*/
void slow()
{
  sseg.set_dp(0x00);                              // Turn off the decimal point.
  led.write(0x000);                                    // Turn off all l.e.d.'s.
  sseg.write_8ptn((uint8_t*)SL_PTN);          // Writes "SL" to the hex display.
}
/**
* Sets the board to turn on and count state.
* Displays the elapsed time since the l.e.d. light up.
* Checks whether the user reacted. 
* Routes to corresponding state based on user's reaction.
*/
void turnOnCount() {
  long ms = now_ms() - ts0;
  sseg.i2sseg((int) ms % 1000, 10, 3, 2, 1, 0);
  if (btn.read(1)) {                  // If btn(1) is pressed, the user reacted.
    ts0 =  elapsed_ms(ts0);
    rand = 0;                                      // Reset the random variable.
    currentState = 5;                                  // Go to the React state.
  else if (elapsed_ms(ts0) > 999)           // Did elapsed time exceed 0.999 ms? 
  {
    rand = 0;                                      // Reset the random variable. 
    currentState = 6;                                       // Go to slow state.
  }
}

///////////////////////
/////// CONTROL ///////
///////////////////////

int main(){
  while(1)
  {
    uint16_t switches = sw.read();                         // Read the switches.
    uint16_t reset = (switches & 0x0001);               // Is a reset requested?

    if(reset)                                            // Reset was requested. 
    {
      currentState = 0;                 // Set the state variable to Initialize.
      rand = 0;                                    // Reset the random variable.
    }
    else if(reset == 0 && currentState == 0)      // We are in Initialize state.
    {
      currentState = 1;                                  // Move to Ready state.
    }
                                          // Time to turn on the reaction l.e.d.
    if(elapsed_ms(ts0) > (rand * 1000) && rand > 0) 
    {
      led.write(0x001);                                     // Turn on l.e.d. 0.
      ts0 = now_ms();               // Record the time the l.e.d. was turned on.
      currentState = 3;             // Move to the reaction time counting state.
    }

    switch(currentState)
    {
      case 0:                                               // Initialize state.
        init();                      // Call the init function to set the board.
        break;
      case 1:                                                    // Ready state.
        ready();                    // Call the ready function to set the board.
        if(btn.read_db(0) && currentState == 1)       // Check for btn[0] press.
        {
          currentState = 2;                          // Go to the Waiting state.
        }
        break;
      case 2:                                                     // Wait state.
          wait();                    // Call the wait function to set the board.
          if(rand == 0)               // The rand variable has not yet been set.
          {
            ts0 = now_ms(); 
            rand = now_ms() % 5 + 1; // Compress timestamp to int between 1 & 6.
          }
                                          // The user has exceeded our patience.
          if(btn.read_db(1) && elapsed_ms(ts0) < (rand * 1000))
          {
            rand = 0;                              // Clear the random variable.
            currentState = 4;                    // Set state to the Slow state.
          }
          break;
      case 3:                                        // Turn on and count state.
        turnOnCount();        // Call the turnOnCount function to set the board.
        break;
      case 4:                                                    // Cheat state.
        cheat();                    // Call the cheat function to set the board.
        break;
      case 5:                                                    // React state.
        break;                                                      // Not used.   
      case 6:                                                     // Slow state.
        slow();                      // Call the slow function to set the board.
        break;
    }
  }
}


