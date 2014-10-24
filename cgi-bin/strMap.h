#ifndef STRMAP_H_
#define	STRMAP_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MAP_SIZE 100
#define MAX_STR_SIZE 2000

typedef struct{
	char key[MAX_STR_SIZE];
	char val[MAX_STR_SIZE];
} strPair;

typedef struct{
	int numEntries;
	strPair pairList[MAX_MAP_SIZE];
} strMap;

/*
	Returns itr to entry
*/
strPair * mapFind(strMap * map, char * key){
	int numEntries = map->numEntries;
	int i;

	for(i = 0; i<numEntries; i++){
		if(!strcmp(key, map->pairList[i].key )){
			return  &(map->pairList[i]);
		}
	}

	return NULL;
}

/*
	Returns value stored in map[key]
*/
char * mapVal(strMap * map, char * key){
	strPair * itr = mapFind(map, key);

	if(itr == NULL){
		return NULL;
	} else {
		return itr->val;
	}
}

strPair * mapAdd(strMap * map, char * key, char * val){
	int numEntries = map -> numEntries;
	strcpy(map->pairList[numEntries].key, key);
	strcpy(map->pairList[numEntries].val, val);
	map -> numEntries = ++numEntries;

	return &(map->pairList[numEntries-1]);
}

strPair * mapUpdate(strMap * map, char * key, char * val){
	strPair * itr = mapFind(map, key);
	if(itr == NULL){
		return mapAdd(map, key, val);
	} else {
		strcpy(itr->val, val);
		return itr;
	}
}

strPair * mapApp(strMap * map, char * key, char * val){
	strPair * itr = mapFind(map, key);
	if(itr == NULL){
		return mapAdd(map, key, val);
	} else {
		strcat(itr->val, val);
		return itr;
	}
}

strMap * mapClear(strMap * map){
	map -> numEntries = 0;
	memset(map->pairList, 0, MAX_STR_SIZE * MAX_MAP_SIZE * sizeof(char));
	return map;
}

int mapSize(strMap * map){
	return map -> numEntries;
}

void printMap(strMap * map){
	int i;
	for(i = 0; i<mapSize(map); i++){
		printf("%s - %s<br>\n", map->pairList[i].key, map->pairList[i].val);
	}
}

#endif