#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include "mytypes.h"
#include "macros.h"
#include "inputReader.h"

pair keyMap[2000];

int main(){
	printf("Content-type:text/html\n\n");


	strcpy(keyMap[0].key, "state1");
	strcpy(keyMap[0].value, "this is state 1");

	char filepath[2000] = {};
	strcpy(filepath, HTMLPATH);
	strcat(filepath, "/load.html");

	FILE * fp = fopen(filepath, "r");
	
	if(fp == NULL){
		printf("Woops");
	} else {

		readInput(fp, stdout, keyMap, 1);
	}

	fclose(fp);

	return 0;
}