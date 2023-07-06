#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct chain {
  const char *key;
  int value;
  struct chain *prev;
  struct chain *next;
} chain_t;

void chain_init(chain_t *c, const char *key, int value) {
  c->key = key;
  c->value = value;
  c->prev = NULL;
  c->next = NULL;
}

typedef struct doubly_linked_list {
  chain_t *head;
  chain_t *tail;
  int length;
} list_t;

void dll_init(list_t *l) {
  l->head = NULL;
  l->tail = NULL;
  l->length = 0;
}

void dll_deinit(list_t *l) {
  while (l->head != NULL) {
    l->length--;
    chain_t *curr = l->head;
    l->head = curr->next;
    free(curr);
  }
  free(l);
}

int dll_add(list_t *l, const char *key, int value) {
  chain_t *link = NULL;
  if ((link = malloc(sizeof(chain_t))) == NULL) {
    return 1;
  }

  chain_init(link, key, value);

  l->length++;
  if (l->head == NULL) {
    l->head = link;
    l->tail = link;
  } else {
    link->next = l->head;
    l->head->prev = link;
    l->head = link;
  }

  return 0;
}

bool dll_get(list_t *l, const char *key, int *value) {
  if (l->length == 0) {
    return false;
  }

  chain_t *curr = l->head;
  for (int i = 0; i < l->length; i++) {
    if (strcmp(curr->key, key) == 0) {
      *value = curr->value;
      return true;
    }
    curr = curr->next;
  }

  return false;
}

bool dll_search(list_t *l, const char *key) {
  if (l->length == 0) {
    return false;
  }

  chain_t *curr = l->head;
  for (int i = 0; i < l->length; i++) {
    if (strcmp(curr->key, key) == 0) {
      return true;
    }
    curr = curr->next;
  }

  return false;
}

bool dll_delete(list_t *l, const char *key) {
  if (l->length == 0) {
    return false;
  }

  chain_t *curr = l->head;
  for (int i = 0; i < l->length; i++) {
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

      l->length--;
      free(curr);
      return true;
    }
    curr = curr->next;
  }

  return false;
}








typedef struct map {
  int n;
  int m;
  list_t **slots;
  int (*hash)(const char *, int);
} map_t;

int map_init(map_t *m, int capacity, int (*hash)(const char *, int)) {
  m->n = 0;
  m->m = capacity;
  m->hash = hash;

  if ((m->slots = malloc(sizeof(list_t*) * capacity)) == NULL) {
    return 1;
  }

  for (int i = 0; i < capacity; i++) {
    if ((m->slots[i] = malloc(sizeof(list_t))) == NULL) {
      return 1;
    }
    dll_init(m->slots[i]);
  }

  return 0;
}

void map_deinit(map_t *m) {
  for (int i = 0; i < m->m; i++) {
    dll_deinit(m->slots[i]);
  }
  free(m->slots);
  free(m);
}

int map_add(map_t *m, const char *key, int value) {
  int slot = m->hash(key, m->m);
  if (dll_add(m->slots[slot], key, value) != 0) {
    return 1;
  }
  m->n++;
  return 0;
}

bool map_get(map_t *m, const char *key, int *value) {
  int slot = m->hash(key, m->m);
  return dll_get(m->slots[slot], key, value);
}

bool map_search(map_t *m, const char *key) {
  int slot = m->hash(key, m->m);
  return dll_search(m->slots[slot], key);
}

bool map_delete(map_t *m, const char *key) {
  int slot = m->hash(key, m->m);
  if (!dll_delete(m->slots[slot], key)) {
    return false;
  }
  m->n--;
  return true;
}






int _radix_128(const char *key) {
  int output = 0;
  int n = strlen(key);
  int scale = 1;

  for (int i = 0; i < n; i++) {
    output += key[n - i - 1] * scale;
    scale *= 128;
  }

  return output;
}

int division_hash(const char *key, int m) {
  return _radix_128(key) % m;
}

int multiplication_hash(const char *key, int m) {
  double scale = (sqrt(5) - 1.0) / 2.0;
  double prod = (double) _radix_128(key) * scale;
  return  (int) ((double) m * (prod - floor(prod)));
}

int main() {
  map_t *m = malloc(sizeof(map_t));
  map_init(m, 5, multiplication_hash);

  assert(map_add(m, "foo", 55) == 0);
  assert(m->n == 1);
  assert(map_add(m, "fool", 75) == 0);
  assert(m->n == 2);
  assert(map_add(m, "foolish", 105) == 0);
  assert(m->n == 3);
  assert(map_add(m, "bar", 69) == 0);
  assert(m->n == 4);

  int value = 0;
  assert( map_get(m, "bar", &value));
  assert(value == 69);
  assert(!map_get(m, "baz", &value));

  assert(!map_delete(m, "qux"));
  assert(m->n == 4);

  assert( map_delete(m, "bar"));
  assert(m->n == 3);
  assert(!map_get(m, "bar", &value));

  map_deinit(m);
  return 0;
}
