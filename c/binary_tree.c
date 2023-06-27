#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct array_list {
  int length;
  int capacity;
  int *backing;
} list_t;

int list_init(list_t *l, int capacity) {
  l->length = 0;
  l->capacity = capacity;
  if ((l->backing = malloc(sizeof(int) * capacity)) == NULL) {
    return 1;
  }
  return 0;
}

void list_deinit(list_t *l) {
  free(l->backing);
  free(l);
}

int list_push(list_t *l, int value) {
  if (l->length >= l->capacity) {
    l->capacity *= 2;
    if ((l->backing = realloc(l->backing, l->capacity)) == NULL) {
      return 1;
    }
  }
  l->backing[l->length++] = value;
  return 0;
}

typedef struct node {
  int value;
  struct node *left;
  struct node *right;
} node_t;

void tree_deinit(node_t *n) {
  if (n == NULL) {
    return;
  }

  tree_deinit(n->left);
  tree_deinit(n->right);
  free(n);
}

void tree_pre_order(node_t *n, list_t *path) {
  if (n == NULL) {
    return;
  }

  list_push(path, n->value);
  tree_pre_order(n->left, path);
  tree_pre_order(n->right, path);
}

void tree_in_order(node_t *n, list_t *path) {
  if (n == NULL) {
    return;
  }

  tree_in_order(n->left, path);
  list_push(path, n->value);
  tree_in_order(n->right, path);
}

void tree_post_order(node_t *n, list_t *path) {
  if (n == NULL) {
    return;
  }

  tree_post_order(n->left, path);
  tree_post_order(n->right, path);
  list_push(path, n->value);
}

int main() {
  /*
   *    20
   *    +- 50
   *    |  +- 100
   *    |  |
   *    |  +- 30
   *    |     +- 45
   *    |     |
   *    |     +- 29
   *    |
   *    +- 10
   *       +- 15
   *       |
   *       +- 5
   *          +- 7
   *       
   */
  node_t *n7 = malloc(sizeof(node_t));
  n7->value = 7;
  n7->right = NULL;
  n7->left = NULL;
  node_t *n5 = malloc(sizeof(node_t));
  n5->value = 5;
  n5->right = n7;
  n5->left = NULL;
  node_t *n15 = malloc(sizeof(node_t));
  n15->value = 15;
  n15->right = NULL;
  n15->left = NULL;
  node_t *n10 = malloc(sizeof(node_t));
  n10->value = 10;
  n10->right = n15;
  n10->left = n5;
  node_t *n29 = malloc(sizeof(node_t));
  n29->value = 29;
  n29->right = NULL;
  n29->left = NULL;
  node_t *n45 = malloc(sizeof(node_t));
  n45->value = 45;
  n45->right = NULL;
  n45->left = NULL;
  node_t *n30 = malloc(sizeof(node_t));
  n30->value = 30;
  n30->right = n45;
  n30->left = n29;
  node_t *n100 = malloc(sizeof(node_t));
  n100->value = 100;
  n100->right = NULL;
  n100->left = NULL;
  node_t *n50 = malloc(sizeof(node_t));
  n50->value = 50;
  n50->right = n100;
  n50->left = n30;
  node_t *n20 = malloc(sizeof(node_t));
  n20->value = 20;
  n20->right = n50;
  n20->left = n10;

  int pre_order[] = {20, 10, 5, 7, 15, 50, 30, 29, 45, 100};
  list_t *pol = malloc(sizeof(list_t));
  assert(list_init(pol, 20) == 0);
  tree_pre_order(n20, pol);
  for (int i = 0; i < 10; i++) {
    assert(pol->backing[i] == pre_order[i]);
  }
  list_deinit(pol);

  int in_order[] = {5, 7, 10, 15, 20, 29, 30, 45, 50, 100};
  list_t *iol = malloc(sizeof(list_t));
  assert(list_init(iol, 20) == 0);
  tree_in_order(n20, iol);
  for (int i = 0; i < 10; i++) {
    assert(iol->backing[i] == in_order[i]);
  }
  list_deinit(iol);

  int post_order[] = {7, 5, 15, 10, 29, 45, 30, 100, 50, 20};
  list_t *sol = malloc(sizeof(list_t));
  assert(list_init(sol, 20) == 0);
  tree_post_order(n20, sol);
  for (int i = 0; i < 10; i++) {
    assert(sol->backing[i] == post_order[i]);
  }
  list_deinit(sol);

  tree_deinit(n20);

  return 0;
}
