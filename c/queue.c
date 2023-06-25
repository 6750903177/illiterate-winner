#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int value;
  struct node *next;
} node_t;

void node_init(node_t *node, int value) {
  node->value = value;
  node->next = NULL;
}

typedef struct queue {
  node_t *head;
  node_t *tail;
  int length;
} queue_t;

int queue_init(queue_t *q) {
  if (q == NULL) {
    return 1;
  }

  q->head = NULL;
  q->tail = NULL;
  q->length = 0;

  return 0;
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

void queue_print(queue_t *q) {
  node_t *curr = q->head;
  for (int i = 0; i < q->length; i++) {
    printf("NODE %d: VALUE=%d\n", i, curr->value);
    curr = curr->next;
  }
}

void queue_enqueue(queue_t *q, int value) {
  q->length++;
  node_t *node = malloc(sizeof(node_t));
  node_init(node, value);
  if (q->tail == NULL) {
    q->head = node;
    q->tail = node;
  } else {
    q->tail->next = node;
    q->tail = node;
  }
}

int queue_deque(queue_t *q, int *value) {
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

int queue_peek(queue_t *q, int *value) {
  if (q->head == NULL) {
    return 1;
  }

  *value = q->head->value;
  return 0;
}

int main() {
  queue_t *q = malloc(sizeof(queue_t));
  assert(queue_init(q) == 0);

  queue_enqueue(q, 5);
  queue_enqueue(q, 7);
  queue_enqueue(q, 9);

  int value;
  assert((queue_deque(q, &value)) == 0);
  assert(value == 5);
  assert(q->length == 2);

  queue_enqueue(q, 11);
  assert((queue_deque(q, &value)) == 0);
  assert(value == 7);
  assert((queue_deque(q, &value)) == 0);
  assert(value == 9);
  assert((queue_peek(q, &value)) == 0);
  assert(value == 11);
  assert((queue_deque(q, &value)) == 0);
  assert(value == 11);
  assert((queue_deque(q, &value)) == 1);
  assert(q->length == 0);

  queue_enqueue(q, 69);
  assert((queue_peek(q, &value)) == 0);
  assert(value == 69);
  assert(q->length == 1);

  queue_deinit(q);

  return 0;
}
