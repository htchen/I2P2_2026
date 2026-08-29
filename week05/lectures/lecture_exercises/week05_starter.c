#include <stdbool.h>
#include <stddef.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

bool list_insert(Node **head, size_t index, int value) {
    /* TODO: find the owning link at index and install one new node. */
    (void)head;
    (void)index;
    (void)value;
    return false;
}

bool list_erase(Node **head, size_t index) {
    /* TODO: unlink and free exactly one node when index exists. */
    (void)head;
    (void)index;
    return false;
}

void list_destroy(Node **head) {
    /* TODO: release every node and leave *head == NULL. */
    (void)head;
}
