#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "macros.h"
#include "myutility.h"
#include "mytypes.h"
#include "inputReader.h"

void loadScene(FILE * fp);
void handle(char * action, char * buffer);

dialogue dialogues[1000] = {};
choiceList choices[100] = {};
int numDialogues = 0;
int numChoices = 0;

pair keyMap[100];
int keyMapSize = 4;
/*
	Contents of keyMap:
	0 nextScene
	1 speaker
	2 speech
	3 state
*/

int main(){
	
	printf("Content-type:text/html\n\n");

	char * data = getenv("CONTENT_LENGTH");
	int datalen = atoi(data);

	char buffer[2000];

	if(!datalen){
		printf("<html><body>");
		printf("Go back to the homepage :)");
		printf("</body></html>");
	} else {
		fgets(buffer, 2000, stdin);
		decode(buffer);
		printf("Buffer:%s", buffer);
		char state[2000];
		sscanf(buffer, "state=%s", state);
		char scene[2000] = SCENEPATH;
		strcat(scene, "/");
		char d_str[2000] = {};
		strcat(scene, strtok(state, ","));
		strcpy(d_str, strtok(NULL, ", "));
		int d_index = atoi(d_str);

		FILE * fp = mopen(scene, "r");

		loadScene(fp);

		fclose(fp);

		int i;
		for(i = 0; i<numDialogues; i++){
			printf("%s:%s<br>", dialogues[i].speaker, dialogues[i].speech);
		}

		strcpy(keyMap[1].key, "speaker");
		strcpy(keyMap[1].value, dialogues[d_index].speaker);
		strcpy(keyMap[2].key, "speech");
		strcpy(keyMap[2].value, dialogues[d_index].speech);
		strcpy(keyMap[3].key, "state");
		strcpy(keyMap[3].value, scene + strlen(SCENEPATH) + 1); // Just take the scene's filename, not the whole path
		strcat(keyMap[3].value, ",");
		itoa(d_index+1, keyMap[3].value+strlen(keyMap[3].value), 10);
		if(d_index == numDialogues-1){ // Branch to scene instead of branching to
			strcpy(keyMap[3].value, keyMap[0].value);
		}
		printf("Next scene: %s", keyMap[3].value);
		for(i = 0; i<keyMapSize; i++){
			printf("<br/>%s:%s",keyMap[i].key, keyMap[i].value);
		}

		char htmlpath[2000] = HTMLPATH;
		strcat(htmlpath, "/story.html");
		fp = mopen(htmlpath, "r");
		readInput(fp, stdout, keyMap, keyMapSize);
		fclose(fp);

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
	} else if(!strcmp(action, "b")) { // Branch to scene
		printf("<br>branch: %s<br>", buffer);
		strcpy(keyMap[0].key, "nextScene");
		strcpy(keyMap[0].value, buffer);
		strcat(keyMap[0].value, ",0");
	}


 
}