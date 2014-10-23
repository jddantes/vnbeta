#include <string.h>
#include <ctype.h>
#include <sqlite3.h>

/*
	originally error or abort, but both are commonly used so I went with terminate, or term for short
*/
void term(char * message){
	printf("error: %s\n", message);
	printf(". Aborting...\n");
	exit(0);
}

char * strapp(char * dest, const char * src){
	return strcpy(dest+strlen(dest), src);
}


/*
	Removes url encoding
*/
char map(char * code){
	if(!strcmp(code, "3B")) return ';';
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