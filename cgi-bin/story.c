#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "macros.h"
#include "myutility.h"
#include "mytypes.h"

void loadScene(FILE * fp);
void handle(char * action, char * buffer);

dialogue dialogues[1000] = {};
choiceList choices[100] = {};
int numDialogues = 0;
int numChoices = 0;

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
		char scene[2000] = SCENEPATH;
		strcat(scene, "/");
		char d_str[2000] = {};
		strcat(scene, strtok(state, ","));
		strcpy(d_str, strtok(NULL, ", "));
		int d_index = atoi(d_str);

		FILE * fp = fopen(scene, "r");
		if(fp == NULL){
			char errMsg[2000];
			sprintf(errMsg, "Could not open scene %s", scene);
			term(errMsg);
		}

		loadScene(fp);

		fclose(fp);

		int i;
		for(i = 0; i<numDialogues; i++){
			printf("%s:%s<br>", dialogues[i].speaker, dialogues[i].speech);
		}

	}

	return 0;

}

void loadScene(FILE * fp){
	char buffer[2000];

	// Read scene headers
	while(mgets(buffer, 2000, fp)!=NULL){
		if(!strcmp(buffer, "-----")){
			break;
		}
	}

	while(mgets(buffer, 2000,fp)!=NULL){
		if(buffer[0] == 0){
			continue;
		}

		char action[2000] = {};
		int len = strlen(buffer);
		if(buffer[0] == '[' && buffer[len-1] == ']'){
			strcpy(action, buffer+1);
			len -= 2;
			action[len] = 0;
		}
		char endMarker[2000] = "[/";
		strcat(endMarker, action);
		strcat(endMarker, "]");

		while(mgets(buffer, 2000, fp) != NULL){
			if(!strcmp(buffer, endMarker)){
				break;
			}
			handle(action, buffer);
		}

	}
}

void handle(char * action, char * buffer){
	if(!strcmp(action, "#")){ // Comment, do nothing

	} else if (!strcmp(action, "d") ){ // Dialogue, one per line, in format speaker:speech
		char * pch = strstr(buffer, ":");
		strncpy(dialogues[numDialogues].speaker, buffer, pch-buffer);
		strcpy(dialogues[numDialogues].speech, pch+1);
		numDialogues++;
	}


 
}