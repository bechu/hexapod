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
 *        File: oneWireSearch.c
 *  Created on: 28 Jul 2011
 *      Author: Clive Webster
 *
 *  This code has been adapted from the Maxim website
 *      http://www.maxim-ic.com/app-notes/index.mvp/id/187
 */
#include "../oneWireBus.h"
#include <string.h>
#include "oneWireDefs.h"


uint8_t const PROGMEM oneWire_crc_table[] = {
        0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
      157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
       35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
      190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
       70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
      219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
      101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
      248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
      140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
       17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
      175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
       50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
      202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
       87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
      233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
      116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53};


static void resetSearch(ONE_WIRE_BUS* bus){
   // reset the search state
   bus->search.LastDiscrepancy = 0;
   bus->search.LastDeviceFlag = FALSE;
   bus->search.LastFamilyDiscrepancy = 0;
}

//--------------------------------------------------------------------------
// Perform the 1-Wire Search Algorithm on the 1-Wire bus using the existing
// search state.
// Return TRUE  : device found, ROM number in ROM_NO buffer
//        FALSE : device not found, end of search
//
static boolean oneWireSearch(ONE_WIRE_BUS* bus){
   int id_bit_number;
   int last_zero, rom_byte_number, search_result;
   boolean id_bit, cmp_id_bit;
   unsigned char rom_byte_mask;
   boolean search_direction;

   // initialize for search
   id_bit_number = 1;
   last_zero = 0;
   rom_byte_number = 0;
   rom_byte_mask = 1;
   search_result = 0;

   uint8_t crc8 = 0;

   // if the last call was not the last one
   if(!bus->search.LastDeviceFlag){
      // 1-Wire reset
      if(!oneWireReset(bus)){
         // reset the search
    	 resetSearch(bus);
         return FALSE;
      }

      // issue the search command
      oneWireWriteByte(bus,SEARCH_ROM);

      // loop to do the search
      do{
         // read a bit and its complement
         id_bit = oneWireReadBit(bus);
         cmp_id_bit = oneWireReadBit(bus);

         // check for no devices on 1-wire
         if( id_bit && cmp_id_bit){
        	 // Both bits are the same
             break;
         }else {
            // all devices coupled have 0 or 1
            if(id_bit != cmp_id_bit){
               search_direction = id_bit;  // bit write value for search
            }else{
               // if this discrepancy is before the Last Discrepancy
               // on a previous next then pick the same as last time
               if(id_bit_number < bus->search.LastDiscrepancy){
                  search_direction = ((bus->search.ROM_NO[rom_byte_number] & rom_byte_mask) > 0) ? TRUE : FALSE;
               }else{
                  // if equal to last pick 1, if not then pick 0
                  search_direction = (id_bit_number == bus->search.LastDiscrepancy) ? TRUE : FALSE;
               }

               // if 0 was picked then record its position in LastZero
               if(!search_direction){
                  last_zero = id_bit_number;

                  // check for Last discrepancy in family
                  if (last_zero < 9){
                     bus->search.LastFamilyDiscrepancy = last_zero;
                  }
               }
            }

            // set or clear the bit in the ROM byte rom_byte_number
            // with mask rom_byte_mask
            if(search_direction){
              bus->search.ROM_NO[rom_byte_number] |= rom_byte_mask;
            }else{
              bus->search.ROM_NO[rom_byte_number] &= ~rom_byte_mask;
            }

            // serial number search direction write bit
            oneWireWriteBit(bus,search_direction);

            // increment the byte counter id_bit_number
            // and shift the mask rom_byte_mask
            id_bit_number++;
            rom_byte_mask <<= 1;

            // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
            if(rom_byte_mask == 0){
                crc8 = oneWireCRC(crc8,bus->search.ROM_NO[rom_byte_number]);  // accumulate the CRC
                rom_byte_number++;
                rom_byte_mask = 1;
            }
         }
      }
      while(rom_byte_number < 8);  // loop until through all ROM bytes 0-7

      // if the search was successful then
      if( !( (id_bit_number < 65) || (crc8 != 0) ) ){
         // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
         bus->search.LastDiscrepancy = last_zero;

         // check for last device
         if(bus->search.LastDiscrepancy == 0){
            bus->search.LastDeviceFlag = TRUE;
         }

         search_result = TRUE;
      }
   }

   // if no device found then reset counters so next 'search' will be like a first
   if (!search_result || !bus->search.ROM_NO[0]){
	   resetSearch(bus);
	   search_result = FALSE;
   }

   return search_result;
}
//--------------------------------------------------------------------------
// Find the 'first' devices on the 1-Wire bus
// Return TRUE  : device found, ROM number in ROM_NO buffer
//        FALSE : no device present
//
boolean oneWireFirst(ONE_WIRE_BUS* bus){
	// reset the search state
	resetSearch(bus);

	return oneWireSearch(bus);
}

