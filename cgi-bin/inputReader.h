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

	while(fscanf(fp, "%c", &c)!=EOF){
		if(c=='{'){
			buffer[bufferIndex] = c;
			bufferIndex++;

			fscanf(fp, "%c", &c);
			if(c == '%'){

				buffer[bufferIndex] = c;
				bufferIndex++;

				while(fscanf(fp, "%c", &c)!=EOF){

					if(bufferIndex>=bufferSize){
						buffer[bufferIndex] = 0;
						fprintf(output, "%s", buffer);
						bufferIndex = 0;
						buffer[0] = 0;
						break;
					}

					buffer[bufferIndex] = c;
					bufferIndex++;

					if(c == '%'){ // Possible end
						fscanf(fp, "%c", &c);
						buffer[bufferIndex] = c;
						bufferIndex++;

						if(c == '}'){
							buffer[bufferIndex] = 0; // Found end marker
							buffer[bufferIndex-2] = 0; // Exclude ending %}

							char trimmed[bufferSize]; // Trim whitespace
							sscanf(buffer+2," %s }", trimmed);
							char retVal[bufferSize];
							retVal[0] = 0;
							getKeyVal(retVal, trimmed, keyMap, mapSize);
							printf("%s", retVal);
							if(!retVal[0]){ // Not found in keyMap
								printf("%s%%}", buffer);
							}

							bufferIndex = 0;
							buffer[0] = 0;

							break;
						} 

					} 


				}


			} else {
				buffer[bufferIndex] = c;
				buffer[bufferIndex+1] = 0;
				fprintf(output, "%s", buffer);
				bufferIndex = 0;
				buffer[0] = 0;
			}


		} else {
			fprintf(output, "%c", c);
		}
	}	

	if(buffer[0]){
		fprintf(output,"%s", buffer);
	}


}