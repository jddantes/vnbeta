#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "inputReader.h"
#include "vnutil.h"

strMap postData;
strMap detailsMap;
strMap ownedMap;

char nullArr[2000];
char nullArr2[2000];

void makePurchase(char * itemName);
void loadOwned();
int hasItem(char * itemName);
void buy(char * itemName);

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

		makePurchase(mapVal(&postData, "purchase"));

		mapAdd(&detailsMap, "currentState", mapVal(&postData, "currentState"));
		mapAdd(&detailsMap, "wallet", mapVal(&postData, "wallet"));

		render(strjoin(nullArr, HTMLPATH, "/storeExec.html", NULL), &detailsMap);

	}

	return 0;
}

void makePurchase(char * itemName){
	loadOwned();
	
	if(hasItem(itemName)){
		mapUpdate(&detailsMap, "purchaseStatus", "You already have this item!");
		return;
	}
	if(getItemPrice(itemName) > tempWallet()  ){
		mapUpdate(&detailsMap, "purchaseStatus", "Too expensive!");
		return;
	}

	buy(itemName);

	mapUpdate(&detailsMap, "purchaseStatus", "Got the item!");
}

void loadOwned(){

	sqlite3 * conn;
	sqlite3_stmt * result;
	const char * tail;
	sql_open(DBPATH, &conn);
	prepare(conn, strjoin(nullArr, "SELECT item_id from temp_purc;", NULL), 2000, &result, &tail);

	while(sqlite3_step(result) == SQLITE_ROW){
		mapAdd(&ownedMap, getItemName(sqlite3_column_int(result, 0), nullArr) , "");
	}

	sqlite3_finalize(result);
	sqlite3_close(conn);
}

int hasItem(char * itemName){
	if(mapFind(&ownedMap, itemName) != NULL){
		return 1;
	} else {
		return 0;
	}
}

void buy(char * itemName){
	sqlite3 * conn;
	sqlite3_stmt * result;
	const char * tail;
	sql_open(DBPATH, &conn);

	prepare(conn, strjoin(nullArr, "INSERT INTO temp_purc VALUES(", strnum(nullArr2, getItemId(itemName)) ,");", NULL), 2000, &result, &tail);
	sqlite3_step(result);
	sqlite3_finalize(result);

	int moneyLeft = tempWallet() - getItemPrice(itemName);
	prepare(conn, strjoin(nullArr, "UPDATE temp_money set money=", strnum(nullArr2, moneyLeft)  ,";", NULL), 2000, &result, &tail);
	sqlite3_step(result);
	mapUpdate(&detailsMap, "wallet", strnum(nullArr2, moneyLeft));
	sqlite3_finalize(result);
	sqlite3_close(conn);
}