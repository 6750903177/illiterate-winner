#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int value;
  struct node *parent;
  struct node *left;
  struct node *right;
  bool is_black;
} node_t;

node_t* node_new(int value) {
  node_t *n = NULL;
  if ((n = malloc(sizeof(node_t))) == NULL) {
    return NULL;
  }

  n->value = value;
  n->parent = NULL;
  n->left = NULL;
  n->right = NULL;
  n->is_black = false;

  return n;
}

node_t* _insert_left(node_t *n, int value) {
  node_t *m = NULL;
  if ((m = node_new(value)) == NULL) {
    return NULL;
  }

  m->parent = n;
  n->left = m;
  return m;
}

node_t* _insert_right(node_t *n, int value) {
  node_t *m = NULL;
  if ((m = node_new(value)) == NULL) {
    return NULL;
  }

  m->parent = n;
  n->right = m;
  return m;
}

node_t* node_insert(node_t *n, int value) {
  return (value <= n->value)
    ? (n->left == NULL)
      ? _insert_left(n, value)
      : node_insert(n->left, value)
    : (n->right == NULL)
      ? _insert_right(n, value)
      : node_insert(n->right, value);
}


typedef struct tree {
  node_t *root;
} tree_t;

tree_t* tree_new(void) {
  tree_t *t = NULL;
  if ((t = malloc(sizeof(tree_t))) == NULL) {
    return NULL;
  }

  t->root = NULL;
  return t;
}

void _free_driver(node_t *n) {
  if (n == NULL) {
    return;
  }

  _free_driver(n->left);
  _free_driver(n->right);
  free(n);
}

void tree_free(tree_t *t) {
  _free_driver(t->root);
  free(t);
}

void tree_rotate_left(tree_t *t, node_t *n) {
  node_t *m = n->right;
  n->right = m->left;

  if (m->left != NULL) {
    m->left->parent = n;
  }

  m->parent = n->parent;
  if (n->parent == NULL) {
    t->root = m;
  } else if (n == n->parent->left) {
    n->parent->left = m;
  } else {
    n->parent->right = m;
  }

  m->left = n;
  n->parent = m;

  return;
}

void tree_rotate_right(tree_t *t, node_t *n) {
  node_t *m = n->left;
  n->left = m->right;

  if (m->right != NULL) {
    m->right->parent = n;
  }

  m->parent = n->parent;
  if (n->parent == NULL) {
    t->root = m;
  } else if (n == n->parent->right) {
    n->parent->right = m;
  } else {
    n->parent->left = m;
  }

  m->right = n;
  n->parent = m;

  return;
}

void _red_black_fix(tree_t *t, node_t *n) {
  while (n->parent != NULL && !n->parent->is_black && n->parent->parent != NULL) {
    if (n->parent == n->parent->parent->left) {
      node_t *y = n->parent->parent->right;
      if (!y->is_black) {
        n->parent->is_black = true;
        y->is_black = true;
        n->parent->parent->is_black = false;
        n = n->parent->parent;
      } else {
        if (n == n->parent->right) {
          n = n->parent;
          tree_rotate_left(t, n);
        }
        n->parent->is_black = true;
        n->parent->parent->is_black = false;
        tree_rotate_right(t, n->parent->parent);
      }
    } else {
      node_t *y = n->parent->parent->right;
      if (!y->is_black) {
        n->parent->is_black = true;
        y->is_black = true;
        n->parent->parent->is_black = false;
        n = n->parent->parent;
      } else {
        if (n == n->parent->right) {
          n = n->parent;
          tree_rotate_right(t, n);
        }
        n->parent->is_black = true;
        n->parent->parent->is_black = false;
        tree_rotate_right(t, n->parent->parent);
      }
    }
  }

  t->root->is_black = true;
}

node_t* tree_insert(tree_t *t, int value) {
  node_t *n = NULL;

  if (t->root == NULL) {
    n = node_new(value);
    t->root = n;
    return n;
  }

  n = node_insert(t->root, value);
  if (n == NULL) {
    return NULL;
  }

  if (
    n->parent != NULL &&
    n->parent->parent != NULL &&
    n->parent->parent->left != NULL &&
    n->parent->parent->right != NULL
  ) {
    _red_black_fix(t, n);
  }
  return n;
}

void _print(node_t *n) {
  if (n == NULL) {
    return;
  }

  _print(n->left);
  printf("%d ", n->value);
  _print(n->right);
} 

void tree_print(tree_t *t) {
  printf("in-order: ");
  _print(t->root);
  printf("\n");
}

int main(void) {
  tree_t *t = tree_new();
  assert(t != NULL);

  tree_insert(t, 11);
  tree_insert(t, 2);
  tree_insert(t, 1);
  tree_insert(t, 7);
  tree_insert(t, 5);
  tree_insert(t, 4);
  tree_insert(t, 8);
  tree_insert(t, 14);
  tree_insert(t, 15);

  tree_print(t);

  tree_free(t);
  return 0;
}