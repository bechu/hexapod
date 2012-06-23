#include "../libdefs.h"
#include "Stream.h"

Stream& Stream::print(char c){
	if(c=='\n'){
		write('\r');
	}
	write(c);
	return *this;
}
