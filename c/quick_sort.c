#include <assert.h>
#include <stdio.h>


int partition(int *arr, int low, int high) {
  int pivotidx = (low + high) / 2;
  int pivotval = arr[pivotidx];
  arr[pivotidx] = arr[high];
  arr[high] = pivotval;

  int swap = low - 1;
  for (int i = low; i < high; i++) {
    if (arr[i] <= pivotval) {
      swap++;
      int tmp = arr[i];
      arr[i] = arr[swap];
      arr[swap] = tmp;
    }
  }

  swap++;
  arr[high] = arr[swap];
  arr[swap] = pivotval;

  return swap;
}

void qs(int *arr, int low, int high) {
  if (low >= high) {
    return;
  }

  int idx = partition(arr, low, high);
  qs(arr, low, idx - 1);
  qs(arr, idx + 1, high);
}

void quick_sort(int *arr, int n) {
  qs(arr, 0, n - 1);
}

int main() {
  int arr[] = {9, 3, 7, 4, 69, 420, 42};
  int truth[] = {3, 4, 7, 9, 42, 69, 420};
  int n = 7;

  quick_sort(arr, n);
  for (int i = 0; i < n; i++) {
    assert(arr[i] == truth[i]);
  }

  return 0;
}

