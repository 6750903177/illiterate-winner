#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

bool _search(int *haystack, int low, int high, int needle) {
  if (needle > haystack[high - 1] || needle < haystack[low]) {
    return false;
  }

  if (high == low) {
    return false;
  }

  int midpoint = (high + low) / 2;
  int midval = haystack[midpoint];

  if (midval == needle) {
    return true;
  } else if (needle < midval) {
    return _search(haystack, low, midpoint - 1, needle);
  } else {
    return _search(haystack, midpoint, high, needle);
  }
}

bool binary_search(int *haystack, int n, int needle) {
  return _search(haystack, 0, n, needle);
}

int main() {
  int haystack[] = {1, 3, 4, 69, 71, 81, 90, 99, 420, 1337, 69420};
  int n = 11;

  assert( binary_search(haystack, n, 69));
  assert(!binary_search(haystack, n, 1336));
  assert( binary_search(haystack, n, 69420));
  assert(!binary_search(haystack, n, 69421));
  assert( binary_search(haystack, n, 1));
  assert(!binary_search(haystack, n, 0));

  return 0;
}
