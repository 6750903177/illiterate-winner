#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct array_list {
  int length;
  int capacity;
  int *backing;
} array_list_t;

int list_init(array_list_t *l, int capacity) {
  if (l == NULL) {
    return 1;
  }

  l->length = 0;
  l->capacity = capacity;
  l->backing = malloc(sizeof(int) * capacity);

  return 0;
}

void list_deinit(array_list_t *l) {
  free(l->backing);
  free(l);
}

void list_print(array_list_t *l) {
  for (int i = 0; i < l->length; i++) {
    printf("INDEX %d: VALUE=%d\n", i, l->backing[i]);
  }
}

int list_insert_at(array_list_t *l, int idx, int value) {
  if (l->length >= l->capacity) {
    l->capacity *= 2;
    l->backing = realloc(l->backing, sizeof(int) * l->capacity);
    if (l->backing == NULL) {
      return 1;
    }
    return list_insert_at(l, idx, value);
  }

  if (idx >= l->length + 1 || idx < 0) {
    return 1;
  }

  for (int i = l->length; i > idx; i--) {
    l->backing[i] = l->backing[i - 1];
  }
  l->backing[idx] = value;
  l->length++;

  return 0;
}

int list_remove_at(array_list_t *l, int idx, int *value) {
  if (idx >= l->length || idx < 0) {
    return 1;
  }

  *value = l->backing[idx];
  for (int i = idx + 1; i < l->length; i++) {
    l->backing[i - 1] = l->backing[i];
  }
  l->length--;

  return 0;
}

int list_remove(array_list_t *l, int tgt, int *value) {
  int found = -1;
  for (int i = 0; i < l->length; i++) {
    if (l->backing[i] == tgt) {
      found = i;
      break;
    }
  }

  if (found != -1) {
    return list_remove_at(l, found, value);
  }

  return 1;
}

int list_push(array_list_t *l, int value) {
  return list_insert_at(l, l->length, value);
}

int list_pop(array_list_t *l, int *value) {
  return list_remove_at(l, l->length - 1, value);
}

int list_enqueue(array_list_t *l, int value) {
  return list_insert_at(l, 0, value);
}

int list_deque(array_list_t *l, int *value) {
  return list_remove_at(l, 0, value);
}

int list_get(array_list_t *l, int idx, int *value) {
  if (idx >= l->length || idx < 0) {
    return 1;
  }

  *value = l->backing[idx];
  return 0;
}

int main() {
  array_list_t *l = malloc(sizeof(array_list_t));
  assert((list_init(l, 10)) == 0);

  assert(list_push(l, 5) == 0);
  assert(list_push(l, 7) == 0);
  assert(list_push(l, 9) == 0);

  int value;
  assert((list_get(l, 0, &value)) == 0);
  assert((list_get(l, 1, &value)) == 0);
  assert((list_get(l, 2, &value)) == 0);
  assert(value == 9);

  assert((list_remove_at(l, 1, &value)) == 0);
  assert(value == 7);
  assert(l->length == 2);

  assert(list_push(l, 11) == 0);
  assert((list_remove_at(l, 1, &value)) == 0);
  assert(value == 9);
  assert((list_remove(l, 9, &value)) == 1);
  assert((list_remove_at(l, 0, &value)) == 0);
  assert(value == 5);
  assert((list_remove_at(l, 0, &value)) == 0);
  assert(value == 11);
  assert(l->length == 0);

  assert(list_enqueue(l, 5) == 0);
  assert(list_enqueue(l, 7) == 0);
  assert(list_enqueue(l, 9) == 0);

  assert((list_get(l, 2, &value)) == 0);
  assert(value == 5);
  assert((list_get(l, 0, &value)) == 0);
  assert(value == 9);
  assert((list_remove(l, 9, &value)) == 0);
  assert(value == 9);
  assert(l->length == 2);
  assert((list_get(l, 0, &value)) == 0);
  assert(value == 7);

  list_insert_at(l, 1, 1);
  assert((list_get(l, 1, &value)) == 0);
  assert(value == 1);
  list_deinit(l);
}
