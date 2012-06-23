#include "../libdefs.h"
#include <stdio.h>
#ifndef _STREAM_H
#define _STREAM_H


#ifdef __cplusplus

#include "../Sensors/_sensor_common.h"
typedef FILE STREAM;

class Stream : STREAM{

public:
	Stream(void){
		fdev_setup_stream(this, &_put, &_get, _FDEV_SETUP_RW);
	}

	// cast to a FILE*
	operator FILE *(){
		return this;
	}

	// Write out the current data from another input stream
	Stream& operator<< (Stream& src){
		return print(src);
	}

	// Dump the value of a sensor
	Stream& operator<< (Sensor& sensor ){
		return print(sensor);
	}

	Stream& operator<< (const char str[] ){
		return print(str);
	}
	Stream& operator<< (char c ){
		return print(c);
	}
	Stream& operator<< (uint8_t num ){
		return print(num);
	}
	Stream& operator<< (uint16_t num ){
		return print(num);
	}
	Stream& operator<< (uint32_t num ){
		return print(num);
	}
	Stream& operator<< (int8_t num ){
		return print(num);
	}
	Stream& operator<< (int16_t num ){
		return print(num);
	}
	Stream& operator<< (int32_t num ){
		return print(num);
	}
	Stream& operator<< (double num ){
		return print(num);
	}

	// print a string from memory, any '\n' are changed to '\r\n'
	Stream& print(const char* str);
	// Print a string from program memory, any '\n' are changed to '\r\n'
	Stream& print_P(const char* str);

	// Dump the value of a sensor
	Stream& print(Sensor& sensor ){
		sensor.dumpTo(this);
		return *this;
	}

	// Write a character
	Stream& print(char c);

	// Friendly routines to print numbers
	Stream& print(uint8_t num);
	Stream& print(uint16_t num);
	Stream& print(uint32_t num);
	Stream& print(int8_t num);
	Stream& print(int16_t num);
	Stream& print(int32_t num);

	// Print integers in a given base
	Stream& print(uint8_t num,uint8_t base);
	Stream& print(uint16_t num,uint8_t base);
	Stream& print(uint32_t num,uint8_t base);
	Stream& print(int8_t num,uint8_t base);
	Stream& print(int16_t num,uint8_t base);
	Stream& print(int32_t num,uint8_t base);

	Stream& print(double num, uint8_t decPlaces=2);
//	Stream& print(double num);

	Stream& print(Stream& src);


	// Print a newline sequence
	Stream& println(void);

	// Send a block of bytes from RAM
	size_t write(const void *src, size_t len);
	// Send a block of bytes from program memory
	size_t write_P(const void *src, size_t len);

	// All stream oriented devices must support
	// the following methods
	virtual int write(uint8_t data) = 0;
	virtual int read(void) = 0;

private:
	/* function to write one char to device
	 * Called directly from the C FILE code
	 * when using commands from stdio.h
	 * This is passed on to the write of our C++ object
	 */
	static int	_put(char c, STREAM *s){
		Stream* str = (Stream*)s;
		return str->write((uint8_t)c);
	}

	/* function to read one char from device
	 * Called directly from the C FILE code
	 * when using commands from stdio.h
	 * This is passed on to the read of our C++ object
	 */
	static int	_get(STREAM *s){
		Stream* str = (Stream*)s;
		return str->read();
	}
};

// A type of stream that just throws away output and returns EOF for input
class NullStream : public Stream{
public:
	NullStream(void) : Stream(){};
protected:
	virtual int write(uint8_t data) { return data;};
	virtual int read(void) {return EOF;};
};
extern NullStream nullStream;
#endif

#endif
