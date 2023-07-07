#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


uint64_t fnv_1a_64bit(const char *key, int n) {
  static uint64_t FNV_OFFSET = 0xcbf29ce484222325;
  static uint64_t FNV_PRIME = 0x00000100000001b3;

  uint64_t hash = FNV_OFFSET;
  for (int i = 0; i < n; i++) {
    hash ^= (uint64_t)(uint8_t)key[i];
    hash *= FNV_PRIME;
  }

  return hash;
}


typedef struct chain {
  const char *key;
  int value;
  struct chain *prev;
  struct chain *next;
} chain_t;

chain_t* chain_init(const char *key, int value) {
  chain_t *chain = malloc(sizeof(chain_t));
  if (chain == NULL) {
    return NULL;
  }

  chain->key = key;
  chain->value = value;
  chain->prev = NULL;
  chain->next = NULL;

  return chain;
}


typedef struct list {
  int length;
  chain_t *head;
  chain_t *tail;
} list_t;

list_t* list_init() {
  list_t *list = malloc(sizeof(list_t));
  if (list == NULL) {
    return NULL;
  }

  list->length = 0;
  list->head = NULL;
  list->tail = NULL;

  return list;
}

void list_deinit(list_t *l) {
  while (l->head != NULL) {
    chain_t *curr = l->head;
    l->head = curr->next;
    free(curr);
  }
  free(l);
}

chain_t* list_push_front(list_t *l, chain_t *node) {
  if (node == NULL) {
    return NULL;
  }

  l->length++;
  if (l->head != NULL) {
    l->head->prev = node;
    node->next = l->head;
    l->head = node;
  } else {
    l->head = node;
    l->tail = node;
  }

  return node;
}

chain_t* list_delete(list_t *l, const char *key) {
  if (l->length == 0) {
    return NULL;
  }

  chain_t *found = NULL;
  chain_t *curr = l->head;
  while (curr != NULL) {
    if (strcmp(curr->key, key) == 0) {
      if (curr->prev != NULL) {
        curr->prev->next = curr->next;
      } else {
        l->head = curr->next;
      }

      if (curr->next != NULL) {
        curr->next->prev = curr->prev;
      } else {
        l->tail = curr->prev;
      }

      found = curr;
      found->prev = NULL;
      found->next = NULL;
      break;
    }

    curr = curr->next;
  }

  return found;
}

chain_t* list_get(list_t *l, const char *key) {
  if (l->length == 0) {
    return NULL;
  }

  chain_t *found = NULL;
  chain_t *curr = l->head;
  while (curr != NULL) {
    if (strcmp(curr->key, key) == 0) {
      found = curr;
      break;
    }

    curr = curr->next;
  }

  return found;
}


typedef struct map {
  int length;
  int capacity;
  uint64_t (*hash)(const char*, int);
  list_t **buckets;
} map_t;

map_t* map_init(int capacity) {
  map_t *map = malloc(sizeof(map_t));
  if (map == NULL) {
    return NULL;
  }

  map->length = 0;
  map->capacity = capacity;
  map->hash = fnv_1a_64bit;
  if ((map->buckets = malloc(sizeof(list_t*) * capacity)) == NULL) {
    free(map);
    return NULL;
  }

  for (int i = 0; i < capacity; i++) {
    if ((map->buckets[i] = list_init()) == NULL) {
      free(map->buckets);
      free(map);
      return NULL;
    }
  }

  return map;
}

void map_deinit(map_t *m) {
  for (int i = 0; i < m->capacity; i++) {
    list_deinit(m->buckets[i]);
  }
  free(m->buckets);
  free(m);
}

const char* map_add(map_t *m, const char *key, int value) {
  chain_t *node = chain_init(key, value);
  if (node == NULL) {
    return NULL;
  }

  int index = m->hash(key, strlen(key)) % m->capacity;
  if ((node = list_push_front(m->buckets[index], node)) == NULL) {
    return NULL;
  }

  m->length++;
  return key;
}

int* map_get(map_t *m, const char *key) {
  int index = m->hash(key, strlen(key)) % m->capacity;

  chain_t *found = list_get(m->buckets[index], key);
  if (found == NULL) {
    return NULL;
  }

  return &found->value;
}

bool map_delete(map_t *m, const char *key) {
  int index = m->hash(key, strlen(key)) % m->capacity;

  chain_t *found = list_delete(m->buckets[index], key);
  if (found == NULL) {
    return false;
  }

  m->length--;
  free(found);

  return true;
}


int main() {
  map_t *m = map_init(16);
  assert(m != NULL);

  assert(map_add(m, "foo", 55) != NULL);
  assert(m->length == 1);
  assert(map_add(m, "fool", 75) != NULL);
  assert(m->length == 2);
  assert(map_add(m, "foolish", 105) != NULL);
  assert(m->length == 3);
  assert(map_add(m, "bar", 69) != NULL);
  assert(m->length == 4);

  assert( map_get(m, "bar") != NULL);
  assert(*map_get(m, "bar") == 69);
  assert( map_get(m, "bax") == NULL);

  assert(!map_delete(m, "qux"));
  assert(m->length == 4);

  assert( map_delete(m, "bar"));
  assert(m->length == 3);
  assert( map_get(m, "bar") == NULL);

  map_deinit(m);
  return 0;
}
