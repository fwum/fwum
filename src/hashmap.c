#include "hashmap.h"
#include <stdbool.h>
#include "linked_list.h"
#include "util.h"

static int get_hash(int key);
static bool void_ptr_equals(void *a, void *b);

static bool void_ptr_equals(void *a, void *b) {
	return a == b;
}

hash_map *hm_new() {
	hash_map *map = new(map);
	map->eq = &void_ptr_equals;
	for(int i = 0; i < HASHMAP_ENTRY_LENGTH; i++)
		map->entries[i].head = map->entries[i].tail = NULL;
	return map;
}

hash_map *hm_new_eqfunc(bool (*eq)(void*, void*)) {
	hash_map *map = hm_new();
	map->eq = eq;
	return map;
}

void hm_put(hash_map *map, int hash, void *key, void *value) {
	int index = get_hash(hash);
	hash_entry *entry = new(entry);
	entry->hash = hash;
	entry->key = key;
	entry->value = value;
	ll_add_last(&(map->entries[index]), entry);
}

void *hm_get(hash_map *map, int hash, void *key) {
	int index = get_hash(hash);
	linked_list data = map->entries[index];
	linked_iter iterator = ll_iter_head(&data);
	while(ll_iter_has_next(&iterator)) {
		hash_entry *entry = ll_iter_next(&iterator);
		if(entry->hash == hash && map->eq(entry->key, key)) {
			return entry->value;
		}
	}
	return NULL;
}

bool hm_has(hash_map *map, int hash, void *key){
	return hm_get(map, hash, key) != NULL;
}

static int get_hash(int key) {
	return abs(key) % HASHMAP_ENTRY_LENGTH;
}
