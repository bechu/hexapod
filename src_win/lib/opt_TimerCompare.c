
// Include hardware info for this device	
#include <avr/io.h>

		
#include "lib_timerdef.h"
#include <timer.h>
#include <errors.h>
// Variable used by main library to link in this module
uint8_t PROGMEM _timer_compare_error = TIMER_COMPARE_CALLBACK_EXISTS;
// The dummy routine used to denote that the compare is in use
// Only ever called from here but not static to avoid unused warnings
void nullTimerCompareCallback(const TimerCompare *timer_compare, void* data){}

// Interrupt handler for  TIMER0_COMPAREA compare interrupt
ISR(TIMER0_COMPA_vect){
	__timer_compareService(TIMER0_COMPAREA);
}
// Interrupt handler for  TIMER0_COMPAREB compare interrupt
ISR(TIMER0_COMPB_vect){
	__timer_compareService(TIMER0_COMPAREB);
}
// Interrupt handler for  TIMER1_COMPAREA compare interrupt
ISR(TIMER1_COMPA_vect){
	__timer_compareService(TIMER1_COMPAREA);
}
// Interrupt handler for  TIMER1_COMPAREB compare interrupt
ISR(TIMER1_COMPB_vect){
	__timer_compareService(TIMER1_COMPAREB);
}
// Interrupt handler for  TIMER1_COMPAREC compare interrupt
ISR(TIMER1_COMPC_vect){
	__timer_compareService(TIMER1_COMPAREC);
}
// Interrupt handler for  TIMER2_COMPAREA compare interrupt
ISR(TIMER2_COMPA_vect){
	__timer_compareService(TIMER2_COMPAREA);
}
// Interrupt handler for  TIMER2_COMPAREB compare interrupt
ISR(TIMER2_COMPB_vect){
	__timer_compareService(TIMER2_COMPAREB);
}
// Interrupt handler for  TIMER3_COMPAREA compare interrupt
ISR(TIMER3_COMPA_vect){
	__timer_compareService(TIMER3_COMPAREA);
}
// Interrupt handler for  TIMER3_COMPAREB compare interrupt
ISR(TIMER3_COMPB_vect){
	__timer_compareService(TIMER3_COMPAREB);
}
// Interrupt handler for  TIMER3_COMPAREC compare interrupt
ISR(TIMER3_COMPC_vect){
	__timer_compareService(TIMER3_COMPAREC);
}
// TIMER4_COMPAREA is in use
static void __attribute__ ((constructor)) init_TIMER4_COMPAREA(void){
	TimerDataCompare* data = compareGetData(TIMER4_COMPAREA);
	data->compare_callback = &nullTimerCompareCallback;
}

// Interrupt handler for  TIMER4_COMPAREB compare interrupt
ISR(TIMER4_COMPB_vect){
	__timer_compareService(TIMER4_COMPAREB);
}
// Interrupt handler for  TIMER4_COMPAREC compare interrupt
ISR(TIMER4_COMPC_vect){
	__timer_compareService(TIMER4_COMPAREC);
}
// TIMER5_COMPAREA is in use
static void __attribute__ ((constructor)) init_TIMER5_COMPAREA(void){
	TimerDataCompare* data = compareGetData(TIMER5_COMPAREA);
	data->compare_callback = &nullTimerCompareCallback;
}

// TIMER5_COMPAREB is in use
static void __attribute__ ((constructor)) init_TIMER5_COMPAREB(void){
	TimerDataCompare* data = compareGetData(TIMER5_COMPAREB);
	data->compare_callback = &nullTimerCompareCallback;
}

// Interrupt handler for  TIMER5_COMPAREC compare interrupt
ISR(TIMER5_COMPC_vect){
	__timer_compareService(TIMER5_COMPAREC);
}
