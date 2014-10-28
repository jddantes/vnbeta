#include <stdio.h>
#include <string.h>
#include <time.h>

char * extractTime(char * str){
	time_t rawtime = time(NULL);
	struct tm * timeinfo = localtime(&rawtime);

	strftime(str, 2000, "%Y-%m-%d %X", timeinfo);

	return str;
}


int main(){
	char buff[2000];
	printf("%s\n", extractTime(buff));
}