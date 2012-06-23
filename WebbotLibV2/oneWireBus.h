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
 *        File: oneWireBus.h
 *  Created on: 26 Jul 2011
 *      Author: Clive Webster
 *
 *  Implement a Dallas (now Maxim) 1 Wire Bus
 *  Code is based on examples at: http://www.maxim-ic.com/app-notes/index.mvp/id/126
 *
 *  The 1 wire bus is a bit like I2C in that it works at any voltage ie:
 *  To set a 0: change pin to output and set to 0
 *  To set a 1: change pin to input and allow pull-up resistor to set the voltage
 *
 *  A single master can control many slave devices. Each slave device has a 64 bit unique address.
 *  This slave address is set during manufacture but starts with a 'family code identifying what
 *  type of device it is: eg Thermometer, EEPROM etc
 *
 *  All slave devices must share a common Ground with the master
 *  All slave devices must connect to the common signal line
 *  The slave devices can either be parasitic (ie they are powered from the signal line) or may have
 *  	a third wire to power them. The parasitic option requires one less pin but can mean slower
 *  	data transmission speeds as the signal line must be slowed down to make sure that they continue
 *  	to receive power.
 *
 *  The speed of the bus can either be 'standard' or 'overdrive'
 */

#ifndef ONE_WIRE_H_
#define ONE_WIRE_H_

#include "iopin.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

// Forward def
struct s_1wire_bus;

// device ids
#define DS18B20_ID        0x28
#define DS18S20_ID        0x10


// Forward definition
struct c_1wire_device;

/*-------------------------------------------------------------
   Each device should contain a structure of this type
   and it should be called 'oneWire'
-------------------------------------------------------------*/
typedef struct s_1wire_device {
	struct s_1wire_bus* bus;		// The bus it is connected to
	boolean		found;				// Has it been found yet
	uint8_t		rom[8];				// The unique address of the device on the bus
	boolean		parasitic;			// Is it parasitic for power
} ONE_WIRE_DEVICE;

#define MAKE_ONE_WIRE_DEVICE(family,a,b,c,d,e,f,crc) { \
		null,FALSE,{family,a,b,c,d,e,f,crc}, TRUE \
}


// A list of ONE_WIRE_DEVICEs
typedef ONE_WIRE_DEVICE* /*PROGMEM*/  ONE_WIRE_DEVICE_LIST;

typedef struct s_1wire_search{
	int							LastDiscrepancy;
	boolean						LastDeviceFlag;
	int							LastFamilyDiscrepancy;
	uint8_t						ROM_NO[8];
} ONE_WIRE_SEARCH_INFO;


/*-------------------------------------------------------------
   Define a 1 Wire Bus
-------------------------------------------------------------*/
typedef struct s_1wire_bus{
	const ONE_WIRE_DEVICE_LIST* const devices;			// The list of devices on the bus
	const uint8_t				numDevices;			// The number of devices
	boolean						initialised;		// Has the bus been initialised
	const IOPin* 				signal;				// The pin used for the bus signal
	uint16_t*					delays;				// The delay table

	// Search info
	ONE_WIRE_SEARCH_INFO		search;
} ONE_WIRE_BUS;

#define MAKE_ONE_WIRE_BUS(signal, devices) { \
	devices, \
	(uint8_t)(sizeof(devices)/sizeof(ONE_WIRE_DEVICE*)),\
	FALSE,	\
	signal,	\
	null,	\
	{0, FALSE, 0} 				/* same as search reset */ \
}

void oneWireInit(ONE_WIRE_BUS* bus);
boolean oneWireReadBit(const ONE_WIRE_BUS* bus);
void oneWireWriteBit(const ONE_WIRE_BUS* bus, boolean bit);
void oneWireWriteByte(const ONE_WIRE_BUS* bus, uint8_t data);
uint8_t oneWireReadByte(const ONE_WIRE_BUS* bus);

