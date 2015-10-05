#include <stdlib.h>
#include <string.h>
#include <math.h>

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

int *getHashKey(char *s, int size, int *key)
{
	if (!s) return NULL;
	int hash, i, val;
	for (hash = 0, i = 0, val = 1; *s != '\0'; i++, s++) {
		hash = (hash<<4) ^ (hash>>28) ^ *s;
		val = val * 31 + (*s - 'a');
	}

	/* index 0 for the key
	 * index 1 for the value
	 * so we don't need strcpy */

	key[0] = abs(hash % size);
	key[1] = val;
	return key;
}

char *valuetoName(int val, char *Name)
{
	int c, i = 0;
	while (val != 1) {
		c = val % 31;
		val = (val - c) / 31;
		Name[i++] = c + 'a';
	}
	Name[i] = '\0';
	int j = 0, temp;
	for (j = 0, --i; i > j; i--, j++) {
		temp = Name[i];
		Name[i] = Name[j];
		Name[j] = temp;
	}
	return Name;
}

entry *findName(HashTable *ht, char lastName[])
{
	int *key = malloc(sizeof(int) * 2);
	key = getHashKey(lastName, ht->size, key);
	if (key[0] == -1) 
		return NULL;
	entry *e;
	if ((e = ht->storage[key[0]])) {
		do {
			if (key[1] == e->lastNameValue) {
				free(key);
				return e;
			} 
			e = e->pNext;
		} while (e);
	}
	return NULL;
}

entry *append(HashTable *ht, char lastName[], int *key)
{
	key = getHashKey(lastName, ht->size, key);
	if (key[0] == -1) 
		return NULL;
	entry *e;
	if (ht->storage[key[0]]) {
		e = malloc(sizeof(entry));
		e->lastNameValue = key[1];
		e->pNext = ht->storage[key[0]]->pNext;
		ht->storage[key[0]] = e;
		return e;
	} else {
		e = malloc(sizeof(entry));
		e->lastNameValue = key[1];
		e->pNext = NULL;
		ht->storage[key[0]] = e;
		return e;
	}
}
