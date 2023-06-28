#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct tree {
  int value;
  struct tree *left;
  struct tree *right;
} tree_t;

void tree_deinit(tree_t *n) {
  if (n == NULL) {
    return;
  }

  tree_deinit(n->left);
  tree_deinit(n->right);
  free(n);
}

bool tree_find(tree_t *t, int needle) {
  if (t->value == needle) {
    return true;
  }

  int v = t->value;
  if (needle <= v && t->left != NULL) {
    return tree_find(t->left, needle);
  } else if (needle > v && t->right != NULL) {
    return tree_find(t->right, needle);
  }

  return false;
}

void tree_insert(tree_t *t, int value) {
  int v = t->value;
  if (value <= v) {
    if (t->left == NULL) {
      tree_t *node = malloc(sizeof(tree_t*));
      node->value = value;
      t->left = node;
    } else {
      tree_insert(t->left, value);
    }
  } else {
    if (t->right == NULL) {
      tree_t *node = malloc(sizeof(tree_t*));
      node->value = value;
      t->right = node;
    } else {
      tree_insert(t->right, value);
    }
  }
}

void tree_find_greatest_child(
  tree_t *parent, tree_t *current,
  tree_t **p, tree_t **c
) {
  if (current->right == NULL) {
    *c = current;
    *p = parent;
    return;
  }

  tree_find_greatest_child(current, current->right, p, c);
}

void tree_remove_node(tree_t *p, tree_t *c, tree_t **v) {
  if (c->left == NULL && c->right == NULL) {  // leaf node
    if (p != NULL) {
      printf("leaf node\n");
      if (c->value <= p->value) {
        p->left = NULL;
      } else {
        p->right = NULL;
      }
    }
  } else if (c->left == NULL) {  // has single child
    p->left = c->right;
    printf("p->left->value=%d\n", p->left->value);
  } else if (c->right == NULL) {  // has single child
    p->right = c->left;
    printf("p->right->value=%d\n", p->right->value);
  } else {  // hard mode
    tree_t **ptr_p = malloc(sizeof(tree_t**));
    tree_t **ptr_c = malloc(sizeof(tree_t**));
    tree_find_greatest_child(c, c->left, ptr_p, ptr_c);

    tree_t *tmp_p = *ptr_p;
    tree_t *tmp_c = *ptr_c;

    tmp_p->right = NULL;
    if (tmp_c->left != NULL) {
      tmp_p->right = tmp_c->left;
      tmp_c->left = NULL;
    }

    tmp_c->right = c->right;
    tmp_c->left = c->left;

    free(ptr_p);
    free(ptr_c);

    if (p != NULL) {
      if (tmp_c->value <= p->value) {
        p->left = tmp_c;
      } else {
        p->right = tmp_c;
      }
    } else {  // overwrite root node with temp node
      *c = *tmp_c;
      return;
    }
  }
  free(c);
}

void tree_delete(tree_t *t, int value) {
  void driver(tree_t *p, tree_t *c, int v) {
    if (c->value == v) {  // current node needs to die
      tree_remove_node(p, c, NULL);
      return;
    }

    if (v <= c->value && c->left != NULL) {
      driver(c, c->left, v);
    } else if (v > c->value && c->right != NULL) {
      driver(c, c->right, v);
    }

    return;
  }

  driver(NULL, t, value);
}

void tree_print_in_order(tree_t *n) {
  if (n == NULL) {
    return;
  }

  tree_print_in_order(n->left);
  printf("NODE: %d\n", n->value);
  tree_print_in_order(n->right);
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
  tree_t *t = malloc(sizeof(tree_t*));
  t->value = 20;
  tree_insert(t, 10);
  tree_insert(t, 50);
  tree_insert(t, 5);
  tree_insert(t, 15);
  tree_insert(t, 30);
  tree_insert(t, 100);
  tree_insert(t, 7);
  tree_insert(t, 29);
  tree_insert(t, 45);

  assert( tree_find(t, 45));
  assert( tree_find(t, 5));
  assert(!tree_find(t, 69));

  tree_deinit(t);
  return 0;
}
