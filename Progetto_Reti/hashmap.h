#ifndef HASH_MAP
#define HASH_MAP
    #include <stdlib.h> //size_t, NULL
    #include <time.h>

    typedef struct{
        struct timespec info;
        struct QueueNode *next;
    } QueueNode;

    typedef struct {
		void *key;
        QueueNode *head;
        QueueNode *tail;
        KeyAndValue *next;
	} KeyAndValue;

	typedef struct {
		KeyAndValue **data;
		int size;
        int a;
        int b;
	} HashMap;

    int hashCode(HashMap *hash, int key);
    void HashCreate(HashMap *array);
    void HashAdd(HashMap *array,KeyAndValue *value);
    void HashAppend(HashMap *array,HashMap *append);
    int HashFreeIndex(HashMap *array,int index);
    void HashFreeAll(HashMap *array);
    /*KeyAndValue *HashGetIndex(HashMap *array,int index);
    KeyAndValue *HashGetWithKey(HashMap *array,void *key);
    void *HashGetKey(HashMap *array,void *value);
    KeyAndValue *HashGetWithValue(HashMap *array,void *value);
    void *HashGetValue(HashMap *array,void *key);*/


#endif
