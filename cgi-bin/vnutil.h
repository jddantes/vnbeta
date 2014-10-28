#ifndef VNUTIL_H_
#define VNUTIL_H_

#include "myutility.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

void readTriple(char * tripleState, char * usr, char * scene, char * d_str){
	char scene_d[2000];
	split(tripleState, usr, scene_d, ":");
	split(scene_d, scene, d_str, ",");
}

void readTripleNum(char * tripleState, char * usr, char * scene, int * d_index){
	char scene_d[2000];
	split(tripleState, usr, scene_d, ":");

	char d_str[2000];
	split(scene_d, scene, d_str, ",");

	*d_index = atoi(d_str);
}


char * makeTriple(char * dest, char * usr, char * scene, int d_index){
	char d_str[2000];
	itoa(d_index, d_str , 10);
	return strjoin(dest, usr, ":", scene, ",", d_str, NULL);
}

char * makeTripleNum(char * dest, int usr_id, char * scene, int d_index){
	char usr[2000];
	char d_str[2000];
	itoa(usr_id, usr, 10);
	itoa(d_index, d_str, 10);
	return strjoin(dest, usr, ":", scene, ",", d_str, NULL);
}

typedef struct{
	char usr[2000];
	char scene[2000];
	char d_str[2000];
} state_t;

state_t makeState(char * usr, char * scene, int d_index){
	state_t s;

	strcpy(s.usr, usr);
	strcpy(s.scene, scene);
	itoa(d_index, s.d_str, 10);

	return s;
}

state_t makeStateFromTriple(char * tripleState){
	state_t s;

	char usr[2000];
	char scene[2000];
	int d_index;

	readTripleNum(tripleState, usr, scene, &d_index);
	s = makeState(usr, scene, d_index);

	return s;
}

/*
	Items
*/

int getItemPrice(char * name){
	char nullArr[2000];

	sqlite3 * conn;
	sqlite3_stmt * result;
	const char * tail;
	sql_open(DBPATH, &conn);
	prepare(conn, strjoin(nullArr, "SELECT price FROM items WHERE item_name='", name, "';", NULL), 2000, &result, &tail);
	sqlite3_step(result);
	int price = sqlite3_column_int(result, 0);
	sqlite3_finalize(result);
	sqlite3_close(conn);

	return price;
}

char * getItemName(int itemId, char * dest){
	char nullArr[2000];
	char nullArr2[2000];

	sqlite3 * conn;
	sqlite3_stmt * result;
	const char * tail;
	sql_open(DBPATH, &conn);
	prepare(conn, strjoin(nullArr, "SELECT item_name FROM items WHERE item_id=", strnum(nullArr2, itemId), ";", NULL), 2000, &result, &tail);
	sqlite3_step(result);
	strcpy(dest, (char *)sqlite3_column_text(result, 0));
	sqlite3_finalize(result);
	sqlite3_close(conn);
	return dest;
}

int getItemId(char * itemName){
	char nullArr[2000];

	sqlite3 * conn;
	sqlite3_stmt * result;
	const char * tail;
	sql_open(DBPATH, &conn);
	prepare(conn, strjoin(nullArr, "SELECT item_id FROM items WHERE item_name='", itemName, "';", NULL), 2000, &result, &tail);
	sqlite3_step(result);
	int id = sqlite3_column_int(result, 0);
	sqlite3_finalize(result);
	sqlite3_close(conn);
	return id;
}

int tempWallet(){
	char nullArr[2000];

	sqlite3 * conn;
	sqlite3_stmt * result;
	const char * tail;
	sql_open(DBPATH, &conn);
	prepare(conn, strjoin(nullArr, "SELECT money FROM temp_money;", NULL), 2000, &result, &tail);
	sqlite3_step(result);
	int money = sqlite3_column_int(result, 0);
	sqlite3_finalize(result);
	sqlite3_close(conn);
	return money;
}

char * getCurrentTime(char * str){
	time_t rawtime = time(NULL);
	struct tm * timeinfo = localtime(&rawtime);

	strftime(str, 2000, "%Y-%m-%d %X", timeinfo);

	return str;
}

#endif