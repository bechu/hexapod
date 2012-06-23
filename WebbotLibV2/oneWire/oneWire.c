/*
 * $Id: $
 * Revision History
 * ================
 * $Log: $
 * ================
 *
 * Copyright (C) 2011 Clive Webster (webbot@webbot.org.uk)
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
 *        File: oneWire.c
 *  Created on: 27 Jul 2011
 *      Author: Clive Webster
 */
#include "../oneWireBus.h"
#include "../core.h"
#include "../iopin.h"
#include <stdlib.h>
#include <string.h>
#include "oneWireDefs.h"

// Create the delay mnemonics
#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6
#define H 7
#define I 8
#define J 9
#define DELAY_MAX 10



// Timings in micro seconds see datasheet at
// http://mbed.org/users/snatch59/programs/OneWireCRC/gpdz56/docs/OneWireCRC_8cpp_source.html
//static const uint16_t PROGMEM standardTbl[DELAY_MAX] = {6, 64, 60, 10, 9, 55, 0, 480, 70, 410};
//static const uint16_t PROGMEM overdriveTbl[DELAY_MAX] = {1.5, 7.5, 7.5, 2.5, 0.75, 7, 2.5, 70, 8.5, 40};

// But we multiply them by 4 to get whole numbers
static const uint16_t PROGMEM standardTbl[DELAY_MAX] = {4*6, 4*64, 4*60, 4*10, 4*9, 4*55, 4*0, 4*480, 4*70, 4*410};
static const uint16_t PROGMEM overdriveTbl[DELAY_MAX] = {4*1.5, 4*7.5, 4*7.5, 4*2.5, 4*0.75, 4*7, 4*2.5, 4*70, 4*8.5, 4*40};


void oneWireInit(ONE_WIRE_BUS* bus){
	if(bus->delays == null){
		bus->delays = malloc(DELAY_MAX * sizeof(uint16_t));
	}
	// Set standard speed
	oneWireSetSpeed(bus, TRUE);
	oneWireReset(bus);

	// The bus is initialised
	bus->initialised = TRUE;

	// Put each device on this bus
	for(uint8_t i=0; i<bus->numDevices;i++){
		ONE_WIRE_DEVICE* device = (ONE_WIRE_DEVICE*)pgm_read_word(&bus->devices[i]);
		if(device){
			device->bus = bus;
			device->found = FALSE;
		}
	}

	// Now search the bus for all ROM values and find matching devices
	boolean more = oneWireFirst(bus);
	while(more){
		// bus->search.ROM_NO has next ROM address
		uint8_t family = bus->search.ROM_NO[0];

		// Try to find an exact match or the next available entry for the same family
		ONE_WIRE_DEVICE* nearest=NULL;
		for(uint8_t i=0; i<bus->numDevices;i++){
			ONE_WIRE_DEVICE* device = (ONE_WIRE_DEVICE*)pgm_read_word(&bus->devices[i]);
			if(device && device->found==FALSE && device->rom[0]==family){
				// Its the same family
				uint8_t exact=0;
				uint8_t wildcard=0;
				for(int j=1; j<8; j++){
					uint8_t actual = bus->search.ROM_NO[j];		// The actual ROM digit
					uint8_t expect = device->rom[j];			// The expected ROM digit

					// Set wildcard to '0' if device has no ROM address specified
					wildcard |= expect;

					// Set exact to 'not 0' if its not an exact match, or 0 if it is an exact match
					exact |= actual - expect;
				}

				// If it is an exact match then we have found it
				if(exact == 0){
					device->found = TRUE;
					goto findNext;
				}

				// If its a wildcard match then remember the first but keep searching for an exact match
				if(nearest==NULL && wildcard == 0){
					nearest = device;
				}
			}
		} // check next device

		// If we have a wildcard entry then record its ROM address
		if(nearest!=NULL){
			nearest->found = TRUE;
			memcpy(&nearest->rom[0], &bus->search.ROM_NO[0], 8);
		}else{
			// This device on the bus is nor part of the project
		}

		// Process next device found on the bus
findNext:
		more = oneWireNext(bus);
	}

	// Now double check each device exists and see if it uses parasitic power
	for(uint8_t i=0; i<bus->numDevices;i++){
		ONE_WIRE_DEVICE* device = (ONE_WIRE_DEVICE*)pgm_read_word(&bus->devices[i]);
		if(device && device->found){

			// Check to see if the device is present
			device->found = oneWireVerify(device);

			// Determine if it is parasitic
			if(device->found){
				oneWireMatchROM(device);
				oneWireWriteByte(bus,READPOWERSUPPLY);
				if(oneWireReadBit(bus)){
					// All devices are not parasitic
					device->parasitic = FALSE;
				}else{
					// Is at least one parasitic device
					device->parasitic = TRUE;
				}
			}

		}
	}
}

//-----------------------------------------------------------------------------
// Ok have checked timings are ok on 16MHz ATMega640
static void tickDelay(const ONE_WIRE_BUS* bus, int num){
	uint16_t cyclesDiv4 = bus->delays[num];
	if(cyclesDiv4){
		_delay_loop_2(cyclesDiv4);
	}
}

