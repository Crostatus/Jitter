#include "hashmap.h"
#include "time.h"
#define P 999149

// create hashmap with size of 10
HashMap HashCreate() {
	HashMap hash;
	hash.size = 50;
	hash.data = malloc(sizeof(KeyAndValue*) * hash->size);
	return hash;
}

// function hash
int HashCode(HashMap *hash, void* key) {
   return (((key*hash->a) + hash->b) % P) % hash->size;
}

// add the new value in the HashMap
void HashAdd(HashMap *hash,void *key, void *t_value) {
	int index = HashCode(hash , key);
	if(hash->data[index]->key == NULL){
		QueueNode *new = malloc(sizeof(QueueNode));
		new->info = t_value;
		new->next = NULL;
		KeyAndValue newKey;
		newKey.key = key;
		newKey.head = new;
		newKey.tail = new;
		newKey.next = NULL;
		hash->data[index] = &newKey;
	}
	else if(hash->data[index]->key == key){
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
			if(aux->key == key){
				QueueNode *new = malloc(sizeof(QueueNode));
				new->info = t_value;
				new->next = NULL;
				aux->tail->next = new;
				return;
			}
			aux = aux->next;
		}
		if(aux->key == key){
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
		newKey.key = key;
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

void HashPrint(HashMap hash){
	printf("\n\n		Valori nella tabella Hash:\n")
	for(int i=0; i<hash.size; i++){
		KeyAndValue *item = hash->data[i];
		while( item != NULL){
			QueueNode aux = item.head;
			printf("	key: %s ---->\n", item.key);
			while( aux != NULL){
				printf("value: %ld\n", aux->info);
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
