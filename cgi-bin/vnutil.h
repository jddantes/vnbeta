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

#endif