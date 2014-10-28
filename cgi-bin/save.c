#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include "mytypes.h"
#include "macros.h"
#include "inputReader.h"
#include "myutility.h"
#include "vnutil.h"
#include "strMap.h"

strMap postData;
strMap detailsMap;

void loadSlotData();

int main(){
	printf("Content-type:text/html\n\n");

	char * data = getenv("CONTENT_LENGTH");
	if(!strlen(data)){
		term("Return to the homepage");
	}  else {
		char buffer[2000];
		fgets(buffer, 2000, stdin);
		makePostMap(&postData, buffer);
		mapAdd(&detailsMap, "currentState", mapVal(&postData, "currentState"));
		mapAdd(&detailsMap, "wallet", mapVal(&postData, "wallet"));
		
		printMap(&postData);

		loadSlotData();
		char filepath[2000];
		strjoin(filepath, HTMLPATH, "/save.html", NULL);
		render(filepath, &detailsMap);


	
	}



	

	return 0;
}

void loadSlotData(){
	sqlite3 * conn;
	sqlite3_stmt * result;
	int error = 0;
	const char * tail;

	char dbpath[2000];
	strjoin(dbpath, ROOTPATH, "/", DBNAME, NULL);

	sql_open(dbpath, &conn);

	prepare(conn, "SELECT * FROM slots;", 2000, &result, &tail);
	int i = 0;
	while(sqlite3_step(result) == SQLITE_ROW){

		char tripleState[2000];
		makeTripleNum(tripleState, sqlite3_column_int(result, 0), (char *) sqlite3_column_text(result, 1), (int) sqlite3_column_int(result, 2));		

		char _state[2000] = "state";
		mapAdd(&detailsMap, strappNum(_state, i+1), tripleState);

		char _timestr[2000] = "time";
		mapAdd(&detailsMap, strappNum(_timestr, i+1), (char *)sqlite3_column_text(result, 4));

		i++;
	}

	sqlite3_finalize(result);
	sqlite3_close(conn);

}