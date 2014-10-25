#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "inputReader.h"
#include "vnutil.h"
#include "macros.h"
#include "strMap.h"
#include "myutility.h"

strMap postData;
strMap detailsMap;

char nullArr[2000];

char * loadItems(char * dump);
char * loadOwned(char * dump);
char * getWallet(char * dump);

int main(){
	
	printf("Content-type:text/html\n\n");

	char * data = getenv("CONTENT_LENGTH");

	if(data == NULL){
		printf("<html><body>");
		printf("Go back to the homepage :)");
		printf("</body></html>");
	} else {
		char buffer[2000];
		fgets(buffer, 2000, stdin);
		makePostMap(&postData, buffer);

		
		
		mapAdd(&detailsMap, "currentState", mapVal(&postData, "currentState"));
		mapAdd(&detailsMap, "inventory", loadItems(nullArr));
		mapAdd(&detailsMap, "purchased", loadOwned(nullArr));
		mapAdd(&detailsMap, "wallet", getWallet(nullArr));

		render(strjoin(nullArr, HTMLPATH, "/store.html", NULL), &detailsMap);

	}

	return 0;
}

char * loadItems(char * dump){

	sqlite3 * conn;
	sqlite3_stmt * result;
	const char * tail;
	sql_open(strjoin(nullArr, ROOTPATH, "/", DBNAME, NULL), &conn);
	prepare(conn, "SELECT * FROM items;", 2000, &result, &tail);

	nullArr[0] = 0;
	while(sqlite3_step(result) == SQLITE_ROW){
		char itemName[2000];
		char itemPrice[50];
		strcpy(itemName, sqlite3_column_text(result, 1));
		itoa(sqlite3_column_int(result, 2), itemPrice, 10);
		strapp(dump, "<input type='radio' value='", itemName, "' name='purchase' />", itemName, ":", itemPrice, NULL);
	}
	sqlite3_finalize(result);
	sqlite3_close(conn);

	return dump;
}

char * loadOwned(char * dump){

	state_t s = makeStateFromTriple(mapVal(&postData, "currentState"));

	sqlite3 * conn;
	sqlite3_stmt * result;
	const char * tail;
	sql_open(DBPATH, &conn);
	prepare(conn, strjoin(nullArr,"SELECT items.item_name FROM items JOIN purchases on items.item_id=purchases.item_id WHERE usr_id=", s.usr,";", NULL), 2000, &result, &tail);

	nullArr[0] = 0;
	while(sqlite3_step(result) == SQLITE_ROW){
		strapp(dump, "<input type='hidden' value='", sqlite3_column_text(result, 0) ,"' />", NULL);
	}

	sqlite3_finalize(result);
	sqlite3_close(conn);

	return dump;
}

char * getWallet(char * dump){
	state_t s = makeStateFromTriple(mapVal(&postData, "currentState"));

	sqlite3 * conn;
	sqlite3_stmt * result;
	const char * tail;
	sql_open(DBPATH, &conn);
	prepare(conn, strjoin(nullArr, "SELECT money FROM temp_money;", NULL), 2000, &result, &tail);
	sqlite3_step(result);
	itoa(sqlite3_column_int(result, 0), dump, 10);
	return dump;
}