void oneWireSetSpeed(ONE_WIRE_BUS* bus, boolean standard);
boolean oneWireReset(const ONE_WIRE_BUS* bus);
uint8_t oneWireTouchByte(const ONE_WIRE_BUS* bus,uint8_t data);
void oneWireTouchBlock(const ONE_WIRE_BUS* bus, uint8_t *data, size_t data_len);
boolean oneWireOverdriveSkip(ONE_WIRE_BUS* bus);

void oneWireDumpROM(FILE* f,const ONE_WIRE_BUS* bus);
boolean oneWireFirst(ONE_WIRE_BUS* bus);
boolean oneWireNext(ONE_WIRE_BUS* bus);

boolean oneWireVerify(const ONE_WIRE_DEVICE* device);
void oneWireMatchROM(const ONE_WIRE_DEVICE* dev);
void oneWireDeviceDumpROM(FILE* f,const ONE_WIRE_DEVICE* device);

// List all ROM codes and return total number of devices
uint16_t oneWireListDevices(ONE_WIRE_BUS* bus);

// Apply power for parasitic devices
void oneWireApplyPower(ONE_WIRE_BUS* bus);

extern const uint8_t PROGMEM oneWire_crc_table[];
static __inline__ uint8_t oneWireCRC(uint8_t crc, uint8_t value){
   return pgm_read_word(&oneWire_crc_table[crc ^ value]);
}

#ifdef __cplusplus
}
/* ==================== C++ Code ==============================================*/
class OneWireBus {
public:
	OneWireBus(ONE_WIRE_BUS* cstr){
		m_cstr = cstr;
	}

	/*
	 * Turn on power in parasite mode
	 */
	void applyPower(void) const {
		oneWireApplyPower(m_cstr);
	}

	// Read a bit from the bus
	boolean readBit(void) const{
		return oneWireReadBit(m_cstr);
	}

	void writeBit(boolean bit) const{
		oneWireWriteBit(m_cstr,bit);
	}

	void setStandardSpeed(void) const{
		oneWireSetSpeed(m_cstr, TRUE);
	}

	void setOverdriveSpeed(void) const{
		oneWireSetSpeed(m_cstr, FALSE);
	}

	boolean reset(void) const{
		return oneWireReset(m_cstr);
	}

	// Write a byte
	void write(uint8_t val) const {
		oneWireWriteByte(m_cstr, val);
	}

	// Read a byte
	uint8_t read(void) const{
		return oneWireReadByte(m_cstr);
	}

	// List all ROM codes and return total number of devices
	uint16_t list(void) const {
		return oneWireListDevices(m_cstr);
	}

	uint8_t touchByte(uint8_t data) const{
		return oneWireTouchByte(m_cstr,data);
	}

	void touchBlock(uint8_t data[], size_t data_len){
		oneWireTouchBlock(m_cstr, data, data_len);
	}

	boolean overdriveSkip(void) const{
		return oneWireOverdriveSkip(m_cstr);
	}

	// Dump ROM code from last call to first/next
	void dumpROM(FILE* f) const{
		oneWireDumpROM(f,m_cstr);
	}

	boolean first(void) const{
		return oneWireFirst(m_cstr);
	}

	boolean next(void) const {
		return oneWireNext(m_cstr);
	}

	const uint8_t* getROM(void) const{
		return &m_cstr->search.ROM_NO[0];
	}



private:
	ONE_WIRE_BUS* m_cstr;

};

class OneWireDevice{
public:
	OneWireDevice(ONE_WIRE_DEVICE* cstr){
		m_cstr = cstr;
	}

	boolean exists(void) const{
		return oneWireVerify(m_cstr);
	}

	boolean found(void) const{
		return m_cstr->found;
	}

	void dumpROM(FILE* f) const{
		oneWireDeviceDumpROM(f,m_cstr);
	}

protected:
	void select(void) const{
		oneWireMatchROM(m_cstr);
	}

private:
	ONE_WIRE_DEVICE* m_cstr;
};
#endif


#endif /* ONE_WIRE_H_ */
