#include "hashmap.h"
#include <stdbool.h>
#include "linked_list.h"
#include "util.h"

static int hash(int key);

hash_map *hm_new() {
	hash_map *map = new(map);
	for(int i = 0; i < HASHMAP_ENTRY_LENGTH; i++)
		map->entries[i].head = map->entries[i].tail = NULL;
	return map;
}

void hm_put(hash_map *map, int key, void *value) {
	int index = hash(key);
	hash_entry *entry = new(entry);
	entry->key = key;
	entry->value = value;
	ll_add_last(&(map->entries[index]), entry);
}

void *hm_get(hash_map *map, int key) {
	int index = hash(key);
	linked_list data = map->entries[index];
	linked_iter iterator = ll_iter_head(&data);
	while(ll_iter_has_next(&iterator)) {
		hash_entry *entry = ll_iter_next(&iterator);
		if(entry->key == key) {
			return entry->value;
		}
	}
	return NULL;
}

bool hm_has(hash_map *map, int key) {
	return hm_get(map, key) != NULL;
}

static int hash(int key) {
	return key % HASHMAP_ENTRY_LENGTH;
}
