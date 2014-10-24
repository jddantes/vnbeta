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
void credits();

dialogue dialogues[1000] = {};
choice choices[100] = {};
int numDialogues = 0;
int numChoices = 0;

pair postData[100];
int numPostData = 0;
pair keyMap[100];
int keyMapSize = 5;
/*
	Contents of keyMap:
	0 nextScene
	1 speaker
	2 speech
	3 state
	4 choiceList
*/

int i;
char usr[2000];
int usr_id;
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
		
		processPostData(buffer, postData, &numPostData);

		char tripleState[2000];
		getKeyVal(tripleState, "state", postData, numPostData);
		char scene[2000];
		int d_index;
		readTriple(tripleState, usr, scene, &d_index);

		loadScene(scene);


		// Process normal dialogues
		

		strcpy(keyMap[1].key, "speaker");
		strcpy(keyMap[1].value, dialogues[d_index].speaker);
		strcpy(keyMap[2].key, "speech");
		strcpy(keyMap[2].value, dialogues[d_index].speech);
		strcpy(keyMap[3].key, "state");
		strjoin(keyMap[3].value, usr, ":", sceneName, ",", NULL);
		itoa(d_index+1, keyMap[3].value+strlen(keyMap[3].value), 10);
		if(d_index == numDialogues-1){ // Branch to scene instead of branching to
			strjoin(keyMap[3].value, usr, ":", keyMap[0].value, NULL);
		}
		for(i = 0; i<keyMapSize; i++){
			printf("<br/>%s:%s",keyMap[i].key, keyMap[i].value);
		}

		// Process choices 
		strcpy(keyMap[4].key, "choiceList");
		for(i = 0; i<numChoices; i++){
			strapp(keyMap[4].value, "<input type='radio' name='state' value='", usr, ":", choices[i].branch, ",0' />", choices[i].speech, "</br>\n", NULL);
		}


		printf("<br>Printing keyMap: <br>");
		for(i = 0; i<keyMapSize; i++){
			printf("%s:%s<br>", keyMap[i].key, keyMap[i].value);
		}
		printf("----<br>");

		// Render HTML
		char htmlpath[2000];
		strjoin(htmlpath, HTMLPATH, "/story.html", NULL);
		FILE * fp = mopen(htmlpath, "r");
		readInput(fp, stdout, keyMap, keyMapSize);
		fclose(fp);

	}

	return 0;

}

void loadScene(char * scene){

	char scenePath[2000];
	strjoin(scenePath, SCENEPATH, "/", scene);
	FILE * fp = mopen(scenePath, "r");

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
		char endMarker[2000];
		strjoin(endMarker, "[/", action, "]", NULL);

		while(mgets(buffer, 2000, fp) != NULL){
			if(!strcmp(buffer, endMarker)){
				break;
			}
			handle(action, buffer);
		}

	}

	close(fp);
}

void handle(char * action, char * buffer){
	if(!strcmp(action, "#")){ // Comment, do nothing

	} else if (!strcmp(action, "d") ){ // Dialogue, one per line, in format speaker:speech
		split(buffer, dialogues[numDialogues].speaker, dialogues[numDialogues].speech, ":");
		numDialogues++;
	} else if(!strcmp(action, "b")) { // Branch to scene
		printf("<br>branch: %s<br>", buffer);
		strcpy(keyMap[0].key, "nextScene");
		strjoin(keyMap[0].value, buffer, ",0", NULL);
	} else if(!strcmp(action, "c")){
		split(buffer, choices[numChoices].branch, choices[numChoices].speech, ":");
		numChoices++;
	}


 
}

void credits(){
	printf("<html><body>You've reached the end!</body></html>");

}