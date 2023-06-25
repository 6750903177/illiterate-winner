#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int max(int a, int b) {
  return (a > b) ? a : b;
}

int two_crystal_balls(bool *breaks, int n) {
  int jump = (int) (sqrt((double) n));
  int first_break = 0;
  for (int i = 0; i < n; i += jump) {
    if (breaks[i]) {
      first_break = i;
      break;
    }
  }

  for (int i = max(0, first_break - jump); i < first_break; i++) {
    if (breaks[i]) {
      return i;
    }
  }

  return -1;
}

int main() {
  int idx = rand() % 10000;
  bool breaks1[10000] = {false};
  for (int i = idx; i < 10000; i++) {
    breaks1[i] = true;
  }
  assert(two_crystal_balls(breaks1, 10000) == idx);

  bool breaks2[821] = {false};
  assert(two_crystal_balls(breaks2, 821) == -1);

  return 0;
}
