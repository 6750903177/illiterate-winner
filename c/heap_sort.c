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
  if (h->backing[idx] > h->backing[p]) {
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
    c = (h->backing[lc] > h->backing[rc]) ? lc : rc;
  }
  
  if (h->backing[idx] < h->backing[c]) {
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

void heap_from_array(heap_t *h, int *arr, int n) {
  h->length = n;
  h->capacity = n;
  h->backing = arr;

  for (int i = h->length / 2; i >= 0; i--) {
    _heapify_down(h, i);
  }
}

void hs(heap_t *h) {
  int l = h->length;
  for (int i = l - 1; i >= 0; i--) {
    int tmp = h->backing[i];
    h->backing[i] = h->backing[0];
    h->backing[0] = tmp;

    h->length--;
    _heapify_down(h, 0);
  }
}

void heap_sort(int *arr, int n) {
  heap_t *h = malloc(sizeof(heap_t));
  heap_from_array(h, arr, n);
  hs(h);
  free(h);
  return;
}

int main() {
  int arr[] = {9, 3, 7, 4, 69, 420, 42};
  int truth[] = {3, 4, 7, 9, 42, 69, 420};
  int n = 7;

  heap_sort(arr, n);
  for (int i = 0; i < n; i++) {
    assert(arr[i] == truth[i]);
  }

  return 0;
}
