#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H
#include <stdio.h>

#define MAX_LAST_NAME_SIZE 16
#define TABLE_SIZE 65536

/* hash version */
typedef struct __PHONE_BOOK_ENTRY {
    int lastNameValue;
    char lastName[MAX_LAST_NAME_SIZE];
    char firstName[16];
    char email[16];
    char phone[10];
    char cell[10];
    char addr1[16];
    char addr2[16];
    char city[16];
    char state[2];
    char zip[5];
    struct __PHONE_BOOK_ENTRY *pNext;
} entry;

typedef struct __PHONE_BOOK_HASHTABLE {
    int size;
    entry **storage;
} HashTable;

typedef struct _thread_data_t {
    int *key;
    FILE *fp;
    HashTable *ht;
} thread_data_t;
HashTable *createHashTable(int size);
void freeHashTable(HashTable *ht);
int *getHashKey(char *s, int size, int *key);
char *valuetoName(int val, char *result);

entry *findName(HashTable *ht, char lastname[]);
void *append_thread(void *arguments);

#endif
