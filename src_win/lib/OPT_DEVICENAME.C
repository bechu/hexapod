#include "../hardware.h"
static const char PROGMEM name_button[] = "button";
static const char PROGMEM name_uart1[] = "uart1";
static const char PROGMEM name_led_display[] = "led_display";
static const char PROGMEM name_marquee[] = "marquee";
static const char PROGMEM name_servo1[] = "servo1";
static const char PROGMEM name_servo2[] = "servo2";
static const char PROGMEM name_bank1[] = "bank1";
static const char PROGMEM unknown[] = "?";

#define NUM_DEVICES 6
static const void* PROGMEM tbl[]={
	&_button_, name_button,
	&__C_uart1, name_uart1,
	&_led_display_, name_led_display,
	&_marquee_, name_marquee,
	&_servo1_, name_servo1,
	&_servo2_, name_servo2,
	&_bank1_, name_bank1
	};

const char* getDeviceName(const void* device){
	const char* rtn = unknown;
	for(int i=0 ; i<NUM_DEVICES*2; i+=2){
		const void* addr = (const void*)pgm_read_word(&tbl[i]);
		if( addr == device){
			rtn = (const void*)pgm_read_word(&tbl[i+1]);
		}
	}
	return rtn;
}
