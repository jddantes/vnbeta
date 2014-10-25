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
void temp_money();
void temp_purc();
void initializeTemp();


char nullArr[MAX_STR_SIZE];

dialogue dialogues[1000] = {};
choice choices[100] = {};
int numDialogues = 0;
int numChoices = 0;

strMap postData;
strMap detailsMap;

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

		if(!strlen(scene)){
			strcpy(scene, DEFAULT_SCENE);
		} else if (!strcmp(scene, END_SCENE)){
			credits();
		}

		if(mapVal(&postData, "wallet") == NULL){;
			printf("No wallet. Initializing temp.<br>");
			initializeTemp();
		} else {
			char nullArr2[2000];
			mapUpdate(&detailsMap, "wallet", strnum(nullArr2, tempWallet()));
			printf("Updating wallet to %s<br>\n", strnum(nullArr2, tempWallet())); 
		}

		int d_index = atoi(d_str);
		mapAdd(&detailsMap, "d_index", d_str);
		mapAdd(&detailsMap, "currentState", mapVal(&postData, "state"));
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
		

		// Render HTML
		char htmlpath[2000];
		strjoin(htmlpath, HTMLPATH, "/story.html", NULL);
		render(htmlpath, &detailsMap);

		printMap(&detailsMap);
	}

	return 0;

}

void loadScene(char * scene){

	char scenePath[2000];
	strjoin(scenePath, SCENEPATH, "/", scene, NULL);
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
	exit(0);

}

void initializeTemp(){
	temp_money();
	temp_purc();
}

void temp_money(){
	sqlite3 * conn;
	sqlite3_stmt * result;
	const char * tail;
	sql_open(DBPATH, &conn);
	prepare(conn, "DELETE FROM temp_money;", 2000, &result, &tail);
	sqlite3_step(result);
	sqlite3_finalize(result);

	state_t s = makeStateFromTriple(mapVal(&postData, "state"));
	prepare(conn, strjoin(nullArr, "SELECT money FROM slots WHERE usr_id=", s.usr, ";", NULL), 2000, &result, &tail);
	sqlite3_step(result);
	char wallet_str[2000];
	itoa(sqlite3_column_int(result, 0), wallet_str, 10);
	mapAdd(&detailsMap, "wallet", wallet_str);
	sqlite3_finalize(result);

	prepare(conn, strjoin(nullArr, "INSERT INTO temp_money VALUES(", mapVal(&detailsMap, "wallet"), ");", NULL), 2000, &result, &tail);
	sqlite3_step(result);
	sqlite3_finalize(result);

	sqlite3_close(conn);
}

void temp_purc(){
	sqlite3 * conn;
	sqlite3_stmt * result;
	const char * tail;
	sql_open(DBPATH, &conn);
	prepare(conn, "DELETE FROM temp_purc", 2000, &result, &tail);
	sqlite3_step(result);
	sqlite3_finalize(result);

	state_t s = makeStateFromTriple(mapVal(&postData, "state"));

	prepare(conn, strjoin(nullArr, "SELECT item_id FROM purchases WHERE usr_id=", s.usr ,";", NULL), 2000, &result, &tail);
	while(sqlite3_step(result) == SQLITE_ROW){
		char purchase[2000];
		itoa(sqlite3_column_int(result, 0), purchase, 10);
		sqlite3_stmt * result2;
		const char * tail2;
		prepare(conn, strjoin(nullArr, "INSERT INTO temp_purc VALUES(", purchase,");", NULL), 2000, &result2, &tail2);
		sqlite3_step(result2);
		sqlite3_finalize(result2);
	}

	sqlite3_finalize(result);
	sqlite3_close(conn);

}