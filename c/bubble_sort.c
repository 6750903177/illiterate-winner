#include <assert.h>
#include <stdio.h>

void bubble_sort(int *arr, int n) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = i + 1; j < n; j++) {
      if (arr[j] < arr[i]) {
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
      }
    }
  }
}

int main() {
  int arr[] = {9, 3, 7, 4, 69, 420, 42};
  int truth[] = {3, 4, 7, 9, 42, 69, 420};
  int n = 7;

  bubble_sort(arr, n);
  for (int i = 0; i < n; i++) {
    assert(arr[i] == truth[i]);
  }

  return 0;
}
