#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include "inputReader.h"
#include "myutility.h"

pair keyMap[100];
pair postData[100];
int numPostData = 0;

char usr[2000];
char nullBuffer[2000];

int main(){
	printf("Content-type:text/html\n\n");


	char * data = getenv("CONTENT_LENGTH");
	if(!strlen(data)){
		printf("Go back to the home page");
	} else {
		char filepath[2000] = {};
		strcpy(filepath, HTMLPATH);
		strcat(filepath, "/saveExec.html");

		char buffer[2000];
		fgets(buffer, 2000, stdin);
		decode(buffer);
		processPostData(buffer, postData, &numPostData);

		char state[2000];
		char nextState[2000];
		getKeyVal(state, "state", postData, numPostData);
		getKeyVal(nextState, "nextState", postData, numPostData);
		printf("State: %s<br>", state);
		printf("nextState: %s<br>", nextState);

		char statePair[2000]; // filename/d_index pair
		/*
			keyMap["state"] now holds state to overwrite
			keyMap["nextState"] now holds state to return to
		*/
		
		split(nextState, usr, statePair, ":");
		printf("statePair:%s<br>", statePair);
		char sceneName[2000];
		char scene[2000] = SCENEPATH; strcat(scene, "/");
		char d_str[2000];
		split(statePair, sceneName, d_str, ",");
		strcat(scene, sceneName);
		int d_index = atoi(d_str);
		
		char saveUsr[2000];
		split(state, saveUsr, nullBuffer, ":");


		sqlite3 * conn;
		sqlite3_stmt * result;
		const char * tail;
		sql_open(DBPATH, &conn);
		char sqlQuery[2000] = "UPDATE slots SET dialogue="; strcat(sqlQuery, d_str);
		strcat(sqlQuery,", scene='"); strcat(sqlQuery, sceneName); 
		strcat(sqlQuery, "' WHERE usr_id="); strcat(sqlQuery, saveUsr);
		strcat(sqlQuery, ";");

		printf("Sqlquery: %s", sqlQuery);
		prepare(conn, sqlQuery, 2000, &result, &tail);
		sqlite3_step(result);
		sqlite3_finalize(result);
		sqlite3_close(conn);


		FILE * fp = mopen(filepath, "r");

		readInput(fp, stdout, postData, numPostData);

		fclose(fp);
	}

	return 0;
}