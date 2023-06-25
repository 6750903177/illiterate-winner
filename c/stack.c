#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int value;
  struct node *prev;
} node_t;

void node_init(node_t *node, int value) {
  node->value = value;
  node->prev = NULL;
}

typedef struct stack {
  node_t *tail;
  int length;
} stack_t;

int stack_init(stack_t *s) {
  if (s == NULL) {
    return 1;
  }

  s->tail = NULL;
  s->length = 0;

  return 0;
}

void stack_deinit(stack_t *s) {
  while (s->tail != NULL) {
    s->length--;
    node_t *curr = s->tail;
    s->tail = curr->prev;
    free(curr);
  }

  free(s);
}

void stack_print(stack_t *s) {
  node_t *curr = s->tail;
  for (int i = 0; i < s->length; i++) {
    printf("NODE %d: VALUE=%d\n", i, curr->value);
    curr = curr->prev;
  }
}

void stack_push(stack_t *s, int value) {
  s->length++;
  node_t *node = malloc(sizeof(node_t));
  node_init(node, value);
  if (s->tail == NULL) {
    s->tail = node;
  } else {
    node->prev = s->tail;
    s->tail = node;
  }
}

int stack_pop(stack_t *s, int *value) {
  if (s->tail == NULL) {
    return 1;
  }

  s->length--;
  node_t *curr = s->tail;
  *value = curr->value;
  if (s->length == 0) {
    s->tail = NULL;
  } else {
    s->tail = curr->prev;
  }
  free(curr);

  return 0;
}

int stack_peek(stack_t *s, int *value) {
  if (s->tail == NULL) {
    return 1;
  }

  *value = s->tail->value;
  return 0;
}

int main() {
  stack_t *s = malloc(sizeof(stack_t));
  assert(stack_init(s) == 0);

  stack_push(s, 5);
  stack_push(s, 7);
  stack_push(s, 9);

  int value;
  assert((stack_pop(s, &value)) == 0);
  assert(value == 9);
  assert(s->length == 2);

  stack_push(s, 11);
  assert((stack_pop(s, &value)) == 0);
  assert(value == 11);
  assert((stack_pop(s, &value)) == 0);
  assert(value == 7);
  assert((stack_peek(s, &value)) == 0);
  assert(value == 5);
  assert((stack_pop(s, &value)) == 0);
  assert(value == 5);
  assert((stack_pop(s, &value)) == 1);

  stack_push(s, 69);
  assert((stack_peek(s, &value)) == 0);
  assert(value == 69);
  assert(s->length == 1);

  stack_deinit(s);

  return 0;
}
