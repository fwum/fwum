#ifndef HASHMAP_H_
#define HASHMAP_H_
#include "linked_list.h"
#include <stdbool.h>

DEFSTRUCT(hash_entry);
DEFSTRUCT(hash_map);

#define HASHMAP_ENTRY_LENGTH 1024

struct hash_entry {
	int hash;
	void *key, *value;
};
struct hash_map {
	linked_list entries[HASHMAP_ENTRY_LENGTH];
	bool (*eq)(void *a, void *b);
};

hash_map *hm_new();
hash_map *hm_new_eqfunc(bool (*eq)(void*, void*));
void hm_put(hash_map *map, int hash, void *key, void *value);
void *hm_get(hash_map *map, int hash, void *key);
bool hm_has(hash_map *map, int hash, void *key);
#endif
