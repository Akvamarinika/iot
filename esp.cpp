#include "mbed.h"
#include <cstdio>

#define BLINKING_RATE     1000ms
#define MAXIMUM_BUFFER_SIZE 32

    DigitalOut led(PC_13);
    BufferedSerial pc(PA_9, PA_10);
    BufferedSerial esp(PA_2, PA_3);

     char buf[MAXIMUM_BUFFER_SIZE] = {0};

void dev_recv(){
        led = !led;

        while (esp.readable()) {
            printf("STM write:\r\n");
            pc.write(buf, esp.read(buf, sizeof(buf)));
        }
}

void pc_recv(){
    while (pc.readable()) {
        printf("ESP write:\r\n");
        esp.write(buf, pc.read(buf, sizeof(buf)));
    }
}

int main()
{
    pc.set_baud(115200);
    esp.set_baud(115200);

    pc.set_format(8, BufferedSerial::None, 1);
    esp.set_format(8, BufferedSerial::None, 1);


    pc.attach(&pc_recv, BufferedSerial::RxIrq);
    esp.attach(&dev_recv, BufferedSerial::RxIrq);


    while (true) {
       sleep();
       dev_recv();
       pc_recv();
    }
}

