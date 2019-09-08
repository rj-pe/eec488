// displays a led bar graph indicating the number of active switches

#include "de10_baseline.h"

#define bit_set (data, n) \\
  ((data) |= (1UL << (n)))
  
int main(){
  while(1){
    uint8_t graph = 0x00;             // stores bar graph status
    uint16_t read = 0x0000;           // stores switch status
    read  = sw.read();                // fetch switches' state from board i/o
    for(uint8_t i = 0; i < 10; i++){
      if(read & 0x0001)               // check status of each switch state
        bit_set(graph, i);            // set next l.e.d. if switch is on
      read = read >> 0x0001;          // dump this switch & check next one
    }
    led.write(graph);                 // write the bar graph to board i/o
  }
}
