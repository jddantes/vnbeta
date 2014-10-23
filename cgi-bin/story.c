#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "macros.h"
#include "myutility.h"

void loadScene(FILE * fp);

dialogue dialogues[1000] = {};
choiceList choices[100] = {};
int numDialogues;

int main(){
	
	printf("Content-type:text/html\n\n");

	char * data = getenv("CONTENT_LENGTH");
	int datalen = strlen(data);

	char buffer[2000];

	if(!datalen){
		printf("Go back to the homepage :)");
	} else {
		fgets(buffer, 2000, stdin);
		decode(buffer);
		char state[2000];
		sscanf(buffer, "state=%s", state);
		char scene[2000] = {};
		char d_str[2000] = {};
		strcpy(scene, strtok(state, ","));
		strcpy(d_str, strtok(NULL, ", "));
		int d_index = atoi(d_str);

		FILE * fp = fopen(scene, "r");
		if(fp == NULL){
			term("Could not open scene");
		}

		loadScene(fp);

		fclose(fp);

	}

	return 0;

}

void loadScene(FIlE * fp){
	char buffer[2000];

	// Read scene headers
	while(mgets(buffer, 2000, fp)!=NULL){
		if(!strcmp(buffer, "-----")){
			break;
		}
	}

	while(mgets(buffer, 2000)!=NULL){
		if(buffer[0] == 0){
			continue;
		}


	}
}