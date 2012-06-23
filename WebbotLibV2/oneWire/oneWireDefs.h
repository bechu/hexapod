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
 *        File: oneWireDefs.h
 *  Created on: 29 Jul 2011
 *      Author: Clive Webster
 */

#ifndef ONEWIREDEFS_H_
#define ONEWIREDEFS_H_


#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

#define OVERDRIVE_SKIP 	  0x3C

 // ROM commands
#define SEARCH_ROM        0xF0
#define READ_ROM          0x33
#define MATCH_ROM         0x55
#define SKIP_ROM          0xCC
#define ALARM_SEARCH      0xEC

// Functions Commnds
#define CONVERTTEMPERATURE 0x44
#define WRITESCRATCH      0x4E
#define READSCRATCH       0xBE
#define COPYSCRATCH       0x48
#define RECALLE2          0xB8
#define READPOWERSUPPLY   0xB4



#ifdef __cplusplus
}
/* ==================== C++ Code ==============================================*/
#endif


#endif /* ONEWIREDEFS_H_ */
