// displays a led bar graph indicating the number of active switches
#include "de10_baseline.h"
int main(){
  while(1){
    uint16_t graph = 0x0000;          // stores bar graph status
    uint16_t read = 0x0000;           // stores switch status
    uint8_t counter = 0x00;
    read  = sw.read();                // fetch switches' state from board i/o
    for(uint8_t i = 0; i < 10; i++){
      if(read & 0x0001){               // check status of each switch state
        bit_write(graph, counter, 1);  // set next l.e.d. if switch is on
        counter++;
      }
      read = read >> 1;               // dump this switch & check next one
    }
    led.write(graph);                 // write the bar graph to board i/o
  }
}
