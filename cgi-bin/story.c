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
		// usr_id  = atoi(getKeyVal(usr, "state", postData, numPostData));
		char temp[2000];
		char statePair[2000]; // filename/d_index pair
		getKeyVal(temp, "state", postData, numPostData);
		split(temp, usr, statePair, ":");

		char sceneName[2000];
		char scene[2000] = SCENEPATH; strcat(scene, "/");
		char d_str[2000];
		split(statePair, sceneName, d_str, ",");
		strcat(scene, sceneName);
		int d_index = atoi(d_str);



		// Get save data from database

		// sqlite3 * conn;
		// sqlite3_stmt * result;
		// int error = 0;
		// const char * tail;

		// sql_open(DBPATH, &conn);
		// prepare(conn, "SELECT * FROM slots;", 2000, &result, &tail);
		// sqlite3_step(result);

		// int d_index = sqlite3_column_int(result,2);
		// char sceneName[2000];
		// strcpy(sceneName, sqlite3_column_text(result, 1));
		// char scene[2000] = SCENEPATH;
		// strcat(scene, "/");
		// strcat(scene, sceneName);

		// if(!strcmp(sceneName, "end.vn")){
		// 	credits();
		// 	return 0;
		// }



		FILE * fp = mopen(scene, "r");

		loadScene(fp);

		fclose(fp);


		// Process normal dialogues
		

		strcpy(keyMap[1].key, "speaker");
		strcpy(keyMap[1].value, dialogues[d_index].speaker);
		strcpy(keyMap[2].key, "speech");
		strcpy(keyMap[2].value, dialogues[d_index].speech);
		strcpy(keyMap[3].key, "state");
		strcpy(keyMap[3].value, usr);
		strcat(keyMap[3].value, ":");
		strcat(keyMap[3].value, sceneName); 
		strcat(keyMap[3].value, ",");
		itoa(d_index+1, keyMap[3].value+strlen(keyMap[3].value), 10);
		if(d_index == numDialogues-1){ // Branch to scene instead of branching to
			strcpy(keyMap[3].value, keyMap[0].value);
		}
		for(i = 0; i<keyMapSize; i++){
			printf("<br/>%s:%s",keyMap[i].key, keyMap[i].value);
		}

		// Process choices 
		strcpy(keyMap[4].key, "choiceList");
		for(i = 0; i<numChoices; i++){
			char radioElement[2000] = "<input type='radio' name='state' value='";
			strcat(radioElement, choices[i].branch);
			strcat(radioElement, ",0' />");
			strcat(radioElement, choices[i].speech);
			strcat(radioElement, "</br>\n");

			strcat(keyMap[4].value, radioElement);
		}


		printf("<br>Printing keyMap: <br>");
		for(i = 0; i<keyMapSize; i++){
			printf("%s:%s<br>", keyMap[i].key, keyMap[i].value);
		}
		printf("----<br>");

		// Render HTML
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
		split(buffer, dialogues[numDialogues].speaker, dialogues[numDialogues].speech, ":");
		numDialogues++;
	} else if(!strcmp(action, "b")) { // Branch to scene
		printf("<br>branch: %s<br>", buffer);
		strcpy(keyMap[0].key, "nextScene");
		strcpy(keyMap[0].value, buffer);
		strcat(keyMap[0].value, ",0");
	} else if(!strcmp(action, "c")){
		split(buffer, choices[numChoices].branch, choices[numChoices].speech, ":");
		numChoices++;
	}


 
}

void credits(){
	printf("<html><body>You've reached the end!</body></html>");

}