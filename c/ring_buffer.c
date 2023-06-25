#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int modulo(int a, int b) {
  int r = a % b;
  while (r < 0) {
    r += b;
  }
  return r;
}

typedef struct ring_buffer {
  int head;
  int length;
  int capacity;
  int *backing;
} ring_buffer_t;

int ring_init(ring_buffer_t *r, int capacity) {
  if (r == NULL) {
    return 1;
  }

  r->head = 0;
  r->length = 0;
  r->capacity = capacity;
  r->backing = malloc(sizeof(int) * capacity);
  if (r->backing == NULL) {
    return 1;
  }

  return 0;
}

void ring_deinit(ring_buffer_t *r) {
  free(r->backing);
  free(r);
}

void ring_print(ring_buffer_t *r) {
  int c = r->capacity;
  int h = r->head;
  for (int i = 0; i < r->length; i++) {
    printf("INDEX %d: VALUE=%d\n", i, r->backing[modulo(i + h, c)]);
  }
}

int ring_get(ring_buffer_t *r, int idx, int *value) {
  if (idx >= r->length || idx < 0) {
    return 1;
  }

  int i = modulo(idx + r->head, r->capacity);
  *value = r->backing[i];

  return 0;
}

int ring_resize(ring_buffer_t *r) {
  int c = r->capacity;
  r->capacity *= 2;
  int *update = malloc(sizeof(int) * r->capacity);
  if (update == NULL) {
    return 1;
  }
  for (int i = 0; i < r->length; i++) {
    update[i] = r->backing[modulo(i + r->head, c)];
  }
  free(r->backing);
  r->backing = update;
  r->head = 0;
  return 0;
}

int ring_push_front(ring_buffer_t *r, int value) {
  if (r->length >= r->capacity) {
    if ((ring_resize(r)) == 1) {
      return 1;
    }
  }

  r->head = modulo(r->head - 1, r->capacity);
  r->backing[r->head] = value;
  r->length++;

  return 0;
}

int ring_push_back(ring_buffer_t *r, int value) {
  if (r->length >= r->capacity) {
    if ((ring_resize(r)) == 1) {
      return 1;
    }
  }

  r->backing[modulo(r->head + r->length++, r->capacity)] = value;

  return 0;
}

int ring_pop_front(ring_buffer_t *r, int *value) {
  if (r->length == 0) {
    return 1;
  }

  *value = r->backing[r->head];
  r->head = modulo(r->head + 1, r->capacity);
  r->length--;

  return 0;
}

int ring_pop_back(ring_buffer_t *r, int *value) {
  if (r->length == 0) {
    return 1;
  }

  int i = modulo(r->head + --r->length, r->capacity);
  *value = r->backing[i];

  return 0;
}

int main() {
  ring_buffer_t *r = malloc(sizeof(ring_buffer_t));
  assert(ring_init(r, 5) == 0);

  for (int i = 0; i < 8; i++) {
    assert(ring_push_front(r, i) == 0);
    assert(ring_push_back(r, i) == 0);
  }

  int value;
  for (int i = 7; i >= 0; i--) {
    assert(ring_pop_front(r, &value) == 0);
    assert(value == i);
    assert(ring_pop_back(r, &value) == 0);
    assert(value == i);
  }

  ring_deinit(r);

  return 0;
}
