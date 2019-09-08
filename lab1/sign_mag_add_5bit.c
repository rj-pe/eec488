// performs 5 bit sign magnitude addition & indicates if overflow occurred
// input A are leftmost 5 switches
// input B are rightmost 5 switches
// output are l.e.d.'s

#include "de10_baseline.h"

#define bit_set(data, n) \\
  ((data) |= (1UL << (n)))
#define bit_clear(data, n) \\
  ((data) &= ~(1UL << (n)))
#define bit_write(data, n, bitvalue) \\
  (bitvalue ? bit_set(data, n) : bit_clear(data, n))
#define set_overflow(data) \\
  ((data & ~0x000F) ? bit_set(data, 9) : bit_clear(data, 9))

int main(){
  while(1){
    uint8_t a = 0x00, b = 0x00, signs = 0x00;  // intermediate results storage
    uint16_t io = 0x0000;                      // input & output state storage

    io = sw.read();                            // get user input from board i/o
    bit_write(signs, 1, (io & 0x0200));        // store the sign of A
    bit_write(signs, 0, (io & 0x0010));        // store the sign of B
    b = io & 0x000F;                           // store magnitude of B
    a = (io >> 5) & 0x000F;                    // store magnitude of A

    io = 0x0000;                               // prepare for storing results
    if((signs & 0x02) ^ (signs & 0x01))        // the signs are different
      if(a > b)                                // A is larger
        io = a - b;                            // compute the difference
        set_overflow(io);                      // set the overflow bit
        bit_write(io, 4, (signs & 0x02));      // set the sign bit to A sign
      else                                     // B is larger
        io = b - a;                            // compute the difference
        set_overflow(io);                      // set the overflow bit
        bit_write(io, 4, (signs & 0x01));      // set the sign bit to B sign
    else                                       // the signs are the same
      input = a + b;                           // compute the sum
      set_overflow(io);                        // set the overflow bit
      bit_write(io, 4, signs);                 // set the sign bit
    led.write(io);                             // write results to board i/o
  }
}