//--------------------------------------------------------------------------
// Find the 'next' devices on the 1-Wire bus
// Return TRUE  : device found, ROM number in ROM_NO buffer
//        FALSE : device not found, end of search
//
boolean oneWireNext(ONE_WIRE_BUS* bus){
	// leave the search state alone
	return oneWireSearch(bus);
}


//--------------------------------------------------------------------------
// Verify the device is present.
// Return TRUE  : device verified present
//        FALSE : device not present
//
boolean oneWireVerify(const ONE_WIRE_DEVICE* device){
   ONE_WIRE_BUS* bus = device->bus;
   boolean rslt = FALSE;

   if(bus){
	   int i;
	   ONE_WIRE_SEARCH_INFO backup;

	   // Backup the current state
	   memcpy(&backup, &bus->search, sizeof(ONE_WIRE_SEARCH_INFO));

	   // Copy in the device ROM code
	   memcpy(&bus->search.ROM_NO[0], &device->rom[0], 8);

	   // set search to find the same device
	   bus->search.LastDiscrepancy = 64;
	   bus->search.LastDeviceFlag = FALSE;

	   if(oneWireSearch(bus)){
	      // check if same device found
	      rslt = TRUE;
	      for(i = 0; i < 8; i++){
	         if(device->rom[i] != bus->search.ROM_NO[i]){
	            rslt = FALSE;
	            break;
	         }
	      }
	   }

	   // restore the search state
	   memcpy(&bus->search, &backup, sizeof(ONE_WIRE_SEARCH_INFO));
   }


   // return the result of the verify
   return rslt;
}

//--------------------------------------------------------------------------
// Setup the search to find the device type 'family_code' on the next call
// to OWNext() if it is present.
//
void oneWireTargetSetup(ONE_WIRE_BUS* bus,uint8_t family_code){
   int i;

   // set the search state to find SearchFamily type devices
   bus->search.ROM_NO[0] = family_code;
   for (i = 1; i < 8; i++){
      bus->search.ROM_NO[i] = 0;
   }
   bus->search.LastDiscrepancy = 64;
   bus->search.LastFamilyDiscrepancy = 0;
   bus->search.LastDeviceFlag = FALSE;
}

//--------------------------------------------------------------------------
// Setup the search to skip the current device type on the next call
// to OWNext().
//
void oneWireFamilySkipSetup(ONE_WIRE_BUS* bus){
	// set the Last discrepancy to last family discrepancy
	bus->search.LastDiscrepancy = bus->search.LastFamilyDiscrepancy;
	bus->search.LastFamilyDiscrepancy = 0;

    // check for end of list
    if(bus->search.LastDiscrepancy == 0){
    	bus->search.LastDeviceFlag = TRUE;
    }
}

/*
 *  List out all of the attached devices
 *  Return total number
 */
uint16_t oneWireListDevices(ONE_WIRE_BUS* bus){
	uint16_t count = 0;
	boolean more = oneWireFirst(bus);
	while(more){
		count++;
		oneWireDumpROM(stdout, bus);
		fputc(13, stdout);
		fputc(10, stdout);
		more = oneWireNext(bus);
	}
	return count;
}

