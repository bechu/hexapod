/*
 * $Id: servosCenter.c,v 1.2 2010/06/15 00:48:59 clivewebster Exp $
 * $Log: servosCenter.c,v $
 * Revision 1.2  2010/06/15 00:48:59  clivewebster
 * Add copyright license info
 *
 *
 * Copyright (C) 2010 Clive Webster (webbot@webbot.org.uk)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * servosCenter.c
 *
 *  Created on: 20 May 2010
 *      Author: Clive Webster
 */
#include "../servos.h"
#include "../uart.h"
#include "../core.h"


static uint8_t current;
static boolean ranging;

static void help(void){
	PRINTF(stdout,"\nL = List\n+ = Increment\n- = Decrement\n* = Add 10\n/ = Sub 10\nN = Next Servo\nP = Previous Servo\nC = Center mode\nR = Range mode\n");
}

static void prompt(void){
	if(ranging){
		PRINTF(stdout,"\nRange Servo %d>",current);
	}else{
		PRINTF(stdout,"\nCenter Servo %d>",current);
	}
}

static char getCh(){
	int val;
	do{
		val = fgetc(stdin);
	}while(val==-1);
	char c = val;
	if(c>='a' && c<='z'){
		c = c - 'a' + 'A';
	}
	return c;
}

static void list(const SERVO_LIST* const servos, uint8_t numServos){
	PRINTF(stdout,"\nServo Center Range\n");
	for(int i=0; i<numServos; i++){
		SERVO* servo = (SERVO*)pgm_read_word(&servos[i]);
		PRINTF(stdout,"%5d  %4d  %4d\n",i,servo->center_us,servo->range_us);
	}
}

static void update(const SERVO_LIST* const servos, int diff){
	SERVO* servo = (SERVO*)pgm_read_word(&servos[current]);
	uint16_t c = servo->center_us;
	uint16_t r = servo->range_us;

	if(ranging){
		r += diff;
	}else{
		c += diff;
	}

	servoSetConfig(servo,c,r);
}

static void setRanging( boolean r, const SERVO_LIST* const servos, uint8_t numServos){
	if(ranging != r){
		ranging = r;
		DRIVE_SPEED speed = (ranging) ? DRIVE_SPEED_MAX : DRIVE_SPEED_BRAKE;
		for(int i=0; i<numServos; i++){
			SERVO* servo = (SERVO*)pgm_read_word(&servos[i]);
			act_setSpeed(servo,speed);
		}
	}
}


void _servosCenter(const SERVO_LIST* const servos, uint8_t numServos){

	list(servos,numServos);
	help();
	prompt();
	while(1){
		char c = getCh();
		switch(c){
		case 'L':
			list(servos,numServos);
			prompt();
			break;
		case '+':
			update(servos,1);
			break;
		case '*':
			update(servos,10);
			break;
		case '-':
			update(servos,-1);
			break;
		case '/':
			update(servos,-10);
			break;
		case 'N':
			current = (current+1) % numServos;
			prompt();
			break;
		case 'P':
			current = (current==0) ? numServos-1 : current-1;
			prompt();
			break;
		case 'C':
			setRanging(FALSE, servos,numServos);
			prompt();
			break;
		case 'R':
			setRanging(TRUE, servos,numServos);
			prompt();
			break;
		default:
			help();
			prompt();
			break;
		}
	}
}