//-----------------------------------------------------------------------------
// Set the 1-Wire timing to 'standard' (standard=1) or 'overdrive' (standard=0).
//
void oneWireSetSpeed(ONE_WIRE_BUS* bus, boolean standard){
	// The table to copy from
	const uint16_t* src = (standard) ? standardTbl : overdriveTbl;

	// Copy the table to a local copy and convert into cycles
	for(int i=0; i <DELAY_MAX; i++){
		// uS delay
		uint16_t uS = pgm_read_word(&src[i]);

		// Convert to clock cycles
		uint32_t cycles = ((uint32_t)uS * cpu_speed_div_1000)/1000U;
		cycles /= 4;			// Remove the 4* from the original table
		cycles /= 4;			// We will call _delay_loop_2 which needs 4 cycles per loop
		bus->delays[i] = cycles;
	}
}

// Drive the bus low
static void setLow(const ONE_WIRE_BUS* bus){
	pin_make_output(bus->signal,FALSE);
}

// Drive the bus high by releasing it and allowing external pullups to work
static void setHigh(const ONE_WIRE_BUS* bus){
	pin_make_input(bus->signal,TRUE);
}

static boolean isLow(const ONE_WIRE_BUS* bus){
	return pin_is_low(bus->signal);
}

static boolean isHigh(const ONE_WIRE_BUS* bus){
	return pin_is_high(bus->signal);
}

//-----------------------------------------------------------------------------
// Generate a 1-Wire reset, return TRUE if no presence detect was found,
// return FALSE otherwise.
// (NOTE: Does not handle alarm presence from DS2404/DS1994)
//
boolean oneWireReset(const ONE_WIRE_BUS* bus){
	boolean result;

	tickDelay(bus,G);
	setLow(bus); 				// Drives DQ low
	tickDelay(bus,H);
	setHigh(bus); 				// Releases the bus
	tickDelay(bus,I);
	result = isLow(bus); 		// Sample for presence pulse from slave
	tickDelay(bus,J); 			// Complete the reset sequence recovery
	return result; 				// Return sample presence pulse result
}

//-----------------------------------------------------------------------------
// Send a 1-Wire write bit. Provide 10us recovery time.
//
void oneWireWriteBit(const ONE_WIRE_BUS* bus, boolean bit){
	if(bit){
		// Write '1' bit
		setLow(bus); 			// Drives DQ low
		tickDelay(bus,A);
		setHigh(bus); 			// Releases the bus
		tickDelay(bus,B); 		// Complete the time slot and 10us recovery
	}else{
		// Write '0' bit
		setLow(bus); 			// Drives DQ low
		tickDelay(bus,C);
		setHigh(bus); 			// Releases the bus
		tickDelay(bus,D);
	}
}

//-----------------------------------------------------------------------------
// Read a bit from the 1-Wire bus and return it. Provide 10us recovery time.
//
boolean oneWireReadBit(const ONE_WIRE_BUS* bus){
	boolean result;

	setLow(bus); 			// Drives DQ low
	tickDelay(bus,A);
	setHigh(bus); 			// Releases the bus
	tickDelay(bus,E);
	result = isHigh(bus); 	// Sample the bit value from the slave
	tickDelay(bus,F); 		// Complete the time slot and 10us recovery

	return result;
}

//-----------------------------------------------------------------------------
// Write 1-Wire data byte
//
void oneWireWriteByte(const ONE_WIRE_BUS* bus, uint8_t data){
	// Loop to write each bit in the byte, LS-bit first
	for(int loop = 0; loop < 8; loop++){
		oneWireWriteBit(bus, (data & 0x01) ? TRUE : FALSE);

		// shift the data byte for the next bit
		data >>= 1;
	}
}

//-----------------------------------------------------------------------------
// Read 1-Wire data byte and return it
//
uint8_t oneWireReadByte(const ONE_WIRE_BUS* bus){
	uint8_t result=0;
	for(int loop = 0; loop < 8; loop++){
		// shift the result to get it ready for the next bit
		result >>= 1;

		// if result is one, then set MS bit
		if(oneWireReadBit(bus)){
			result |= 0x80;
		}
	}
	return result;
}


//-----------------------------------------------------------------------------
// Set all devices on 1-Wire to overdrive speed. Return '1' if at least one
// overdrive capable device is detected.
//
boolean oneWireOverdriveSkip(ONE_WIRE_BUS* bus){
	// set the speed to 'standard'
	oneWireSetSpeed(bus, TRUE);

	// reset all devices
	if(oneWireReset(bus)){ 	// Reset the 1-Wire bus
		return FALSE; 				// Return if no devices found
	}

	// overdrive skip command
	oneWireWriteByte(bus, OVERDRIVE_SKIP);

	// set the speed to 'overdrive'
	oneWireSetSpeed(bus, FALSE);

	// do a 1-Wire reset in 'overdrive' and return presence result
	return oneWireReset(bus);
}

//-----------------------------------------------------------------------------
// Find a specific ROM Number
//

void oneWireMatchROM(const ONE_WIRE_DEVICE* dev){
	ONE_WIRE_BUS* bus = dev->bus;
	if(bus){
		oneWireReset(bus);
		oneWireWriteByte(bus,MATCH_ROM);
		for(int i=0; i<8; i++){
			oneWireWriteByte(bus,dev->rom[i]);
		}
	}
}

void oneWireApplyPower(ONE_WIRE_BUS* bus){
	pin_make_output(bus->signal,TRUE);
}
