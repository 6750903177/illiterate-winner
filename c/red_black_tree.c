#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int value;
  struct node *parent;
  struct node *left;
  struct node *right;
  char color;
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
  n->color = 'r';

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

void tree_rotate_left(tree_t *t, node_t *x) {
  if (x == NULL || x->right == NULL) {
    return;
  }

  node_t *y = x->right;
  x->right = y->left;
  
  if (x->right != NULL) {
    x->right->parent = x;
  }

  if (x->parent == NULL) {
    t->root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
  return;
}

void tree_rotate_right(tree_t *t, node_t *x) {
  if (x == NULL || x->right == NULL) {
    return;
  }

  node_t *y = x->right;
  x->right = y->right;
  
  if (x->right != NULL) {
    x->right->parent = x;
  }

  if (x->parent == NULL) {
    t->root = y;
  } else if (x == x->parent->right) {
    x->parent->right = y;
  } else {
    x->parent->right = y;
  }

  y->right = x;
  x->parent = y;
  return;
}

void _red_black_fix(tree_t *t, node_t *z) {
  while (
    z != t->root &&
    z != t->root->left &&
    z != t->root->right &&
    z->parent->color == 'r'
  ) {
    // find uncle node
    node_t *y = NULL;
    if (z->parent && z->parent->parent && z->parent == z->parent->parent->left) {
      y = z->parent->parent->right;
    } else {
      y = z->parent->parent->left;
    }

    if (y == NULL) {
      // if uncle does not exist
      // 1. move current node to grandparent
      z = z->parent->parent;
    } else if (y->color == 'r') {
      // if uncle is RED
      // 1. change color of parent and uncle to BLACK
      // 2. change color of grandparent to RED
      // 3. move current node to grandparent
      y->color = 'b';
      z->parent->color = 'b';
      z->parent->parent->color = 'r';
      z = z->parent->parent;
    } else {
      if (z->parent == z->parent->parent->left && z == z->parent->left) {
        char color = z->parent->color;
        z->parent->color = z->parent->parent->color;
        z->parent->parent->color = color;
        tree_rotate_right(t, z->parent->parent);
      }

      if (z->parent && z->parent->parent && z->parent == z->parent->parent->left && z == z->parent->right) {
        char color = z->color;
        z->color = z->parent->parent->color;
        z->parent->parent->color = color;
        tree_rotate_left(t, z->parent);
        tree_rotate_right(t, z->parent->parent);
      }

      if (z->parent && z->parent->parent && z->parent == z->parent->parent->right && z == z->parent->right) {
        char color = z->parent->color;
        z->parent->color = z->parent->parent->color;
        z->parent->parent->color = color;
        tree_rotate_left(t, z->parent->parent);
      }

      if (z->parent && z->parent->parent && z->parent == z->parent->parent->right && z == z->parent->left) {
        char color = z->color;
        z->color = z->parent->parent->color;
        z->parent->parent->color = color;
        tree_rotate_right(t, z->parent);
        tree_rotate_left(t, z->parent->parent);
      }
    }
  }

  t->root->color = 'b';
  return;
}

node_t* tree_insert(tree_t *t, int value) {
  node_t *n = NULL;

  if (t->root == NULL) {
    n = node_new(value);
    n->color = 'b';
    t->root = n;
    return n;
  }

  n = node_insert(t->root, value);
  if (n == NULL) {
    return NULL;
  }

  _red_black_fix(t, n);
  return n;
}

void _print(node_t *n) {
  if (n == NULL) {
    return;
  }

  _print(n->left);
  printf("%d(%c) ", n->value, n->color);
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
  tree_insert(t, 8);
  tree_insert(t, 14);
  tree_insert(t, 15);
  tree_insert(t, 4);

  tree_print(t);
  printf("%d(%c)\n", t->root->value, t->root->color);

  tree_free(t);
  return 0;
}
