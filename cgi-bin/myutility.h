#ifndef MYUTILITY_H_
#define MYUTILITY_H_

#include <string.h>
#include <ctype.h>
#include <sqlite3.h>
#include "mytypes.h"
#include "strmap.h"
/*
	originally error or abort, but both are commonly used so I went with terminate, or term for short
*/
void term(char * message){
	printf("error: %s\n", message);
	printf(". Aborting...\n");
	exit(0);
}

/*
	Removes url encoding
*/
char map(char * code){
	if(!strcmp(code, "3B")) return ';';
	if(!strcmp(code, "3A")) return ':';
	if(!strcmp(code, "20")) return ' ';
	if(!strcmp(code, "25")) return '%';
	if(!strcmp(code, "21")) return '!';
	if(!strcmp(code, "26")) return '&';
	if(!strcmp(code, "2A")) return '*';
	if(!strcmp(code, "2B")) return '+';
	if(!strcmp(code, "2C")) return ',';
	if(!strcmp(code, "2E")) return '.';
}

/*
	Changes str to decoded value
*/
char * decode(char * str){
	int len = strlen(str);
	char decoded[len+1];

	int i = 0;
	int index = 0;

	char code[3] = {};

	for(i = 0; i<len ;i++){
		if(str[i]=='+'){
			decoded[index] = ' ';
			index++;
		} else if (str[i] == '%' && i+2 < len){
			code[0] = str[i+1];
			code[1] = str[i+2];
			decoded[index] = map(code);
			i+=2;
			index++;
		} else {
			decoded[index] = str[i];
			index++;
		}
	}

	decoded[index] = 0;

	return strcpy(str, decoded);
}



int isallNum(char * str){
	int i;
	for(i = 0; str[i]; i++){
		if(str[i] == '-' && i==0 && strlen(str)>1){
			continue;
		}
		if(!isdigit(str[i])){
			return 0;
		}
	}

	return 1;
}

/*
	sqlite3_prepare_v2 with error check
*/
int prepare(sqlite3 * conn, const char * query, int numBytes, sqlite3_stmt ** result, const char ** tail){
	int error = sqlite3_prepare_v2(conn, query, numBytes, result, tail);

	if(error != SQLITE_OK){
		term("Got error while retrieving data");
	}

	return error;
}

/*
	sqlite3_open with error check
*/
int sql_open(const char * dbpath, sqlite3 ** conn){
	int error = sqlite3_open(dbpath, conn);
	if(error){
		term("could not open database");
	}
	return error;
}


char * mgets(char * str, int num, FILE * stream){
	char * ret = fgets(str, num, stream);
	if(str[strlen(str)-1] == '\n'){
		str[strlen(str)-1] = 0;		
	}
	return ret;
}

FILE * mopen(const char * filename, const char * mode){
	FILE * ret = fopen(filename, mode);
	if(ret == NULL){
		char errMsg[2000];
		sprintf(errMsg, "could not open file %s", filename);
		term(errMsg);
	} else {
		return ret;
	}
}

void split(char * buffer, char * a, char * b, char * delim){
	char * pch = strstr(buffer, delim);
	strncpy(a, buffer, pch-buffer);
	strcpy(b, pch+1);

}

void makePostMap(strMap * map, char * postString){
	char buffer[strlen(postString)+1];
	strcpy(buffer, postString);
	decode(buffer);

	char * pch = strtok(buffer, "&");

	while(pch != NULL){
		char key[MAX_STR_SIZE];
		char val[MAX_STR_SIZE];
		split(pch, key, val, "=");
		mapAdd(map, key, val);

		pch = strtok(NULL, "&");
	}

}


/*
	strjoin and strapp to emulate c++ string handling
*/

char * strjoin(char * dest, ...){
	va_list vl;
	va_start(vl, dest);
	
	char * str;

	int i;
	for(i = 0; ; i++){
		str = va_arg(vl, char *);

		if(str == NULL){
			break;
		}
		i ? strcat(dest, str) : strcpy(dest, str);
	}

	va_end(vl);

	return dest;
}

char * strapp(char * dest, ...){
	va_list vl;
	va_start(vl, dest);

	char * str;

	int i;
	for(i = 0; ; i++){
		str = va_arg(vl, char *);

		if( str == NULL ){
			break;
		}

		strcat(dest, str);
	}

	va_end(vl);

	return dest;
}

char * strappNum(char * dest, int num){
	char num_str[2000];
	itoa(num, num_str, 10);
	return strapp(dest, num_str, NULL);
}
#endif