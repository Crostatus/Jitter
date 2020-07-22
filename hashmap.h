#ifndef HASH_MAP
#define HASH_MAP
    #include <stdlib.h> //size_t, NULL
    #include <time.h>

    struct Q{
        struct timespec info;
        struct Q *next;
    };

    typedef struct Q QueueNode;

    struct KAV{
		char *key;
        QueueNode *head;
        QueueNode *tail;
        struct KAV *next;
	};

  typedef struct KAV KeyAndValue;

	typedef struct {
		KeyAndValue **data;
		int size;
        int a;
        int b;
	} HashMap;

    int HashCode(HashMap *hash, char *key);
    HashMap HashCreate();
    void HashAdd(HashMap *array, char *key, struct timespec t_value);
    void HashPrint(HashMap hash);
    void HashAppend(HashMap *array,HashMap *append);
    int HashFreeIndex(HashMap *array,int index);
    void HashFreeAll(HashMap *array);
    /*KeyAndValue *HashGetIndex(HashMap *array,int index);
    KeyAndValue *HashGetWithKey(HashMap *array,void *key);
    void *HashGetKey(HashMap *array,void *value);
    KeyAndValue *HashGetWithValue(HashMap *array,void *value);
    void *HashGetValue(HashMap *array,void *key);*/


#endif
