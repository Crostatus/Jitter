#include "hashmap.h"
#include "time.h"
#define P 999149

// create hashmap with size of 10
void HashCreate(HashMap *hash, size_t size) {
	hash->size=size;
	hash->data=malloc(sizeof(KeyAndValue*) * hash->size);
}

// function hash
int hashCode(HashMap *hash, int ip) {
   return ((ip*hash->a) + hash->b % P) % hash->size;
}

// add the new value in the HashMap
void HashAdd(HashMap *hash,int ip, struct timespec t_value) {
	int index = hashCode(hash , ip);
	if(hash->data[index]->key == NULL){
		QueueNode *new = malloc(sizeof(QueueNode));
		new->info = t_value;
		new->next = NULL;
		KeyAndValue newKey;
		newKey.key = ip;
		newKey.head = new;
		newKey.tail = new;
		newKey.next = NULL;
		hash->data[index] = &newKey;
	}
	else if(hash->data[index]->key == ip){
		// add only new data in the queue
		QueueNode *new = malloc(sizeof(QueueNode));
		new->info = t_value;
		new->next = NULL;
		hash->data[index]->tail->next = new;
		hash->data[index]->tail = new;

	}
	else{
		// check if key is in the queue, and manage the conflict
		KeyAndValue *aux = hash->data[index]->next;
		while( aux->next != NULL ){
			if(aux->key == ip){
				QueueNode *new = malloc(sizeof(QueueNode));
				new->info = t_value;
				new->next = NULL;
				aux->tail->next = new;
				return;
			}
			aux = aux->next;
		}
		if(aux->key == ip){
				QueueNode *new = malloc(sizeof(QueueNode));
				new->info = t_value;
				new->next = NULL;
				aux->tail->next = new;
				return;
		}

		QueueNode *new = malloc(sizeof(QueueNode));
		new->info = t_value;
		new->next = NULL;
		KeyAndValue newKey;
		newKey.key = ip;
		newKey.head = new;
		newKey.tail = new;
		newKey.next = NULL;
		aux->next = &newKey;
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
