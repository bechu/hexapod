#include "Stream.h"

Stream& Stream::print(Stream& src){
	int c;
	while( (c = src.read()) != EOF){
		write( (uint8_t) c);
	}
	return *this;
}
