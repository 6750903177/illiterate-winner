#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


void merge(int *arr, int p, int q, int r) {
  int n1 = q - p;
  int n2 = r - q;

  int *left = malloc(sizeof(int) * n1);
  for (int i = 0; i < n1; i++) {
    left[i] = arr[p + i];
  }

  int *right = malloc(sizeof(int) * n2);
  for (int i = 0; i < n2; i++) {
    right[i] = arr[q + i];
  }

  int i = 0;
  int j = 0;
  for (int k = p; k < r; k++) {
    if (i < n1 && j < n2) {
      arr[k] = (left[i] <= right[j]) ? left[i++] : right[j++];
    } else if (i < n1) {
      arr[k] = left[i++];
    } else {
      arr[k] = right[j++];
    }
  }

  free(left);
  free(right);
}

void ms(int *arr, int low, int high) {
  int mid = (low + high) / 2;
  if (low < high && low != mid) {
    ms(arr, low, mid);
    ms(arr, mid, high);
    merge(arr, low, mid, high);
  }
}

void merge_sort(int *arr, int n) {
  ms(arr, 0, n);
}

int main() {
  int arr[] = {9, 3, 7, 4, 69, 420, 42};
  int truth[] = {3, 4, 7, 9, 42, 69, 420};
  int n = 7;

  merge_sort(arr, n);
  for (int i = 0; i < n; i++) {
    assert(arr[i] == truth[i]);
  }

  return 0;
}
