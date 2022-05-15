#include "mbed.h"
PwmOut myled(PC_7);
 
int main() {
    float fade;
    fade=0;
    myled.period(0.01f);//10ms
       while(1) {     
        wait(0.02); // 20 ms
        myled.write(fade);
        fade=fade+0.01;
        if (fade>1) {
            fade=0;
           myled.write(fade);
            wait(1);
        }
}