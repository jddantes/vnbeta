#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "macros.h"
#include "myutility.h"
#include "mytypes.h"
#include "inputReader.h"
#include "strMap.h"
#include "vnutil.h"

void loadScene(char * scene);
void handle(char * action, char * buffer);
void credits();

char nullArr[MAX_STR_SIZE];

dialogue dialogues[1000] = {};
choice choices[100] = {};
int numDialogues = 0;
int numChoices = 0;

strMap postData;
strMap detailsMap;

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
		makePostMap(&postData, buffer);

		char tripleState[2000];
		strcpy(tripleState, mapVal(&postData, "state"));
		char scene[2000];
		char d_str[2000];
		readTriple(tripleState, usr, scene, d_str);
		int d_index = atoi(d_str);
		mapAdd(&detailsMap, "d_index", d_str);
		loadScene(scene);



		// Process normal dialogues
		mapAdd(&detailsMap, "speaker", dialogues[d_index].speaker);
		mapAdd(&detailsMap, "speech", dialogues[d_index].speech);
		mapAdd(&detailsMap, "state", makeTriple(nullArr, usr, scene, d_index+1));

		if(d_index == numDialogues-1){ // Branch to scene instead of branching to
			mapUpdate(&detailsMap, "state", mapVal(&detailsMap, "nextScene"));
		}

		// Process choices 
		mapAdd(&detailsMap, "choiceList", "");
		for(i = 0; i<numChoices; i++){
			mapApp(&detailsMap, "choiceList", strjoin(nullArr, "<input type='radio' name='state' value='", usr, ":", choices[i].branch, ",0' />", choices[i].speech, "</br>\n", NULL));
		}
		printMap(&detailsMap);

		// Render HTML
		char htmlpath[2000];
		strjoin(htmlpath, HTMLPATH, "/story.html", NULL);
		FILE * fp = mopen(htmlpath, "r");
		readInput(fp, stdout, &detailsMap);
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
		// printf("<br>branch: %s<br>", buffer);
		mapAdd(&detailsMap, "nextScene", strjoin(nullArr, usr, ":", buffer, ",0", NULL));
	} else if(!strcmp(action, "c")){
		split(buffer, choices[numChoices].branch, choices[numChoices].speech, ":");
		numChoices++;
	}

}

void credits(){
	printf("<html><body>You've reached the end!</body></html>");

}