#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*
 *  Structs and Function Prototypes
 */

typedef struct array_list {
  int length;
  int capacity;
  int *backing;
} list_t;
int list_init(list_t*, int);
void list_deinit(list_t*);
int list_push(list_t*, int);

typedef struct tree {
  int value;
  struct tree *left;
  struct tree *right;
} tree_t;
void tree_deinit(tree_t*);
bool tree_breadth_first_search(tree_t*, int);
void tree_pre_order(tree_t*, list_t*);
void tree_in_order(tree_t*, list_t*);
void tree_post_order(tree_t*, list_t*);

typedef struct node {
  tree_t *value;
  struct node *next;
} node_t;

typedef struct queue {
  node_t *head;
  node_t *tail;
  int length;
} queue_t;
void queue_init(queue_t*);
void queue_deinit(queue_t*);
void queue_push(queue_t*, tree_t*);
int queue_pop(queue_t*, tree_t**);


/*
 *  Function Implementations
 */

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

void tree_deinit(tree_t *n) {
  if (n == NULL) {
    return;
  }

  tree_deinit(n->left);
  tree_deinit(n->right);
  free(n);
}

bool tree_breadth_first_search(tree_t *n, int needle) {
  bool found = false;
  queue_t *buf = malloc(sizeof(queue_t));
  queue_init(buf);
  queue_push(buf, n);

  tree_t **curr_ptr = malloc(sizeof(tree_t**));
  while ((queue_pop(buf, curr_ptr)) == 0) {
    tree_t *curr = *curr_ptr;
    if (curr->value == needle) {
      found = true;
      break;
    }

    if (curr->left != NULL) {
      queue_push(buf, curr->left);
    }

    if (curr->right != NULL) {
      queue_push(buf, curr->right);
    }
  }

  free(curr_ptr);
  queue_deinit(buf);
  return found;
}

void tree_pre_order(tree_t *n, list_t *path) {
  if (n == NULL) {
    return;
  }

  list_push(path, n->value);
  tree_pre_order(n->left, path);
  tree_pre_order(n->right, path);
}

void tree_in_order(tree_t *n, list_t *path) {
  if (n == NULL) {
    return;
  }

  tree_in_order(n->left, path);
  list_push(path, n->value);
  tree_in_order(n->right, path);
}

void tree_post_order(tree_t *n, list_t *path) {
  if (n == NULL) {
    return;
  }

  tree_post_order(n->left, path);
  tree_post_order(n->right, path);
  list_push(path, n->value);
}

void queue_init(queue_t *q) {
  q->head = NULL;
  q->tail = NULL;
  q->length = 0;
  return;
}

void queue_deinit(queue_t *q) {
  while (q->head != NULL) {
    q->length--;
    node_t *curr = q->head;
    q->head = curr->next;
    free(curr);
  }
  free(q);
}

void queue_push(queue_t *q, tree_t *value) {
  q->length++;
  node_t *n = malloc(sizeof(node_t));
  n->value = value;
  n->next = NULL;

  if (q->tail == NULL) {
    q->head = n;
    q->tail = n;
  } else {
    q->tail->next = n;
    q->tail = n;
  }
}

int queue_pop(queue_t *q, tree_t **value) {
  if (q->head == NULL) {
    return 1;
  }

  q->length--;
  node_t *curr = q->head;
  *value = curr->value;
  if (q->length == 0) {
    q->head = NULL;
    q->tail = NULL;
  } else {
    q->head = curr->next;
  }
  free(curr);
  return 0;
}


/*
 *  Main
 */

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
  tree_t *n7 = malloc(sizeof(tree_t));
  n7->value = 7;
  n7->right = NULL;
  n7->left = NULL;
  tree_t *n5 = malloc(sizeof(tree_t));
  n5->value = 5;
  n5->right = n7;
  n5->left = NULL;
  tree_t *n15 = malloc(sizeof(tree_t));
  n15->value = 15;
  n15->right = NULL;
  n15->left = NULL;
  tree_t *n10 = malloc(sizeof(tree_t));
  n10->value = 10;
  n10->right = n15;
  n10->left = n5;
  tree_t *n29 = malloc(sizeof(tree_t));
  n29->value = 29;
  n29->right = NULL;
  n29->left = NULL;
  tree_t *n45 = malloc(sizeof(tree_t));
  n45->value = 45;
  n45->right = NULL;
  n45->left = NULL;
  tree_t *n30 = malloc(sizeof(tree_t));
  n30->value = 30;
  n30->right = n45;
  n30->left = n29;
  tree_t *n100 = malloc(sizeof(tree_t));
  n100->value = 100;
  n100->right = NULL;
  n100->left = NULL;
  tree_t *n50 = malloc(sizeof(tree_t));
  n50->value = 50;
  n50->right = n100;
  n50->left = n30;
  tree_t *n20 = malloc(sizeof(tree_t));
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

  assert( tree_breadth_first_search(n20, 45));
  assert( tree_breadth_first_search(n20, 7));
  assert(!tree_breadth_first_search(n20, 69));

  tree_deinit(n20);
  return 0;
}
