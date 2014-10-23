#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include "mytypes.h"
#include "macros.h"
#include "inputReader.h"
#include "myutility.h"

pair keyMap[NUM_SLOTS];

void loadSlotData();

int main(){
	printf("Content-type:text/html\n\n");
	char filepath[2000] = {};
	strcpy(filepath, HTMLPATH);
	strcat(filepath, "/load.html");
	
	loadSlotData();

	FILE * fp = fopen(filepath, "r");
	
	if(fp == NULL){
		printf("Error opening file: %s\n", filepath);
	} else {

		readInput(fp, stdout, keyMap, NUM_SLOTS);
	}

	fclose(fp);

	return 0;
}

void loadSlotData(){
	sqlite3 * conn;
	sqlite3_stmt * result;
	int error = 0;
	const char * tail;

	char dbpath[2000];
	strcat(dbpath, ROOTPATH);
	strcat(dbpath, "/");
	strcat(dbpath, DBNAME);

	printf("%s", dbpath);

	error = sqlite3_open(dbpath, &conn);
	if(error){
		term("Could not open database");
	}


	prepare(conn, "SELECT * FROM slots;", 2000, &result, &tail);
	int i = 0;
	while(sqlite3_step(result) == SQLITE_ROW){

		char mapKey[2000] = "state";
		char itoatemp[2000] = {};
		strcat(mapKey, itoa(i+1, itoatemp, 10));
		char mapVal[2000] = {};
		strcpy(mapVal, sqlite3_column_text(result, 1));
		strcat(mapVal, ",");
		strcat(mapVal, itoa(sqlite3_column_int(result, 2), itoatemp, 10));

		strcpy(keyMap[i].key, mapKey);
		strcpy(keyMap[i].value, mapVal);
		i++;
	}

	sqlite3_finalize(result);
	sqlite3_close(conn);

}