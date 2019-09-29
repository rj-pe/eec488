#include "de10_baseline.h"
#define RED_CH5 5 // pwm(5) for red led
#define BLU_CH7 7 // pwm(7) for blue led

int main() {
    while (1) {
        int r_on = sw.read(0); // turn on red led w/ sw(0)
        int b_on = sw.read(1); // turn on blue led w/ sw(1)

        for (int i=0; i <100; i++){
            // common-anode led (active-low)
            float duty = 0.01*i;
            // red fades in
            pwm.set_duty(r_on*(1.0-duty), RED_CH5);
            // blue fades out
            pwm.set_duty(b_on*duty, BLU_CH7);
            sleep_ms(10);
        }// for
        for (int i=0; i <100; i++){
            float duty = 0.01*i;
            // red fades out
            pwm.set_duty(r_on*duty, RED_CH5);
            // blue fades in
            pwm.set_duty(b_on*(1.0-duty), BLU_CH7);
            sleep_ms(10);
        } // for
    } // while
} // main
