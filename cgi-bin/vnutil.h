#include <myutility.h>
#include <stdlib.h>

void readTriple(char * tripleState, char * usr, char * scene, int * d_index){
	char scene_d[2000];
	split(tripleState, usr, scene_d, ":");

	char d_str[2000];
	split(scene_d, scene, d_str, ",");

	*d_index = atoi(d_str);
}