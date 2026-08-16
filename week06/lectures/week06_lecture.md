# Lecture 6 — Recursion and Binary Trees

> October 13, 2026 · Source lineage: the legacy recursion and binary-tree notes
> and the 2025 Week 3–4 notebooks

## Learning objectives

By the end of this lecture, you should be able to:

1. Write a recursive function from a decreasing problem measure.
2. Trace recursive calls and stack frames.
3. Represent and traverse a binary tree.
4. Build and destroy an owning tree safely.
5. Relate tree shape to time and stack-space complexity.

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | How do recursive contracts guarantee progress? | Trace and implement numeric/list recursion |
| 2 | How does tree shape determine traversal order? | Draw trees and derive traversal/query functions |
| 3 | How are trees built, reconstructed, tested, and destroyed? | Complete a BST module and verify ownership |

## Hour 1 — Recursive reasoning and the call stack

### 1. A recursive proof becomes a recursive program

A correct recursive design needs:

1. a **contract** for what the function returns;
2. one or more **base cases** solved directly;
3. a recursive case using smaller instances;
4. a **decreasing measure** that guarantees reaching a base case.

```c
unsigned long long factorial(unsigned int n)
{
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}
```

The measure `n` decreases. This function is structurally correct but its result
still overflows quickly; recursion does not remove numeric limits.

### 2. Stack frames

Each active call has its own parameters, local variables, and return location.
Tracing `factorial(4)`:

```text
factorial(4)
  4 * factorial(3)
        3 * factorial(2)
              2 * factorial(1)
                    1
```

After the base case, calls finish in reverse order. Recursion depth consumes
stack space. A missing base case or non-decreasing argument eventually exhausts
the stack rather than producing a Python `RecursionError` with a portable limit.

### Euclid's algorithm

```c
int gcd(int a, int b)
{
    if (b == 0) return a < 0 ? -a : a;
    return gcd(b, a % b);
}
```

Contract: for supported inputs, return the nonnegative greatest common divisor.
The absolute value of the second argument decreases after reduction. Discuss
the `INT_MIN` limitation and decide whether the production interface should use
a wider type or reject that case.

### Fast exponentiation

The legacy recursion notes contrasted linear and logarithmic recursion:

```c
long long power(long long base, unsigned int exponent)
{
    if (exponent == 0) return 1;
    long long half = power(base, exponent / 2);
    if (exponent % 2 == 0) return half * half;
    return half * half * base;
}
```

The decreasing measure is `exponent`; depth is O(log exponent). Arithmetic may
still overflow. Trace `power(3, 5)` and draw when multiplication happens on the
unwinding path.

### Recursion versus iteration

Use recursion when it exposes the structure of the proof or data. Use iteration
when the state transition is simpler and deep recursion risks the stack. Tail
calls are not guaranteed to be optimized by C, so rewriting a linear recursive
loop may be necessary for unbounded input.

### Hour 1 checkpoint

For Fibonacci, binary search, and linked-list length, identify the problem
measure, number of recursive calls, maximum depth, and overlapping subproblems.
Explain why naive Fibonacci is exponential while the other two need not be.

## Hour 2 — Recursive data and traversal design

### 3. Recursion follows recursive data

```c
struct TreeNode {
    int value;
    struct TreeNode *left;
    struct TreeNode *right;
};
```

A tree is either empty (`NULL`) or a node with two smaller trees. The data
definition suggests the program structure.

### Traversal orders

```c
void preorder(const struct TreeNode *node)
{
    if (node == NULL) return;
    printf("%d ", node->value);
    preorder(node->left);
    preorder(node->right);
}

void inorder(const struct TreeNode *node)
{
    if (node == NULL) return;
    inorder(node->left);
    printf("%d ", node->value);
    inorder(node->right);
}

void postorder(const struct TreeNode *node)
{
    if (node == NULL) return;
    postorder(node->left);
    postorder(node->right);
    printf("%d ", node->value);
}
```

- Preorder handles the root before its children.
- Inorder handles the root between left and right.
- Postorder handles children before the root.

The appropriate order follows the task, not a memorized ranking.

### 4. Aggregate queries

```c
size_t tree_size(const struct TreeNode *node)
{
    if (node == NULL) return 0;
    return 1 + tree_size(node->left) + tree_size(node->right);
}

int tree_height(const struct TreeNode *node)
{
    if (node == NULL) return -1; /* height measured in edges */
    int left = tree_height(node->left);
    int right = tree_height(node->right);
    return 1 + (left > right ? left : right);
}
```

Define conventions explicitly. Some books call an empty tree height `0` and a
leaf height `1`; this note measures edges, so they are `-1` and `0`.

### Evaluate a tree-shaped expression

Before the compiler lecture, treat operator trees as ordinary binary trees:

```c
enum ExprKind { ExprNumber, ExprAdd, ExprMultiply };

struct Expr {
    enum ExprKind kind;
    int number;
    struct Expr *left;
    struct Expr *right;
};

int expr_evaluate(const struct Expr *expr)
{
    if (expr->kind == ExprNumber) return expr->number;
    int left = expr_evaluate(expr->left);
    int right = expr_evaluate(expr->right);
    return expr->kind == ExprAdd ? left + right : left * right;
}
```

