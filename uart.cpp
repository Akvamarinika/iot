
#include "mbed.h"
#include "platform/mbed_thread.h"

#define MAXIMUM_BUFFER_SIZE 32

Serial pc(PA_9, PA_10); // tx, rx

int main()
{
    while(1) {      
    wait(1);
    pc.printf("Text... \n");
    }
          
   Serial_pc.set_baud(9600);
   // char buffer[MAXIMUM_BUFFER_SIZE] = {0};

   // while(1) {
   //       if (uint32_t num = serial_port.read(buffer, sizeof(buffer))) {
   //         Serial_pc.write(buf, num);
   //       }  
   // }

}


























#include "mbed.h"
#include "platform/mbed_thread.h"

#define MAXIMUM_BUFFER_SIZE 32

static BufferedSerial serial_port(PA_9, PA_10); // tx, rx

int main()
{
          
   Serial_pc.set_baud(9600);
   
   serial_port.set_format(8, BufferedSerial::None, 1);
   
   char buffer[MAXIMUM_BUFFER_SIZE] = {0};

   while(1) {
        if (uint32_t num = serial_port.read(buffer, sizeof(buffer))) {
            serial_port.write(buffer, num);
        }  
   }


    
}
