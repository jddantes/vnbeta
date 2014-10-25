#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include "inputReader.h"
#include "myutility.h"
#include "strMap.h"
#include "vnutil.h"

strMap postData;
strMap detailsMap;

char usr[2000];
char nullBuffer[2000];

int main(){
	printf("Content-type:text/html\n\n");


	char * data = getenv("CONTENT_LENGTH");
	if(!strlen(data)){
		printf("Go back to the home page");
	} else {

		char buffer[2000];
		fgets(buffer, 2000, stdin);
		makePostMap(&postData, buffer);

		state_t currentState, newState; // currentState to return to, newState slot to overwrite to
		currentState = makeStateFromTriple(mapVal(&postData, "currentState"));
		newState = makeStateFromTriple(mapVal(&postData, "state"));
		strcpy(newState.scene, currentState.scene);
		strcpy(newState.d_str, currentState.d_str);

		char tripleState[2000];
		makeTriple(tripleState, newState.usr, newState.scene, atoi(newState.d_str));
		mapAdd(&detailsMap, "state", tripleState);

		sqlite3 * conn;
		sqlite3_stmt * result;
		const char * tail;
		sql_open(DBPATH, &conn);
		char sqlQuery[2000];
		strjoin(sqlQuery, "UPDATE slots SET dialogue=", newState.d_str, ", scene='", newState.scene, "' WHERE usr_id=", newState.usr, ";", NULL);
		
		printf("Sqlquery: %s", sqlQuery);
		prepare(conn, sqlQuery, 2000, &result, &tail);
		sqlite3_step(result);
		sqlite3_finalize(result);
		sqlite3_close(conn);


		char filepath[2000];
		strjoin(filepath, HTMLPATH, "/saveExec.html", NULL);
		render(filepath, &detailsMap);

	}

	return 0;
}