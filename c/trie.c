#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int VOCAB_SIZE = 26;

typedef struct trie {
  bool is_word;
  struct trie **children;
} trie_t;

int trie_init(trie_t *t) {
  t->is_word = false;
  if ((t->children = malloc(sizeof(trie_t*) * VOCAB_SIZE)) == NULL) {
    return 1;
  }

  for (int i = 0; i < VOCAB_SIZE; i++) {
    t->children[i] = NULL;
  }

  return 0;
}

void trie_deinit(trie_t *t) {
  if (t == NULL) {
    return;
  }

  for (int i = 0; i < VOCAB_SIZE; i++) {
    trie_deinit(t->children[i]);
  }

  free(t->children);
  free(t);
}

int trie_insert(trie_t *t, const char *str) {
  int n = strlen(str);
  if (n == 0) {
    t->is_word = true;
    return 0;
  }

  char l = str[0];
  int idx = l - 'a';
  if (t->children[idx] == NULL) {
    if ((t->children[idx] = malloc(sizeof(trie_t*) * VOCAB_SIZE)) == NULL) {
      return 1;
    }

    if (trie_init(t->children[idx]) != 0) {
      return 1;
    }
  }

  return trie_insert(t->children[idx], str + 1);
}

bool trie_find(trie_t *t, const char *str) {
  int n = strlen(str);
  if (n == 0) {
    return t->is_word;
  }

  char l = str[0];
  int idx = l - 'a';

  if (t->children[idx] == NULL) {
    return false;
  }

  return trie_find(t->children[idx], str + 1);
}

void trie_delete(trie_t *t, const char *str) {
  int n = strlen(str);
  if (n == 0) {
    t->is_word = false;
    return;
  }

  char l = str[0];
  int idx = l - 'a';

  if (t->children[idx] == NULL) {
    return;
  }

  return trie_delete(t->children[idx], str + 1);
}


int main() {
  trie_t *t = malloc(sizeof(trie_t));
  trie_init(t);

  assert(trie_insert(t, "foo") == 0);
  assert(trie_insert(t, "fool") == 0);
  assert(trie_insert(t, "foolish") == 0);
  assert(trie_insert(t, "bar") == 0);

  assert( trie_find(t, "foo"));
  assert( trie_find(t, "foolish"));
  assert(!trie_find(t, "fo"));
  assert(!trie_find(t, "fork"));

  trie_delete(t, "fool");
  assert(!trie_find(t, "fool"));

  trie_deinit(t);
  return 0;
}
