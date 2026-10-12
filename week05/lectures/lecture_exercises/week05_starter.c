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
      !check(list_insert(&head, 2, 40), "insert at end") ||
      !check(list_insert(&head, 2, 30), "insert in middle")) {
    list_destroy(&head);
    return 1;
  }

  const int built[] = {10, 20, 30, 40};
  if (!check(list_equals(head, built, 4), "list after insertions") ||
      !check(!list_insert(&head, 5, 50), "reject out-of-range insertion") ||
      !check(list_equals(head, built, 4),
             "failed insertion leaves list unchanged") ||
      !check(list_reverse_range(&head, 1, 2), "accept one-element reversal") ||
      !check(list_equals(head, built, 4), "one-element reversal is a no-op") ||
      !check(list_reverse_range(&head, 1, 3), "reverse a middle range")) {
    list_destroy(&head);
    return 1;
  }

  const int middle_reversed[] = {10, 30, 20, 40};
  if (!check(list_equals(head, middle_reversed, 4),
             "list after middle reversal") ||
      !check(!list_reverse_range(NULL, 0, 0), "reject a NULL head link") ||
      !check(!list_reverse_range(&head, 3, 2), "reject reversed boundaries") ||
      !check(!list_reverse_range(&head, 4, 5),
             "reject boundary beyond list length") ||
      !check(!list_reverse_range(&head, 5, 5),
             "reject empty range beyond list length") ||
      !check(list_reverse_range(&head, 4, 4),
             "accept empty range at the end") ||
      !check(list_equals(head, middle_reversed, 4),
             "invalid reversals leave list unchanged") ||
      !check(list_reverse_range(&head, 0, 4), "reverse complete range")) {
    list_destroy(&head);
    return 1;
  }

  const int fully_reversed[] = {40, 20, 30, 10};
  if (!check(list_equals(head, fully_reversed, 4),
             "list after complete reversal") ||
      !check(list_erase(&head, 0), "erase head node") ||
      !check(list_erase(&head, 1), "erase middle node") ||
      !check(list_erase(&head, 1), "erase tail node")) {
    list_destroy(&head);
    return 1;
  }

  const int one_node[] = {20};
  if (!check(list_equals(head, one_node, 1), "list after three erasures") ||
      !check(!list_erase(&head, 1), "reject out-of-range erasure") ||
      !check(list_equals(head, one_node, 1),
             "failed erasure leaves list unchanged") ||
      !check(list_erase(&head, 0), "erase only node") ||
      !check(head == NULL, "erasing only node publishes NULL") ||
      !check(!list_erase(&head, 0), "reject erasure from empty list")) {
    list_destroy(&head);
    return 1;
  }

  if (!check(list_insert(&head, 0, 1), "rebuild first node") ||
      !check(list_insert(&head, 1, 2), "rebuild second node")) {
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