The representation changes the base case: unlike a general binary tree, a
well-formed operator expression never uses `NULL` as a complete expression.
Every recursive algorithm begins from its data invariant.

### Derive, do not memorize, traversal order

Choose the moment to process the root:

- print an outline before children → preorder;
- print BST values in order → inorder;
- compute a directory's size after child sizes → postorder;
- release a resource after owned children → postorder;

### Hour 2 board exercise

For a seven-node tree, derive preorder, inorder, and postorder sequences. Then
work backward: given preorder and inorder with unique labels, circle the root,
partition both sequences, and repeat on each subtree. Include an empty subtree
and a one-child node rather than using only a perfect tree.

## Hour 3 — BST construction, reconstruction, and ownership

### 5. Build a binary search tree

For a binary search tree (BST), all values in the left subtree are smaller and
all values in the right subtree are larger under our no-duplicates policy.

```c
#include <stdlib.h>

int bst_insert(struct TreeNode **link, int value)
{
    while (*link != NULL) {
        if (value < (*link)->value) {
            link = &(*link)->left;
        } else if (value > (*link)->value) {
            link = &(*link)->right;
        } else {
            return 1; /* already present */
        }
    }

    struct TreeNode *node = malloc(sizeof *node);
    if (node == NULL) return 0;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    *link = node;
    return 1;
}
```

This iterative version reuses the pointer-to-pointer technique from linked
lists. A recursive insertion is also natural; both must preserve the BST
invariant and handle allocation failure.

### 6. Destruction is postorder

```c
void tree_destroy(struct TreeNode *node)
{
    if (node == NULL) return;
    tree_destroy(node->left);
    tree_destroy(node->right);
    free(node);
}
```

Free the children before the parent because their addresses are stored in the
parent. This is postorder traversal applied to resource ownership.

At the owning call site:

```c
tree_destroy(root);
root = NULL;
```

### 7. Reconstructing from traversals

For distinct values, preorder gives the root first and inorder tells which
values belong to the left and right subtrees.

```text
preorder: A B D E C F
inorder:  D B E A C F

root = A
left inorder  = D B E   -> left preorder  = B D E
right inorder = C F     -> right preorder = C F
```

The same argument recursively reconstructs both subtrees. A simple linear
search for each root gives O(n²) worst-case time; an index map can reduce it to
O(n), provided values are unique.

### 8. Shape determines cost

Every full traversal visits `n` nodes: O(n) time. Its additional stack use is
O(h), where `h` is tree height.

- Balanced tree: `h = O(log n)`.
- Completely skewed tree: `h = O(n)`.

BST search and insertion are O(h), not automatically O(log n). A plain BST can
degrade into a linked list when values arrive in sorted order.

### Search and extrema

```c
const struct TreeNode *bst_find(const struct TreeNode *node, int target)
{
    while (node != NULL) {
        if (target < node->value) node = node->left;
        else if (target > node->value) node = node->right;
        else return node;
    }
    return NULL;
}

const struct TreeNode *bst_minimum(const struct TreeNode *node)
{
    if (node == NULL) return NULL;
    while (node->left != NULL) node = node->left;
    return node;
}
```

Both return borrowed pointers. They do not transfer ownership, and a later tree
mutation or destruction may invalidate them.

### Deletion cases

BST removal separates three cases:

1. leaf: replace its incoming link with `NULL`;
2. one child: replace its incoming link with that child;
3. two children: replace the value with the inorder successor/predecessor, then
   remove that replacement node from its original one-child position.

Implement deletion with a pointer-to-pointer to the root link. Require students
to draw ownership before coding; most deletion defects are wrong link updates or
use-after-free, not incorrect comparisons.

### Reconstruction implementation plan

Use half-open ranges in the traversal arrays. A helper receives preorder range,
inorder range, and an output tree link. Allocate the root only after validating
that it appears in the inorder range. If either subtree fails, destroy any
partial children and the root before returning failure.

### Hour 3 integration test

Insert a sequence, verify inorder ordering and recorded size, search present and
absent values, remove a leaf/one-child/two-child root, and destroy the tree. Run
the same operation sequence for a balanced insertion order and sorted insertion
order; compare observed height and maximum recursion depth.

### 9. Tree testing strategy

Include at least:

- empty tree;
- one node;
- only-left and only-right chains;
- balanced shape;
- duplicate insertion under the chosen policy;
- allocation failure if the design permits injection;
- traversal sequences and size/height agreement;
- destruction under AddressSanitizer.

Useful properties include “inorder output of a BST is strictly increasing” and
“size after inserting a new distinct value increases by exactly one.”

## Check yourself

1. Identify the base case and decreasing measure in `tree_size`.
2. Why is preorder unsuitable for `tree_destroy`?
3. Draw the BST produced by `4, 2, 6, 1, 3, 5, 7`.
4. How does inserting sorted input affect recursion depth?
5. Reconstruct a tree from `preorder: 2 1 3` and `inorder: 1 2 3`.

## Summary

- Recursive code should follow a contract, base case, and decreasing measure.
- Tree structure naturally produces recursive algorithms.
- Traversal order describes when the root is processed relative to children.
- Ownership determines destruction order.
- Complexity depends on tree height as well as node count.
