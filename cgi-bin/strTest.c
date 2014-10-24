#include <stdio.h>
#include <string.h>
#include <stdarg.h>

char * strjoin(char * dest, ...){
	va_list vl;
	va_start(vl, dest);
	
	char * str;

	int i;
	for(i = 0; ; i++){
		str = va_arg(vl, char *);

		if(str == NULL){
			break;
		}
		i ? strcat(dest, str) : strcpy(dest, str);
	}

	va_end(vl);

	return dest;
}

char * strapp(char * dest, ...){
	va_list vl;
	va_start(vl, dest);

	char * str;

	int i;
	for(i = 0; ; i++){
		str = va_arg(vl, char *);

		if( str == NULL ){
			break;
		}

		strcat(dest, str);
	}

	va_end(vl);

	return dest;
}

int main(){
	
	char str[20000] = "";

	char asdf[] = "Trying";
	char this[] = "this";

	puts(strjoin(str, asdf, " ", this, " string", NULL));

	strapp(str, ". Then appended this", NULL);
	puts(str);

	strjoin(str, this, " is the new content now :)", NULL);
	puts(str);

	
	return 0;
}