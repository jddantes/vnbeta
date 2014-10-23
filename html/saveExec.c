#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

int main(){
	printf("Content-type:text/html\n\n");


	char * data = getenv("CONTENT_LENGTH");
	if(!strlen(data)){
		printf("Go back to the home page");
	} else {
			
	}

	return 0;
}