#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int value;
  struct Node* next;
} Node;

static bool list_insert(Node** head, size_t index, int value) {
  if (head == NULL) {
    return false;
  }
  Node** link = head;
  while (index > 0 && *link != NULL) {
    link = &(*link)->next;
    --index;
  }
  if (index != 0) {
    return false;
  }
  Node* node = malloc(sizeof(*node));
  if (node == NULL) {
    return false;
  }
  *node = (Node){value, *link};
  *link = node;
  return true;
}

static bool list_erase(Node** head, size_t index) {
  if (head == NULL) {
    return false;
  }
  Node** link = head;
  while (index > 0 && *link != NULL) {
    link = &(*link)->next;
    --index;
  }
  if (*link == NULL) {
    return false;
  }
  Node* removed = *link;
  *link = removed->next;
  free(removed);
  return true;
}

static bool list_reverse_range(Node** head, size_t first, size_t last) {
  if (head == NULL || first > last) {
    return false;
  }

  Node** first_link = head;
  for (size_t index = 0; index < first; ++index) {
    if (*first_link == NULL) {
      return false;
    }
    first_link = &(*first_link)->next;
  }

  Node* after = *first_link;
  for (size_t count = last - first; count > 0; --count) {
    if (after == NULL) {
      return false;
    }
    after = after->next;
  }

  Node* current = *first_link;
  Node* reversed = after;
  for (size_t count = last - first; count > 0; --count) {
    Node* next = current->next;
    current->next = reversed;
    reversed = current;
    current = next;
  }
  *first_link = reversed;
  return true;
}

static void list_destroy(Node** head) {
  while (head != NULL && *head != NULL) {
    (void)list_erase(head, 0);
  }
}

int main(void) {
  Node* head = NULL;
  if (!list_insert(&head, 0, 20) || !list_insert(&head, 0, 10) ||
      !list_insert(&head, 2, 30)) {
    fputs("could not build list\n", stderr);
    list_destroy(&head);
    return 1;
  }
  if (!list_reverse_range(&head, 0, 3)) {
    fputs("could not reverse list segment\n", stderr);
    list_destroy(&head);
    return 1;
  }
  if (list_reverse_range(&head, 3, 4) ||
      !list_reverse_range(&head, 3, 3)) {
    fputs("range validation failed\n", stderr);
    list_destroy(&head);
    return 1;
  }
  if (!list_erase(&head, 1)) {
    fputs("expected list element was absent\n", stderr);
    list_destroy(&head);
    return 1;
  }
  for (const Node* node = head; node != NULL; node = node->next) {
    printf("%d%c", node->value, node->next == NULL ? '\n' : ' ');
  }
  list_destroy(&head);
  return 0;
}
