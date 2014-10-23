#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include "mytypes.h"
#include "macros.h"
#include "inputReader.h"
#include "myutility.h"

pair keyMap[100];
pair postData[100];
int numPostData = 0;

void loadSlotData();

int main(){
	printf("Content-type:text/html\n\n");

	char * data = getenv("CONTENT_LENGTH");
	if(!strlen(data)){
		term("Return to the homepage");
	}  else {
		char buffer[2000];
		fgets(buffer, 2000, stdin);
		decode(buffer);

		processPostData(buffer, postData, &numPostData);
		char temp[2000];
		getKeyVal(temp, "state", postData, numPostData);
		strcpy(keyMap[NUM_SLOTS+1].key, "nextState");
		strcpy(keyMap[NUM_SLOTS+1].value, temp);
		
		
		loadSlotData();
		char filepath[2000] = {}; strcpy(filepath, HTMLPATH); strcat(filepath, "/save.html");
		FILE * fp = mopen(filepath, "r");

		readInput(fp, stdout, keyMap, 100);
		/*
			What will be passed to saveExec:
				state => state to overwrite
				nextState => state to return to
		*/
		fclose(fp);
	}



	

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
		strcpy(keyMap[i].value, itoa(i+1, itoatemp, 10)); // Formerly mapVal instaed of itoa - using usr_id.
		strcat(keyMap[i].value, ":");
		strcat(keyMap[i].value, mapVal); // Now using a triple of id, filename and d_index
		i++;
	}

	sqlite3_finalize(result);
	sqlite3_close(conn);

}