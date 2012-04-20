#define BUILDING_LIBRARY
#include <avr/io.h>
#include <libdefs.h>
#include <timer.h>
extern const PROGMEM Timer PROGMEM pgm_Timers[];
void __initTimers(void){
 
// Set Timer4 to the following:-
//		Mode 	 = TIMER_MODE_PWM_FAST_ICR
//		Prescale = 8
	// Save the timer mode
	timerGetData(&pgm_Timers[4])->mode = TIMER_MODE_PWM_FAST_ICR;
	// Set the timer mode
	
	// Mode TIMER_MODE_PWM_FAST_ICR is 14
	// Assume current mode settings are all 0	
		
			
	if(14 & 1){
		sbi(TCCR4A,WGM40);
	} 
		
			
	if(14 & 2){
		sbi(TCCR4A,WGM41);
	} 
		
			
	if(14 & 4){
		sbi(TCCR4B,WGM42);
	} 
		
			
	if(14 & 8){
		sbi(TCCR4B,WGM43);
	} 
		
	// Top is stored in ICR register
	ICR4 = 40000;			
			
	// Turn on the timer by setting prescaler
	timerGetData(&pgm_Timers[4])->prescale_value = 8;
	TCCR4B |= 2;
	 
// Set Timer5 to the following:-
//		Mode 	 = TIMER_MODE_CTC_OCR
//		Prescale = 8
	// Save the timer mode
	timerGetData(&pgm_Timers[5])->mode = TIMER_MODE_CTC_OCR;
	// Set the timer mode
	
	// Mode TIMER_MODE_CTC_OCR is 4
	// Assume current mode settings are all 0	
		
			
	if(4 & 1){
		sbi(TCCR5A,WGM50);
	} 
		
			
	if(4 & 2){
		sbi(TCCR5A,WGM51);
	} 
		
			
	if(4 & 4){
		sbi(TCCR5B,WGM52);
	} 
		
			
	if(4 & 8){
		sbi(TCCR5B,WGM53);
	} 
		
	// Top is stored in Compare A OCR
				OCR5A = 64000;			
				
	// Turn on the timer by setting prescaler
	timerGetData(&pgm_Timers[5])->prescale_value = 8;
	TCCR5B |= 2;
	}
