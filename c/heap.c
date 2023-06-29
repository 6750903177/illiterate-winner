#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct heap {
  int length;
  int capacity;
  int *backing;
} heap_t;

int heap_init(heap_t *h, int capacity) {
  h->length = 0;
  h->capacity = capacity;
  if ((h->backing = malloc(sizeof(int) * capacity)) == NULL) {
    return 1;
  }

  return 0;
}

void heap_deinit(heap_t *h) {
  free(h->backing);
  free(h);
}

void heap_print(heap_t *h) {
  for (int i = 0; i < h->length; i++) {
    printf("%d --> %d\n", i, h->backing[i]);
  }
}

int heap_peak(heap_t *h) {
  return h->backing[0];
}

int _resize(heap_t *h) {
  h->capacity *= 2;
  h->backing = realloc(h->backing, sizeof(int) * h->capacity);
  if (h->backing == NULL) {
    return 1;
  }
  return 0;
}

int _get_left_child_idx(int idx) {
  return 2 * idx + 1;
}

int _get_right_child_idx(int idx) {
  return 2 * idx + 2;
}

int _get_parent_idx(int idx) {
  return (idx - 1) / 2;
}

void _heapify_up(heap_t *h, int idx) {
  if (idx == 0) {
    return;
  }

  int p = _get_parent_idx(idx);
  if (h->backing[idx] < h->backing[p]) {
    int tmp = h->backing[idx];
    h->backing[idx] = h->backing[p];
    h->backing[p] = tmp;
    _heapify_up(h, p);
  }
}

void _heapify_down(heap_t *h, int idx) {
  int lc = _get_left_child_idx(idx);
  int rc = _get_right_child_idx(idx);

  int c;
  if (lc >= h->length) {
    return;
  } else if (rc >= h->length) {
    c = lc;
  } else {
    c = (h->backing[lc] < h->backing[rc]) ? lc : rc;
  }
  
  if (h->backing[idx] > h->backing[c]) {
    int tmp = h->backing[idx];
    h->backing[idx] = h->backing[c];
    h->backing[c] = tmp;
    _heapify_down(h, c);
  }
}

int heap_push(heap_t *h, int value) {
  if (h->length >= h->capacity) {
    if (_resize(h) != 0) {
      return 1;
    }
  }

  h->backing[h->length] = value;
  _heapify_up(h, h->length++);
  return 0;
}

int heap_pop(heap_t *h, int *value) {
  if (h->length == 0) {
    return 1;
  }

  *value = h->backing[0];
  h->backing[0] = h->backing[--h->length];

  _heapify_down(h, 0);
  return 0;
}

int main() {
  heap_t *h = malloc(sizeof(heap_t));
  heap_init(h, 5);

  assert(h->length == 0);

  assert(heap_push(h, 5) == 0);
  assert(heap_push(h, 3) == 0);
  assert(heap_push(h, 69) == 0);
  assert(heap_push(h, 420) == 0);
  assert(heap_push(h, 4) == 0);
  assert(heap_push(h, 1) == 0);
  assert(heap_push(h, 8) == 0);
  assert(heap_push(h, 7) == 0);

  int value;
  assert(h->length == 8);
  assert(heap_pop(h, &value) == 0);
  assert(value == 1);
  assert(heap_pop(h, &value) == 0);
  assert(value == 3);
  assert(heap_pop(h, &value) == 0);
  assert(value == 4);
  assert(heap_pop(h, &value) == 0);
  assert(value == 5);
  assert(h->length == 4);
  assert(heap_pop(h, &value) == 0);
  assert(value == 7);
  assert(heap_pop(h, &value) == 0);
  assert(value == 8);
  assert(heap_pop(h, &value) == 0);
  assert(value == 69);
  assert(heap_pop(h, &value) == 0);
  assert(value == 420);
  assert(h->length == 0);

  heap_deinit(h);
  return 0;
}
