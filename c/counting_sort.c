#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


void counting_sort(int *arr, int n, int k) {
  int *sorted = malloc(sizeof(int) * n);
  for (int i = 0; i < n; i++) {
    sorted[i] = 0;
  }

  int *counts = malloc(sizeof(int) * (k + 1));
  for (int i = 0; i < k + 1; i++) {
    counts[i] = 0;
  }

  for (int i = 0; i < n; i++) {
    counts[arr[i]]++;
  }

  for (int i = 1; i < k + 1; i++) {
    counts[i] = counts[i] + counts[i - 1];
  }

  for (int i = n - 1; i >= 0; i--) {
    sorted[--counts[arr[i]]] = arr[i];
  }

  for (int i = 0; i < n; i++) {
    arr[i] = sorted[i];
  }
  free(counts);
  free(sorted);
}

int main() {
  int arr[] = {9, 3, 7, 4, 69, 420, 42};
  int truth[] = {3, 4, 7, 9, 42, 69, 420};
  int n = 7;
  int k = 420;

  counting_sort(arr, n, k);
  for (int i = 0; i < n; i++) {
    assert(arr[i] == truth[i]);
  }

  return 0;
}

