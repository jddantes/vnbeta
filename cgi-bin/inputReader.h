#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mytypes.h"
#include "strMap.h"
#include "myutility.h"

void readInput(FILE * fp, FILE * output, strMap * map);


void render(char * filename, strMap * map){
	FILE * fp = mopen(filename, "r");
	readInput(fp, stdout, map);
	close(fp);
}

void readInput(FILE * fp, FILE * output, strMap * map){

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

							char * actualValue = mapVal(map, trimmed);
							if( actualValue != NULL){
								fprintf(output, "%s", actualValue);
							} else {
								// Do nothing when keyword is not found
								// printf("%s\n", strjoin(trimmed, buffer, "%}", NULL));
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


