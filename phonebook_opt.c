#include <stdlib.h>
#include <string.h>

#include "phonebook_opt.h"

HashTable *createHashTable(int size)
{
	if (size < 1) return NULL;
	HashTable *ht = malloc(sizeof(HashTable));
	ht->size = size;
	ht->storage = calloc(size, sizeof(entry *));
	return ht;
}

void freeHashTable(HashTable *ht)
{
	int i = 0;
	for (i = 0; i < ht->size; i++) {
		entry *cur;
		if ((cur = ht->storage[i])) {
			do {
				entry *e = cur;
				cur = e->pNext;
				free(e);
			} while (cur);
		}
	}
	free(ht->storage);
	free(ht);
}

int getHashKey(char *s, int size)
{
	if (!s) return -1;
	int hash = 0;
	while(*s) {
		hash += *s++ * 17;
	}
	return hash % size;
}

entry *findName(HashTable *ht, char lastName[])
{
    int key;
	if ((key = getHashKey(lastName, ht->size)) == -1) return NULL;
	entry *e;
	if ((e = ht->storage[key])) {
		do {
			if (strcasecmp(lastName, e->lastName) == 0) return e;
			e = e->pNext;
		} while (e);
	}
	return NULL;
}

entry *append(HashTable *ht, char lastName[])
{
	int key;
	if ((key = getHashKey(lastName, ht->size)) == -1) return NULL;
	entry *e;
	if (ht->storage[key]) {
		entry *cur = ht->storage[key];
		while (cur->pNext) cur = cur->pNext;
		e = malloc(sizeof(entry));
		strcpy(e->lastName, lastName);
		e->pNext = NULL;
		cur->pNext = e;
		return e;
	} else {
		e = malloc(sizeof(entry));
		strcpy(e->lastName, lastName);
		e->pNext = NULL;
		ht->storage[key] = e;
		return e;
	}
}
