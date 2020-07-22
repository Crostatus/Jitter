#include "hashmap.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#define P 999149

// create hashmap with size of 50
HashMap HashCreate() {
	HashMap hash;
	hash.size = 50;
	hash.data = (KeyAndValue**)malloc(sizeof(KeyAndValue*) * hash.size);
	for(int i=0; i<hash.size; i++){
		hash.data[i] = (KeyAndValue*)malloc(sizeof(KeyAndValue));
		(hash.data[i])->key = NULL;
		(hash.data[i])->head = NULL;
		(hash.data[i])->tail = NULL;
		(hash.data[i])->next = NULL;
	}

	return hash;
}

// function hash
int HashCode(HashMap *hash, char *key) {
   return (((atoi(key)*hash->a) + hash->b) % P) % hash->size;
}

// add the new value in the HashMap
void HashAdd(HashMap *hash,char *key, struct timespec t_value) {
	int index = HashCode(hash , key);
	printf("indice inserimento: %d\n", index);
	if(hash->data[index]->key == NULL){
		printf("Sto inserendo nuovo elemento\n");
		QueueNode *new = malloc(sizeof(QueueNode));
		new->info = t_value;
		new->next = NULL;
		hash->data[index]->key = malloc(sizeof(char)*50);
		strcpy(hash->data[index]->key, key);
		hash->data[index]->head = new;
		hash->data[index]->tail = new;
		hash->data[index]->next = NULL;

	}
	else if(strcmp(hash->data[index]->key, key) == 0){
		// add only new data in the queue
		printf("Sto inserendo nuovo timespec elemento\n");
		QueueNode *new = malloc(sizeof(QueueNode));
		new->info = t_value;
		new->next = NULL;
		hash->data[index]->tail->next = new;
		hash->data[index]->tail = new;

	}
	else{
		// check if key is in the queue, and manage the conflict
		printf("Conflitto\n");
		KeyAndValue *aux = hash->data[index]->next;
		if(aux == NULL){
			QueueNode *new = malloc(sizeof(QueueNode));
			new->info = t_value;
			new->next = NULL;
			aux = malloc(sizeof(KeyAndValue));
			aux->key = malloc(sizeof(char)*50);
			strcpy(aux->key, key);
			aux->head = new;
			aux->tail = new;
			aux->next = NULL;
			return;
		}
		while( aux->next != NULL ){
			if(strcmp(aux->key, key) == 0){
				printf("QUI CI ARRIVO 1\n");
				QueueNode *new = malloc(sizeof(QueueNode));
				new->info = t_value;
				new->next = NULL;
				aux->tail->next = new;
				return;
			}
			aux = aux->next;
		}
		if(strcmp(aux->key, key) == 0){
			printf("QUI CI ARRIVO 2\n");
				QueueNode *new = malloc(sizeof(QueueNode));
				new->info = t_value;
				new->next = NULL;
				aux->tail->next = new;
				return;
		}

		printf("QUI CI ARRIVO 3\n");
		QueueNode *new = malloc(sizeof(QueueNode));
		new->info = t_value;
		new->next = NULL;
		aux->next = malloc(sizeof(KeyAndValue));
		aux->next->key = malloc(sizeof(char)*50);
		strcpy(aux->next->key, key);
		aux->next->head = new;
		aux->next->tail = new;
		aux->next->next = NULL;
	}

}

//  return 0 if the memory of HashMap[index] is free, -1 otherwise
int HashFreeIndex(HashMap *hash,int index) {
	if(index < hash->size) {
		KeyAndValue *item = hash->data[index];
		while( item != NULL){
			free(item->key);
			QueueNode *aux = item->head;
			while(aux != NULL){
				item->head = aux->next;
				free(aux);
				aux = item->head;
			}
			free(item->tail);
			item = item->next;
		}
		free(hash->data[index]);
		return 0;
	}
	return -1;
}

// free all memory of the HashMap
void HashFreeAll(HashMap *hash) {
	for(int i=0; i<hash->size; i++)
		HashFreeIndex(hash,i);
}

void HashPrint(HashMap hash){
	printf("\n		Valori nella tabella Hash:\n");
	for(int i=0; i<hash.size; i++){
		KeyAndValue *item = hash.data[i];
		while( item != NULL){
			QueueNode *aux = item->head;
			printf("	key: %s ---->\n", item->key);
			while( aux != NULL){
				printf("value: %ld(s), %ld(ns)\n", aux->info.tv_sec, aux->info.tv_nsec);
				aux = aux->next;
			}
			item = item->next;
		}
	}
}


/* // return element at HashMap[index] if there is, NULL otherwise
KeyAndValue *HashGetIndex(HashMap *hash,int index) {
	if(index < hash->size) {
		return hash->data[index];
	}
	return NULL;
}


KeyAndValue *HashGetWithKey(HashMap *hash,void *key) {
	for(size_t index=0;index<hash->length;index++) {
		KeyAndValue *item=hash->data[index];
		if(item != NULL && item->key == key) {
			return item;
		}
	}
	return NULL;
}

// return index of value in the HashMap
void *HashGetKey(HashMap *hash,void *value) {
	for(size_t index=0;index<hash->length;index++) {
		KeyAndValue *item=hash->data[index];
		if(item != NULL && item->value == value) {
			return item->key;
		}
	}
	return NULL;
}

KeyAndValue *HashGetWithValue(HashMap *hash,void *value) {
	for(size_t index=0;index<hash->length;index++) {
		KeyAndValue *item=hash->data[index];
		if(item != NULL && item->value == value) {
			return item;
		}
	}
	return NULL;
}

void *HashGetValue(HashMap *hash,void *key) {
	for(size_t index=0;index<hash->length;index++) {
		KeyAndValue *item=hash->data[index];
		if(item != NULL && item->key == key) {
			return item->value;
		}
	}
	return NULL;
}*/
