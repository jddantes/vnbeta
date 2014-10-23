#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mytypes.h"



char * getKeyVal(char * dest, char * key, pair * keyMap, int mapSize){
	int i;
	for(i = 0; i<mapSize; i++){
		if(!strcmp(keyMap[i].key, key)){
			return strcpy(dest,keyMap[i].value);
		}
	}

	return 0;
}

void readInput(FILE * fp, FILE * output, pair * keyMap, int mapSize){
	const int bufferSize = 2000;
	char buffer[bufferSize];
	buffer[0] = 0;
	int bufferIndex = 0;
	char c;

	while(fscanf(fp, "%c", &c) != EOF){

		if(c=='{'){
			fscanf(fp, "%c", &c);
			if(c == '%'){

				while(fscanf(fp, "%c", &c)!=EOF){
					if(bufferIndex >= bufferSize){
						fprintf(output, "%s", buffer);
						buffer[0] = 0;
						bufferIndex = 0;
						break;
					}

					if(c == '%'){
						fscanf(fp, "%c", &c);
						if(c == '}'){
							buffer[bufferIndex] = 0;
							char trimmed[bufferSize];
							trimmed[0] = 0;
							sscanf(buffer, "{%% s %%}", trimmed);

							char stringVal[bufferSize];
							stringVal[0] = 0;
							getKeyVal(stringVal, trimmed, keyMap, mapSize);
							printf("%s", stringVal);

							buffer[0] = 0;
							bufferIndex = 0;

						} else {
							buffer[bufferIndex] = '%';
							buffer[bufferIndex+1] = c;
							bufferIndex += 2;
						}
					} else {
						buffer[bufferIndex] = c;
						bufferIndex++;
					}
				}


			} else {
				fprintf(output, "%c", c);
			}


		} else {
			fprintf(output, "%c", c);
		}
	}
	if(buffer[0]){
		fprintf(output, "%s", buffer);
	}
}