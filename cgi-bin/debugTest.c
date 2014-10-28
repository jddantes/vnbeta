#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define DEBUG 1

int debug(const char * format, ...){
	if(DEBUG){
		va_list argptr;
		va_start(argptr, format);
		printf(format, argptr);
		va_end(argptr);
		return 1;
	}
	return 0;
}

int main(){
	debug("asdf %d %d", 2, 4);

	return 0;

}