#pragma once
#include<utility.h>
#include<stdio.h>
#include<stdlib.h>

void read_config_values_inizialize(int *packet, int *size, int *a, int *b, int *P){
	FILE *inputFile;
	int maxLineLenght = 64;
	char *buffer = (char*)malloc(maxLineLenght * sizeof(char));

	if((inputFile = fopen("confing.txt", "r")) == NULL){
		perror("Errore in apertura del file di configurazione");
		exit(EXIT_FAILURE);
	}

	while(fgets(buffer, maxLineLenght, inputFile) != NULL){

		char *token = strtok(buffer, " ");

		if(strcmp(token, "Packet") == 0){
			token = strtok(NULL," ");
			*packet = atoi(token);
		}
		else if(strcmp(token, "table_size") == 0){
			token = strtok(NULL, " ");
			*size = atoi(token);
		}
		else if(strcmp(token, "A") == 0){
			token = strtok(NULL, " ");
			 *a = atoi(token);
		}
		else if(strcmp(token, "B") == 0){
			token = strtok(NULL, " ");
			*b = atoi(token);
	   }
		else if(strcmp(token, "P") == 0){
			token = strtok(NULL, " ");
			*P = atoi(token);
		}
}
