#include "sys/axon2.h"
#include "core.h"
#include "timer.h"
#include "uart.h"
#include "rprintf.h"
#include "servos.h"

char t;

#define USB_UART UART1
#define USB_ACTIVATE &uart1SendByte
#define USB_BAUD (BAUD_RATE)115200

#define XBEE_UART UART3
#define XBEE_ACTIVATE &uart3SendByte
#define XBEE_BAUD (BAUD_RATE)9600

SERVO right_wheel = MAKE_SERVO(TRUE, H4, 1500, 800);
SERVO left_wheel = MAKE_SERVO(TRUE, H3, 1500, 800);
//SERVO digital_example = MAKE_SERVO(FALSE, H5, 1520, 910);//digital servos have greater range

// Create the list - remember to place an & at the
// start of each servo name
SERVO_LIST servos[] = {&left_wheel, &right_wheel};

// Create a driver for the list of servos
SERVO_DRIVER bank1 = MAKE_SERVO_DRIVER(servos);
void appInitHardware(void)
{
    uartInit(USB_UART, USB_BAUD);
    uartInit(XBEE_UART, XBEE_BAUD);
    //led_off();
    servoPWMInit(&bank1);
    t = 0;
	led_put_char(8);
}

TICK_COUNT appInitSoftware(TICK_COUNT loopStart)
{

led_put_char(1);
	/*servosDisconnect(&bank1);//turn servos off

	delay_ms(1000);//button debounce delay

	servosConnect(&bank1);//turn servos on
    
	led_put_char(3);

	delay_ms(1000);//button debounce delay

	act_setSpeed(&left_wheel,0);
    
	led_put_char(2);
   */
    return 0; // dont pause after
}


TICK_COUNT appControl(LOOP_COUNT loopCount, TICK_COUNT loopStart)
{
//static int position = 0;
    //int ch = uartGetByte(XBEE_UART);
    //if(ch != -1)   
    //{ 
    //    if(ch == 'a')
    //        position += 10;
    //    if(ch == 'b')
    //        position -= 10;
    //    act_setSpeed(&left_wheel, position);
	//    //segled_on(&led_display, SEGMENT_F);
    //    uartSendByte(USB_UART, ch); 
    //}
    //else
    //    led_put_char(1);
    //position += 10;
   //  act_setSpeed(&left_wheel, position);

//	led_put_char(0);
    return 100; // wait for 1 second before calling me again. 1000000us = 1
}


