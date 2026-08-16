# Lecture 5 — Linked Lists and Pointer-to-Pointer Techniques

> October 6, 2026 · Source lineage: the legacy linked-list notes and the 2025
> Week 1–3 notebooks; examples were consolidated around explicit ownership

## Learning objectives

By the end of this lecture, you should be able to:

1. Represent a singly linked list with dynamically allocated nodes.
2. Implement insertion, removal, traversal, and destruction.
3. Use a pointer-to-pointer to update a link uniformly.
4. State list invariants and ownership rules.
5. Compare linked-list and array operation costs.

## 1. Why link nodes?

An array stores elements contiguously. A linked list stores each element in a
node that points to the next node.

```text
head
  |
  v
+-------+------+    +-------+------+    +-------+------+
|  10   |   o--+--->|  20   |   o--+--->|  30   | NULL |
+-------+------+    +-------+------+    +-------+------+
```

This permits insertion without shifting later elements, but costs one pointer
per node, non-contiguous memory access, and linear-time indexing.

## 2. Representation and invariants

```c
struct Node {
    int value;
    struct Node *next;
};

struct List {
    struct Node *head;
    size_t size;
};
```

Our representation invariant is:

- `head == NULL` exactly when `size == 0`;
- following `next` reaches exactly `size` nodes and then `NULL`;
- every reachable node is owned by this list;
- no node is reachable twice (the list has no cycle).

Initialize every link before publishing the node into the list.

```c
void list_init(struct List *list)
{
    list->head = NULL;
    list->size = 0;
}
```

## 3. Allocate one node safely

```c
#include <stdlib.h>

static struct Node *node_create(int value, struct Node *next)
{
    struct Node *node = malloc(sizeof *node);
    if (node == NULL) return NULL;
    node->value = value;
    node->next = next;
    return node;
}
```

The function returns ownership of a new node or reports failure with `NULL`.
Because it is `static`, it is a private implementation detail of `list.c`.

## 4. Insert at the front

```c
int list_push_front(struct List *list, int value)
{
    struct Node *node = node_create(value, list->head);
    if (node == NULL) return 0;
    list->head = node;
    ++list->size;
    return 1;
}
```

Order matters: allocate first, connect the new node to the old head, and only
then replace `head`. If allocation fails, the original list is unchanged.

## 5. A link is a modifiable location

Removing a node usually requires either changing `list->head` or changing a
previous node's `next`. A pointer-to-pointer lets one loop treat both as “the
link that points to the current node.”

```c
int list_remove_first(struct List *list, int target)
{
    struct Node **link = &list->head;

    while (*link != NULL && (*link)->value != target) {
        link = &(*link)->next;
    }

    if (*link == NULL) return 0;

    struct Node *removed = *link;
    *link = removed->next;
    free(removed);
    --list->size;
    return 1;
}
```

Trace three cases:

1. the target is the head;
2. the target is in the middle;
3. the target does not exist.

There is no special head-removal branch because `link` initially points to the
head field itself.

## 6. Insert in sorted order

```c
int list_insert_sorted(struct List *list, int value)
{
    struct Node **link = &list->head;
    while (*link != NULL && (*link)->value < value) {
        link = &(*link)->next;
    }

    struct Node *node = node_create(value, *link);
    if (node == NULL) return 0;
    *link = node;
    ++list->size;
    return 1;
}
```

The loop invariant is: every node before `*link` has value less than `value`,
and `link` is the exact location that must be updated for insertion.

## 7. Traversal and read-only borrowing

```c
void list_print(const struct List *list, FILE *stream)
{
    for (const struct Node *node = list->head;
         node != NULL;
         node = node->next) {
        fprintf(stream, "%d%s", node->value,
                node->next == NULL ? "\n" : " -> ");
    }
}
```

The function borrows the list and does not mutate it. The local traversal
pointer is non-owning; it must never be passed to `free`.

## 8. Destroy the entire list

```c
void list_clear(struct List *list)
{
    struct Node *node = list->head;
    while (node != NULL) {
        struct Node *next = node->next;
        free(node);
        node = next;
    }
    list->head = NULL;
    list->size = 0;
}
```

Save `next` **before** freeing the node. Reading `node->next` after `free(node)`
would be a use-after-free.

## 9. Complexity and representation choice

| Operation | Dynamic array | Singly linked list |
|-----------|---------------|--------------------|
| Index `i` | O(1) | O(i) |
| Push front | O(n) | O(1) |
| Insert after known position | O(n) shifts | O(1) |
| Find a value | O(n) | O(n) |
| Cache locality | Good | Usually poor |
| Per-element overhead | None | One link and allocator metadata |

Big-O does not say the list is automatically faster. For many workloads,
contiguous arrays win because allocation and memory locality matter.

## 10. Circular lists and Josephus

In a circular list, the last node points back to the first instead of `NULL`.
This can model repeated elimination in the Josephus problem. It also changes
the invariant and termination condition: traversal must remember the starting
node or a count, and destruction must deliberately break or walk the cycle.

Use a circular list because the problem is circular, not merely because it is an
interesting structure. The Josephus problem also has array and mathematical
solutions with different tradeoffs.

## Check yourself

1. Draw `link`, `*link`, and `**link` during removal of the second node.
2. Why is a traversal pointer not an owner?
3. Add `list_pop_front` and state its failure behavior.
4. Which invariant detects an accidental cycle?
5. Run insertion and removal tests under AddressSanitizer.

## Summary

- A linked list is a chain of separately allocated nodes.
- The list owns every reachable node and must release each exactly once.
- A pointer-to-pointer uniformly represents the link being inspected or changed.
- Mutation should preserve invariants even when allocation fails.
- Choose a representation using access patterns and real costs, not Big-O alone.
