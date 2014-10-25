#ifndef VNUTIL_H_
#define VNUTIL_H_

#include "myutility.h"
#include <string.h>
#include <stdlib.h>

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

#endif