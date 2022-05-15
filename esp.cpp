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

	if (uint32_t msg = serial_port.read(buf, sizeof(buf))) {
		printf("STM write:\r\n");
        	esp_serial.write(buf, msg);
    }
}

void pc_recv(){
    if (uint32_t msg = esp_serial.read(buf, sizeof(buf))) {
	printf("ESP write:\r\n");
        serial_port.write(buf, msg);
    }
}

int main()
{
    pc.set_baud(9600);
    esp.set_baud(9600);

    pc.set_format(8, BufferedSerial::None, 1);
    esp.set_format(8, BufferedSerial::None, 1);
    char buf[MAXIMUM_BUFFER_SIZE] = {0};


    //pc.attach(&pc_recv, BufferedSerial::RxIrq);
    //esp.attach(&dev_recv, BufferedSerial::RxIrq);


    while (true) {
       dev_recv();
       ThisThread::sleep_for(100ms);
       pc_recv();
    }
}

