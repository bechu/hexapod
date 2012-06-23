/*
 * $Id: rprintf.h,v 1.11 2010/10/15 12:17:04 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: rprintf.h,v $
 * Revision 1.11  2010/10/15 12:17:04  clivewebster
 * Change rprintfStrLen to use size_t
 *
 * Revision 1.10  2010/06/15 00:48:59  clivewebster
 * Add copyright license info
 *
 * Revision 1.9  2010/06/07 18:28:41  clivewebster
 * *** empty log message ***
 *
 * Revision 1.8  2009/11/02 19:02:47  clivewebster
 * Added revision log
 *
 * ===========
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
 */
#ifndef RPRINTF_H
#define RPRINTF_H

// needed for use of PSTR below
#if !defined(_WINDOWS_)
#include <avr/pgmspace.h>
#endif
#include "libdefs.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif


// defines/constants
#define STRING_IN_RAM	0
#define STRING_IN_ROM	1

// make a putchar for those that are used to using it
//#define putchar(c)	rprintfChar(c);

extern const char PROGMEM HexChars[];
#define hexchar(x)	pgm_read_byte( HexChars+((x)&0x0f) )

// Deprecated use Writer instead
// #define RPRINTF_STATE Writer

// functions

// Initializes the rprintf library for an output stream.
// You must call this initializer once before using any other rprintf function.
// The argument must be a character stream output function.
// The previous value is returned and may be restored by call rprintfInit with the returned value
Writer rprintfInit(Writer writer);

// Make rputchar private
//extern Writer(rputchar);


static __inline__ void __rRestore(const  Writer *__s){ rprintfInit(*__s); __asm__ volatile ("" ::: "memory");}
#define OUTPUT(writer) for ( Writer rprintf_save __attribute__((__cleanup__(__rRestore))) = rprintfInit(writer), __ToDo=(Writer)1; __ToDo ; __ToDo = null )



// prints a single character to the current output device
int rprintfChar(uint8_t c);

// Repeat a character a number of times
void rprintfCharN(uint8_t c, size_t repeat);

// prints a null-terminated string stored in RAM
void rprintfStr(char str[]);

// Prints a section of a string stored in RAM.
// Begins printing at position indicated by <start>,
// and prints number of characters indicated by <len>.
void rprintfStrLen(char str[], size_t start, size_t len);

// prints a string stored in program rom
// This function does not actually store your string in
// program rom, but merely reads it assuming you stored it properly.
void rprintfProgStr(const char str[]);

// Using the function rprintfProgStrM(...) automatically causes
// your string to be stored in ROM, thereby not wasting precious RAM.
// Example usage:
// rprintfProgStrM("Hello, this string is stored in program rom");
#define rprintfProgStrM(string)			(rprintfProgStr(PSTR(string)))

// Prints a carriage-return and line-feed.
// Useful when printing to serial ports/terminals.
void rprintfCRLF(void);

// Prints the number contained in "data" in hex format
// u04,u08,u16,and u32 functions handle 4,8,16,or 32 bits respectively
void rprintfu04(uint8_t data);	///< Print 4-bit hex number. Outputs a single hex character.
void rprintfu08(uint8_t data);	///< Print 8-bit hex number. Outputs two hex characters.
void rprintfu16(uint16_t data);	///< Print 16-bit hex number. Outputs four hex characters.
void rprintfu32(uint32_t data);	///< Print 32-bit hex number. Outputs eight hex characters.

// Dump stuff out as a hex table
void rprintfMemoryDump(const void* data, size_t offset, size_t len);

// A flexible integer-number printing routine.
// Print the number "n" in the given "base", using exactly "numDigits".
//	Print +/- if signed flag "isSigned" is TRUE.
//	The character specified in "padchar" will be used to pad extra characters.
//
//	Examples:
// rprintfNum(10, 6,  TRUE, ' ',   1234);  -->  " +1234"
//	rprintfNum(10, 6, FALSE, '0',   1234);  -->  "001234"
//	rprintfNum(16, 6, FALSE, '.', 0x5AA5);  -->  "..5AA5"
void rprintfNum(char base, char numDigits, char isSigned, char padchar, long n);

// floating-point print routine
void rprintfFloat(char numDigits, double x);

// int rprintf(const char *format, ...);
// A reduced substitute for the usual C printf() function.
//
int rprintfRamRom(unsigned char stringInRom, const char *format, ...);
#if !defined(_WINDOWS_)
#define rprintfROM(format, args...)  		rprintfRamRom(STRING_IN_ROM, PSTR(format), ## args)
#define rprintfRAM(format, args...)  		rprintfRamRom(STRING_IN_RAM, format, ## args)
#define rprintf(format, args...)  			rprintfRamRom(STRING_IN_ROM, PSTR(format), ## args)
#endif


#ifdef __cplusplus
}
#endif


#endif	// RPRINTF
