#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct tree {
  int value;
  struct tree *left;
  struct tree *right;
} tree_t;

void tree_init(tree_t *t, int value) {
  t->value = value;
  t->left = NULL;
  t->right = NULL;
}

void tree_deinit(tree_t *t) {
  if (t == NULL) {
    return;
  }

  tree_deinit(t->left);
  tree_deinit(t->right);
  free(t);
}

int _insert_left(tree_t *t, int value) {
  if ((t->left = malloc(sizeof(tree_t))) == NULL) {
    return 1;
  }

  tree_init(t->left, value);
  return 0;
}

int _insert_right(tree_t *t, int value) {
  if ((t->right = malloc(sizeof(tree_t))) == NULL) {
    return 1;
  }

  tree_init(t->right, value);
  return 0;
}

int tree_insert(tree_t *t, int value) {
  return (value <= t->value)
    ? (t->left == NULL)
        ? _insert_left(t, value)
        : tree_insert(t->left, value)
    : (t->right == NULL)
        ? _insert_right(t, value)
        : tree_insert(t->right, value);
}

bool tree_find(tree_t *t, int value) {
  if (t == NULL) {
    return false;
  }

  if (t->value == value) {
    return true;
  }

  return (value < t->value)
    ? tree_find(t->left, value)
    : tree_find(t->right, value);
}

void tree_print(tree_t *t) {
  if (t == NULL) {
    return;
  }

  tree_print(t->left);
  printf("%d\n", t->value);
  tree_print(t->right);
}

void _find_greatest_smaller_child(
  tree_t *p,
  tree_t *c,
  tree_t **ptrp,
  tree_t **ptrc
) {
  if (c->right == NULL) {
    *ptrp = p;
    *ptrc = c;
    return;
  }

  _find_greatest_smaller_child(c, c->right, ptrp, ptrc);
}

void _find_smallest_greater_child(
  tree_t *p,
  tree_t *c,
  tree_t **ptrp,
  tree_t **ptrc
) {
  if (c->left == NULL) {
    *ptrp = p;
    *ptrc = c;
    return;
  }

  _find_smallest_greater_child(c, c->left, ptrp, ptrc);
}

void _unlink_leaf(tree_t *parent, tree_t *current) {
  if (current->value <= parent->value) {
    parent->left = NULL;
  } else {
    parent->right = NULL;
  }
}

void _unlink_with_single_child(tree_t *parent, tree_t *current) {
  if (current->left != NULL) {
    if (current->value <= parent->value) {
      parent->left = current->left;
    } else {
      parent->right = current->left;
    }
    current->left = NULL;
  } else {
    if (current->value <= parent->value) {
      parent->left = current->right;
    } else {
      parent->right = current->right;
    }
    current->right = NULL;
  }
}

int _delete_node(tree_t *parent, tree_t *current) {
  if (current->left != NULL && current->right != NULL) {
    tree_t **ptrp = malloc(sizeof(tree_t*));
    tree_t **ptrc = malloc(sizeof(tree_t*));
    _find_greatest_smaller_child(current, current->left, ptrp, ptrc);

    current->value = (*ptrc)->value;
    (*ptrp)->right = NULL;
    free(*ptrc);

    free(ptrc);
    free(ptrp);
  } else if (current->left == NULL && current->right == NULL) {
    _unlink_leaf(parent, current);
    free(current);
  } else {
    _unlink_with_single_child(parent, current);
    free(current);
  }
  return 0;
}

int tree_delete(tree_t *t, int value) {
  int driver(tree_t *p, tree_t *c, int value) {
    if (c == NULL) {
      return 1;
    }

    if (c->value == value) {
      return _delete_node(p, c);
    }

    return (value <= c->value)
      ? driver(c, c->left, value)
      : driver(c, c->right, value);
  }

  return driver(NULL, t, value);
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
  tree_t *t = malloc(sizeof(tree_t));
  tree_init(t, 20);

  tree_insert(t, 10);
  tree_insert(t, 50);
  tree_insert(t, 5);
  tree_insert(t, 15);
  tree_insert(t, 30);
  tree_insert(t, 100);
  tree_insert(t, 7);
  tree_insert(t, 29);
  tree_insert(t, 45);

  assert(tree_delete(t, 20) == 0);
  assert(tree_delete(t, 50) == 0);
  assert(tree_delete(t, 69) == 1);

  assert( tree_find(t, 45));
  assert( tree_find(t, 5));
  assert(!tree_find(t, 69));

  tree_deinit(t);
  return 0;
}
