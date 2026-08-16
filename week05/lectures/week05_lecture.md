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

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | How is a linked structure represented and owned? | Build, print, and validate a list by hand |
| 2 | How can one algorithm update the head or an interior link? | Implement insertion, removal, and reversal with pointer-to-pointer reasoning |
| 3 | When are circular/doubly linked variants justified? | Solve and compare Josephus implementations, then run memory tests |

## Hour 1 — Representation, construction, and ownership

### 1. Why link nodes?

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

### 2. Representation and invariants

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

### 3. Allocate one node safely

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

### Separate payload from structure

The legacy list notes eventually separated list mechanics from stored data. A
node can own a heap-allocated string, borrow a string, or store the bytes inline;
these choices change destruction and copy behavior.

```c
struct StringNode {
    char *owned_text;
    struct StringNode *next;
};
```

If `owned_text` is owned, node creation must duplicate the string and node
destruction must free it before freeing the node. If it is borrowed, the source
string must outlive the list. Never leave this decision implicit.

### 4. Insert at the front

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

### Validate the invariant during development

```c
int list_is_valid(const struct List *list)
{
    size_t observed = 0;
    for (const struct Node *node = list->head;
         node != NULL;
         node = node->next) {
        ++observed;
        if (observed > list->size) return 0; /* cycle or wrong size */
    }
    return observed == list->size;
}
```

This finite check detects many, but not every, malformed representation. Call it
with `assert` at public-operation boundaries while developing. Later compare it
with Floyd's tortoise-and-hare cycle detector, which does not rely on `size`.

### Hour 1 construction trace

Starting from an empty list, push `30`, `20`, then `10`. Draw every allocation
before and after the head update. Repeat with a forced allocation failure on the
third push and prove that the original two-node list remains valid and owned.

## Hour 2 — Link-location algorithms

### 5. A link is a modifiable location

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

### 6. Insert in sorted order

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

### Reverse in place

```c
void list_reverse(struct List *list)
{
    struct Node *reversed = NULL;
    struct Node *remaining = list->head;

    while (remaining != NULL) {
        struct Node *next = remaining->next;
        remaining->next = reversed;
        reversed = remaining;
        remaining = next;
    }
    list->head = reversed;
}
```

Loop invariant: `reversed` owns the already processed prefix in reverse order;
`remaining` owns the untouched suffix; together they contain exactly the
original nodes, with no node reachable from both.

### Remove all matching nodes

Extend the pointer-to-pointer pattern:

```c
size_t list_remove_all(struct List *list, int target)
{
    size_t removed_count = 0;
    struct Node **link = &list->head;
    while (*link != NULL) {
        if ((*link)->value == target) {
            struct Node *removed = *link;
            *link = removed->next;
            free(removed);
            --list->size;
            ++removed_count;
        } else {
            link = &(*link)->next;
        }
    }
    return removed_count;
}
```

After removal, do not advance `link`: it already designates the next link to
inspect. This is the key case when adjacent nodes match.

### Hour 2 studio

Implement and test:

1. `list_find` returning a borrowed node pointer;
2. `list_insert_after` taking a borrowed position;
3. `list_clone` returning a deep copy with the same order;
4. `list_equal` without exposing nodes to the caller.

Define behavior when the position does not belong to the list. Decide whether
the API can detect that efficiently or must state it as a precondition.

## Hour 3 — Traversal variants, circular lists, and Josephus

### 7. Traversal and read-only borrowing

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

### 8. Destroy the entire list

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

### 9. Complexity and representation choice

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

### 10. Circular lists and Josephus

In a circular list, the last node points back to the first instead of `NULL`.
This can model repeated elimination in the Josephus problem. It also changes
the invariant and termination condition: traversal must remember the starting
node or a count, and destruction must deliberately break or walk the cycle.

Use a circular list because the problem is circular, not merely because it is an
interesting structure. The Josephus problem also has array and mathematical
solutions with different tradeoffs.

### Circular-list representation

A useful representation stores a `tail` whose `next` is the head:

```c
struct CircularList {
    struct Node *tail;
    size_t size;
};

/* empty: tail == NULL
   nonempty: tail->next is head, and size links return to head */
```

Insertion after the tail is O(1), as is access to the head. Destruction must use
the stored size or first break the cycle; a `while (node != NULL)` loop never
terminates.

### Josephus comparison

For `n` participants and step `k`, compare:

1. erase from an array/vector-like representation—simple indexing, O(n²) shifts;
2. circular linked list—O(nk) link steps with O(1) removal after the predecessor;
3. recurrence `J(1,k)=0`, `J(n,k)=(J(n-1,k)+k) mod n`—O(n) time and O(1) space
   iteratively, but returns only the survivor unless extended.

The structure-simulation version is still valuable when the complete
elimination order is required. Algorithm selection follows the requested output.

### Doubly linked lists

A doubly linked node adds `previous`. Every mutation must update two directions;
the invariant requires `node->next->previous == node` and
`node->previous->next == node` where neighbors exist. It enables O(1) removal
from a known node without searching for its predecessor but adds memory and more
ways to corrupt links.

### Hour 3 verification

Run empty, singleton, adjacent-removal, head/tail, and full-destruction cases
under AddressSanitizer. For the circular version, additionally test `k = 1`,
`k > n`, and repeated wraparound. Compare the elimination order with a simple
array reference implementation on small `n`.

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
