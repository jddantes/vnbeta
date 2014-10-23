#ifndef MYTYPES_H_
#define MYTYPES_H_

#include "macros.h"

typedef struct{
	char key[2000];
	char value[2000];
} pair;

typedef struct{
	char speaker[2000];
	char speech[2000];
	int isChoice;
} dialogue;

typedef struct{
	char choice[100][2000];
	int numChoices;
} choiceList;

#endif