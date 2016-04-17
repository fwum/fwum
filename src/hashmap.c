#include "hashmap.h"
#include <stdbool.h>
#include "linked_list.h"
#include "util.h"

static int hash(int key);

hash_map *hm_new()
{
	hash_map *map = new(map);
	for(int i = 0; i < HASHMAP_ENTRY_LENGTH; i++)
		map[i] = {NULL, NULL};
	return map;
}

void hm_put(hash_map *map, int key, void *value)
{
	int index = hash(key);
	ll_add_last(map[index], value);
}

void *hm_get(hash_map *map, int key)
{
	int index = hash(key);
	linked_list data = map[index];
	//TODO: Use an iterator to find the key
	return NULL;
}

bool hm_has(hash_map *map, int key)
{
	//TODO: use an iterator to find the key
	return false;
}

static int hash(int key) 
{
	return key % HASHMAP_ENTRY_LENGTH;
}