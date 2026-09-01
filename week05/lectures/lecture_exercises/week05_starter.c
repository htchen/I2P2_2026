#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int value;
  struct Node* next;
} Node;

bool list_insert(Node** head, size_t index, int value) {
  /* TODO: find the owning link at index and install one new node. */
  (void)head;
  (void)index;
  (void)value;
  return false;
}

bool list_erase(Node** head, size_t index) {
  /* TODO: unlink and free exactly one node when index exists. */
  (void)head;
  (void)index;
  return false;
}

bool list_reverse_range(Node** head, size_t first, size_t last) {
  /* TODO: reverse [first, last) by rewiring links. Return false, without
     modifying the list, when head is NULL, first > last, or either boundary
     is beyond the list length. */
  (void)head;
  (void)first;
  (void)last;
  return false;
}

void list_destroy(Node** head) {
  /* TODO: release every node and leave *head == NULL. */
  (void)head;
}

static bool list_equals(const Node* head, const int expected[], size_t size) {
  for (size_t index = 0; index < size; ++index) {
    if (head == NULL || head->value != expected[index]) {
      return false;
    }
    head = head->next;
  }
  return head == NULL;
}

static bool check(bool condition, const char* message) {
  if (!condition) {
    fprintf(stderr, "check failed: %s\n", message);
    return false;
  }
  return true;
}

int main(void) {
  Node* head = NULL;
  if (!check(list_insert(&head, 0, 20), "insert into empty list") ||
      !check(list_insert(&head, 0, 10), "insert at head") ||
      !check(list_insert(&head, 2, 30), "insert at end")) {
    list_destroy(&head);
    return 1;
  }

  const int built[] = {10, 20, 30};
  if (!check(list_equals(head, built, 3), "list after insertions") ||
      !check(list_reverse_range(&head, 0, 3), "reverse complete range")) {
    list_destroy(&head);
    return 1;
  }

  const int reversed[] = {30, 20, 10};
  if (!check(list_equals(head, reversed, 3), "list after reversal") ||
      !check(!list_reverse_range(&head, 3, 4),
             "reject out-of-range reversal") ||
      !check(list_reverse_range(&head, 3, 3),
             "accept empty range at the end") ||
      !check(list_erase(&head, 1), "erase middle node")) {
    list_destroy(&head);
    return 1;
  }

  const int erased[] = {30, 10};
  if (!check(list_equals(head, erased, 2), "list after erasure")) {
    list_destroy(&head);
    return 1;
  }

  list_destroy(&head);
  if (!check(head == NULL, "destroy publishes NULL")) {
    return 1;
  }
  puts("all Week 5 starter checks passed");
  return 0;
}
