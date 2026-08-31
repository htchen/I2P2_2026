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

static void list_destroy(Node** head) {
  while (head != NULL && *head != NULL) {
    (void)list_erase(head, 0);
  }
}

int main(void) {
  Node* head = NULL;
  (void)list_insert(&head, 0, 20);
  (void)list_insert(&head, 0, 10);
  (void)list_insert(&head, 2, 30);
  (void)list_erase(&head, 1);
  for (const Node* node = head; node != NULL; node = node->next) {
    printf("%d%c", node->value, node->next == NULL ? '\n' : ' ');
  }
  list_destroy(&head);
  return 0;
}
