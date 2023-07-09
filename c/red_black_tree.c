#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum color {
  BLACK = 0,
  RED = 1,
} color_t;

typedef enum orientation {
  LEFT = -1,
  NONE = 0,
  RIGHT = 1,
} orientation_t;

typedef struct node {
  int value;
  struct node *p;
  struct node *left;
  struct node *right;
  color_t color;
} node_t;

node_t* node_new(int value) {
  node_t *n = NULL;
  if ((n = malloc(sizeof(node_t))) == NULL) {
    return NULL;
  }

  n->value = value;
  n->p = NULL;
  n->left = NULL;
  n->right = NULL;
  n->color = RED;

  return n;
}

void node_free(node_t *n) {
  if (n == NULL) {
    return;
  }

  node_free(n->left);
  node_free(n->right);
  free(n);
}

void node_recolor(node_t *n) {
  n->color ^= 1;
}

node_t* node_parent(node_t *n) {
  return (n == NULL) ? NULL : n->p;
}

node_t* node_grandparent(node_t *n) {
  if (n == NULL || n->p == NULL) {
    return NULL;
  }

  return n->p->p;
}

node_t* node_uncle(node_t *n) {
  node_t *p = node_parent(n);
  node_t *g = node_grandparent(n);
  if (n == NULL || p == NULL || g == NULL) {
    return NULL;
  }

  return (p == g->left) ? g->right : g->left;
}

void _insert_left(node_t *n, node_t *m) {
  m->p = n;
  n->left = m;
}

void _insert_right(node_t *n, node_t *m) {
  m->p = n;
  n->right = m;
}

void node_insert(node_t *n, node_t *m) {
  return (m->value <= n->value)
    ? (n->left == NULL)
      ? _insert_left(n, m)
      : node_insert(n->left, m)
    : (n->right == NULL)
      ? _insert_right(n, m)
      : node_insert(n->right, m);
}

orientation_t node_forms_line(node_t *z, node_t *p, node_t *g) {
  if (z == NULL || p == NULL || g == NULL) {
    return NONE;
  }

  if (z == p->left && p == g->left) {
    /*
     *      G
     *     /
     *    P
     *   /
     *  Z 
     */
    return LEFT;
  }

  if (z == p->right && p == g->right) {
    /*
     *  G
     *   \
     *    P
     *     \
     *      Z 
     */
    return RIGHT;
  }

  return NONE;
}

orientation_t node_forms_angle(node_t *z, node_t *p, node_t *g) {
  if (z == NULL || p == NULL || g == NULL) {
    return NONE;
  }

  if (z == p->left && p == g->right) {
    /*
     *    G
     *     \
     *      P
     *     /
     *    Z 
     */
    return RIGHT;
  }

  if (z == p->right && p == g->left) {
    /*
     *    G
     *   /
     *  P
     *   \
     *    Z 
     */
    return LEFT;
  }

  return NONE;
}

typedef struct tree {
  node_t *root;
} tree_t;

tree_t* tree_new() {
  tree_t *t = NULL;
  if ((t = malloc(sizeof(tree_t))) == NULL) {
    return NULL;
  }

  t->root = NULL;

  return t;
}

void tree_free(tree_t *t) {
  node_free(t->root);
  free(t);
}

void tree_rotate_left(tree_t *t, node_t *x) {
  node_t *y = x->right;
  x->right = y->left;

  if (y->left != NULL) {
    y->left->p = x;
  }
  y->p = x->p;

  if (x->p == NULL) {
    t->root = y;
  } else if (x == x->p->left) {
    x->p->left = y;
  } else {
    x->p->right = y;
  }

  y->left = x;
  x->p = y;

  return;
}

void tree_rotate_right(tree_t *t, node_t *x) {
  node_t *y = x->left;
  x->left = y->right;

  if (y->right != NULL) {
    y->left->p = x;
  }
  y->p = x->p;

  if (x->p == NULL) {
    t->root = y;
  } else if (x == x->p->right) {
    x->p->right = y;
  } else {
    x->p->left = y;
  }

  y->right = x;
  x->p = y;

  return;
}

void _rbtree_fix_insert(tree_t *t, node_t *z) {
  while (z->p && z->p->color == RED) {
    node_t *p = node_parent(z);
    node_t *g = node_grandparent(z);
    node_t *u = node_uncle(z);

    // case #1: z's uncle is RED
    if (u != NULL && u->color == RED) {
      node_recolor(p);
      node_recolor(g);
      node_recolor(u);
      z = g;
    }

    // case #2: z's uncle is BLACK and forms angle
    orientation_t is_angle = node_forms_angle(z, p, g);
    if ((u == NULL || u->color == BLACK) && is_angle != NONE) {
      if (is_angle == LEFT) {
        tree_rotate_left(t, p);
      } else {
        tree_rotate_right(t, p);
      }
      z = p;
    }

    // case #3: z's uncle is BLACK and forms line
    orientation_t is_line = node_forms_line(z, p, g);
    if ((u == NULL || u->color == BLACK) && is_line != NONE) {
      if (is_line == LEFT) {
        tree_rotate_right(t, g);
      } else {
        tree_rotate_left(t, g);
      }
      node_recolor(p);
      node_recolor(g);
    }
  }

  t->root->color = BLACK;
}

node_t* tree_insert(tree_t *t, int value) {
  node_t *n = NULL;
  if ((n = node_new(value)) == NULL) {
    return NULL;
  }

  if (t->root == NULL) {
    t->root = n;
  } else {
    node_insert(t->root, n);
  }

  _rbtree_fix_insert(t, n);
  return n;
}

void _rbtree_fix_delete(tree_t *t, node_t *n) {
  return;
}

void _print(node_t *n) {
  if (n == NULL) {
    return;
  }

  _print(n->left);
  printf("NODE %d, COLOR %d\n", n->value, n->color);
  _print(n->right);
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

  _print(t->root);

  //
  // tree_print(t);
  // printf("%d(%c)\n", t->root->value, t->root->color);
  //
  tree_free(t);
  return 0;
}
