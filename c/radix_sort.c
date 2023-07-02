#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void radix_sort(int *arr, int n, int d, int k) {
  int modulus = k;
  int divisor = 1;

  int *sorted = malloc(sizeof(int) * n);
  for (int i = 0; i < n; i++) {
    sorted[i] = 0;
  }

  for (int i = 0; i < d; i++) {
    int *counts = malloc(sizeof(int) * k);
    for (int i = 0; i < k; i++) {
      counts[i] = 0;
    }

    for (int i = 0; i < n; i++) {
      counts[(arr[i] / divisor) % modulus]++;
    }

    for (int i = 1; i < k; i++) {
      counts[i] = counts[i] + counts[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
      sorted[--counts[(arr[i] / divisor) % modulus]] = arr[i];
    }

    for (int i = 0; i < n; i++) {
      arr[i] = sorted[i];
    }

    divisor = modulus;
    modulus *= k;
    free(counts);
  }

  free(sorted);
}

int main() {
  int arr[] = {9, 3, 7, 4, 69, 420, 42};
  int truth[] = {3, 4, 7, 9, 42, 69, 420};
  int n = 7;
  int d = 3;
  int k = 420;

  radix_sort(arr, n, d, k);
  for (int i = 0; i < n; i++) {
    assert(arr[i] == truth[i]);
  }

  return 0;
}
