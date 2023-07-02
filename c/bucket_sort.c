#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct buckets {
  int n;
  int c;
  float **buckets;
} buckets_t;

void buckets_init(buckets_t *b, int n, int c) {
  b->n = n;
  b->c = c;
  b->buckets = malloc(sizeof(float*) * n);
  for (int i = 0; i < n; i++) {
    b->buckets[i] = malloc(sizeof(float) * c);
    for (int j = 0; j < c; j++) {
      b->buckets[i][j] = -1.0;
    }
  }
}

void buckets_deinit(buckets_t *b) {
  for (int i = 0; i < b->n; i++) {
    free(b->buckets[i]);
  }
  free(b->buckets);
  free(b);
}

void buckets_insert(buckets_t *b, int n, float v) {
  for (int i = 0; i < b->c; i++) {
    if (b->buckets[n][i] == -1.0) {
      b->buckets[n][i] = v;
      break;
    }
  }
}

void bucket_sort(float *arr, int n) {
  buckets_t *b = malloc(sizeof(buckets_t));
  buckets_init(b, n, n);

  for (int i = 0; i < n; i++) {
    int idx = (int)((float) n * arr[i]);
    buckets_insert(b, idx, arr[i]);
  }

  for (int i = 0; i < n; i++) {
    for (int j = 1; j < b->c; j++) {
      float curr = b->buckets[i][j];
      int k = j - 1;
      while(k >= 0 && b->buckets[i][k] > curr) {
        b->buckets[i][k + 1] = b->buckets[i][k];
        k--;
      }
      b->buckets[i][k + 1] = curr;
    }
  }

  int idx = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      float v = b->buckets[i][j];
      if (v != -1.0) {
        arr[idx++] = v;
      }
    }
  }

  buckets_deinit(b);
}

int main() {
  float arr[] = {0.9, 0.3, 0.7, 0.4, 0.69, 0.420, 0.421};
  float truth[] = {0.3, 0.4, 0.42, 0.421, 0.69, 0.7, 0.9};
  int n = 7;

  bucket_sort(arr, n);
  for (int i = 0; i < n; i++) {
    assert(arr[i] == truth[i]);
  }

  return 0;
}


