#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int value;
  struct node *prev;
  struct node *next;
} node_t;

void node_init(node_t *node, int value) {
  node->value = value;
  node->prev = NULL;
  node->next = NULL;
}

typedef struct doubly_linked_list {
  node_t *head;
  node_t *tail;
  int length;
} doubly_linked_list_t;

int dll_init(doubly_linked_list_t *dll) {
  if (dll == NULL) {
    return 1;
  }

  dll->head = NULL;
  dll->tail = NULL;
  dll->length = 0;

  return 0;
}

void dll_deinit(doubly_linked_list_t *dll) {
  while (dll->tail != NULL) {
    dll->length--;
    node_t *curr = dll->tail;
    dll->tail = curr->prev;
    free(curr);
  }

  free(dll);
}

void dll_print(doubly_linked_list_t *dll) {
  node_t *curr = dll->head;
  for (int i = 0; i < dll->length; i++) {
    printf("NODE %d: VALUE=%d\n", i, curr->value);
    curr = curr->next;
  }
}

void dll_append(doubly_linked_list_t *dll, int value) {
  dll->length++;
  node_t *node = malloc(sizeof(node_t));
  node_init(node, value);
  if (dll->tail == NULL) {
    dll->head = node;
    dll->tail = node;
  } else {
    node->prev = dll->tail;
    dll->tail->next = node;
    dll->tail = node;
  }
}

void dll_prepend(doubly_linked_list_t *dll, int value) {
  dll->length++;
  node_t *node = malloc(sizeof(node_t));
  node_init(node, value);
  if (dll->head == NULL) {
    dll->head = node;
    dll->tail = node;
  } else {
    node->next = dll->head;
    dll->head->prev = node;
    dll->head = node;
  }
}

int dll_insert_at(doubly_linked_list_t *dll, int idx, int value) {
  if (idx >= dll->length || idx < 0) {
    return 1;
  }

  if (idx == 0) {
    dll_prepend(dll, value);
    return 0;
  }

  if (idx == dll->length) {
    dll_append(dll, value);
    return 0;
  }

  dll->length++;
  node_t *node = malloc(sizeof(node_t));
  node_init(node, value);

  node_t *curr = dll->head;
  for (int i = 0; i < idx - 1; i++) {
    curr = curr->next;
  }

  node->prev = curr;
  node->next = curr->next;
  curr->next->prev = node;
  curr->next = node;

  return 0;
}

int dll_remove_at(doubly_linked_list_t *dll, int idx, int *value) {
  if (idx >= dll->length || idx < 0) {
    return 1;
  }

  dll->length--;
  node_t *curr = dll->head;

  for (int i = 0; i < idx; i++) {
    curr = curr->next;
  }

  if (curr->prev != NULL) {
    curr->prev->next = curr->next;
  } else {
    dll->head = curr->next;
  }

  if (curr->next != NULL) {
    curr->next->prev = curr->prev;
  } else {
    dll->tail = curr->prev;
  }

  *value = curr->value;

  if (curr != NULL) {
    free(curr);
  }

  return 0;
}

int dll_remove(doubly_linked_list_t *dll, int tgt, int *value) {
  if (dll->length == 0) {
    return 1;
  }

  int idx = -1;
  node_t *curr = dll->head;
  for (int i = 0; i < dll->length; i++) {
    if (curr->value == tgt) {
      idx = i;
      break;
    }
    curr = curr->next;
  }

  if (idx == -1) {
    return 1;
  }

  return dll_remove_at(dll, idx, value);
}

int dll_get(doubly_linked_list_t *dll, int idx, int *value) {
  if (idx >= dll->length || idx < 0) {
    return 1;
  }

  node_t *curr = dll->head;
  for (int i = 0; i < idx; i++) {
    curr = curr->next;
  }

  *value = curr->value;

  return 0;
} 

int main() {
  doubly_linked_list_t *dll = malloc(sizeof(doubly_linked_list_t));
  assert(dll_init(dll) == 0);
  dll_append(dll, 5);
  dll_append(dll, 7);
  dll_append(dll, 9);

  int value;
  assert((dll_get(dll, 0, &value)) == 0);
  assert((dll_get(dll, 1, &value)) == 0);
  assert((dll_get(dll, 2, &value)) == 0);
  assert(value == 9);

  assert((dll_remove_at(dll, 1, &value)) == 0);
  assert(value == 7);
  assert(dll->length == 2);

  dll_append(dll, 11);
  assert((dll_remove_at(dll, 1, &value)) == 0);
  assert(value == 9);
  assert((dll_remove(dll, 9, &value)) == 1);
  assert((dll_remove_at(dll, 0, &value)) == 0);
  assert(value == 5);
  assert((dll_remove_at(dll, 0, &value)) == 0);
  assert(value == 11);
  assert(dll->length == 0);

  dll_prepend(dll, 5);
  dll_prepend(dll, 7);
  dll_prepend(dll, 9);

  assert((dll_get(dll, 2, &value)) == 0);
  assert(value == 5);
  assert((dll_get(dll, 0, &value)) == 0);
  assert(value == 9);
  assert((dll_remove(dll, 9, &value)) == 0);
  assert(value == 9);
  assert(dll->length == 2);
  assert((dll_get(dll, 0, &value)) == 0);
  assert(value == 7);

  dll_insert_at(dll, 1, 1);
  assert((dll_get(dll, 1, &value)) == 0);
  assert(value == 1);
  dll_deinit(dll);

  return 0;
}
