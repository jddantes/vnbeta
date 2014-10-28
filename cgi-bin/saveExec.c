#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include "inputReader.h"
#include "vnutil.h"

strMap postData;
strMap detailsMap;

char usr[2000];
char nullArr[2000];

void save();
void saveMoney();
void saveItems();
void flushItems(char * usr);
void saveTempItems();
void saveTime();

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
		flushItems(currentState.usr);
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
		save();

		char filepath[2000];
		strjoin(filepath, HTMLPATH, "/saveExec.html", NULL);
		render(filepath, &detailsMap);

	}

	return 0;
}

void save(){
	saveMoney();
	saveItems();
	saveTime();
}

void saveMoney(){	
	char nullArr2[2000];

	int wallet = tempWallet();
	mapUpdate(&detailsMap, "wallet", strnum(nullArr, wallet));

	state_t s = makeStateFromTriple(mapVal(&detailsMap, "state"));

	sqlite3 * conn;
	sqlite3_stmt * result;
	const char * tail;
	sql_open(DBPATH, &conn);
	prepare(conn, strjoin(nullArr, "UPDATE slots SET money=", strnum(nullArr2, wallet), " WHERE usr_id=", s.usr, ";", NULL), 2000, &result, &tail);

	sqlite3_step(result);
	sqlite3_finalize(result);
	sqlite3_close(conn);
}	

void saveItems(){
	saveTempItems();
}

void flushItems(char * usr){

	sqlite3 * conn;
	sqlite3_stmt * result;
	const char * tail;
	sql_open(DBPATH, &conn);

	prepare(conn, strjoin(nullArr, "DELETE FROM purchases WHERE usr_id=", usr, ";", NULL), 2000, &result, &tail);
	
	sqlite3_step(result);
	sqlite3_finalize(result);
	sqlite3_close(conn);
}

void saveTempItems(){
	char nullArr2[2000];
	state_t s = makeStateFromTriple(mapVal(&detailsMap, "state"));

	sqlite3 * conn;
	sqlite3_stmt * result;
	const char * tail;
	sql_open(DBPATH, &conn);

	prepare(conn, strjoin(nullArr, "SELECT item_id FROM temp_purc;", NULL), 2000, &result, &tail);
	while(sqlite3_step(result) == SQLITE_ROW){

		sqlite3_stmt * result2;
		const char * tail2;

		strnum(nullArr2, sqlite3_column_int(result, 0));
		prepare(conn, strjoin(nullArr, "INSERT INTO purchases VALUES(",s.usr,",", nullArr2,");", NULL), 2000, &result2, &tail2);
		sqlite3_step(result2);
		sqlite3_finalize(result2);
	}
	sqlite3_finalize(result);
	sqlite3_close(conn);
}

void saveTime(){
	char nullArr[2000];
	char nullArr2[2000];

	state_t s = makeStateFromTriple(mapVal(&detailsMap, "state"));

	sqlite3 * conn;
	sqlite3_stmt * result;
	const char * tail;
	sql_open(DBPATH, &conn);

	prepare(conn, strjoin(nullArr, "UPDATE slots SET timestr='", getCurrentTime(nullArr2), "' WHERE usr_id=", s.usr, ";", NULL), 2000, &result, &tail);
	sqlite3_step(result);
	sqlite3_finalize(result);

	sqlite3_close(conn);
}