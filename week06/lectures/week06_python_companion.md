# Python Contrast Companion for Week 6

Read this companion beside the [Week 6 lecture
notes](week06_lecture.md). Recursive decomposition, traversal order,
backtracking, tree reconstruction, and binary-search-tree search translate
strongly. Fixed-width overflow, C stack limits, pointer-to-pointer insertion,
and explicit postorder destruction do not.

## Relationship labels

- **Direct:** the recursive definition or tree algorithm transfers closely.
- **Same algorithm, different result interface:** Python returns lists/values
  where C prints or fills structures.
- **Closest Python model:** object references represent edges without C pointer
  ownership.
- **No faithful equivalent:** explicit allocation/destruction or pointer-link
  mutation is absent.

## Coverage map

Every fenced example in the Week 6 note is accounted for below.

| Source example | Relationship | Companion treatment |
|---|---|---|
| Recursive factorial | Direct recurrence | Add a nonnegative precondition and note Python's recursion limit/arbitrary integers |
| Factorial call trace | Direct | Preserve the same pending multiplication frames |
| Stack-frame Mermaid graph | Conceptual | Reuse call dependencies, not ABI frame layout |
| Recursive Euclidean GCD | Direct for intended domain | Normalize signs and retain the decreasing second argument |
| Fast exponentiation | Direct algorithm | Preserve divide/recombine; Python does not overflow fixed-width integers |
| Towers of Hanoi | Direct recursion | Return a move list rather than print from the algorithm |
| N-queens choose/mark/recurse/unmark | Direct backtracking invariant | Use sets and explicit undo |
| `TreeNode` structure | Closest model | Dataclass attributes represent child edges |
| Traversal-order Mermaid tree | Direct structure | Use the same tree for preorder/inorder/postorder |
| Three traversal functions | Direct | Return value lists for easy testing |
| Tree size and height | Direct | Preserve empty-tree size zero and edge-height minus one |
| BST insertion through `TreeNode**` | Same search, different link mutation | Return/create a root and assign child attributes |
| Postorder `tree_destroy` | No explicit-release equivalent | Optionally detach children; Python manages unreachable nodes |
| Destroy then null root | Partial | Rebind the caller's root name to `None`; external aliases may remain |
| Preorder/inorder reconstruction trace | Direct | Split traversal ranges around the root |
| Ambiguous two-node shapes | Direct ambiguity | Preorder alone still cannot determine shape |
| BST find and minimum | Direct search | Return a node or `None` |

## Recurrence, frames, and limits

```python
def factorial(n: int) -> int:
    if n < 0:
        raise ValueError("factorial requires a nonnegative integer")
    if n <= 1:
        return 1
    return n * factorial(n - 1)
```

The `factorial(4)` trace has the same pending multiplications. Python raises
`RecursionError` when recursion exceeds the interpreter limit; C may exhaust
the call stack with undefined or implementation-specific consequences. Python
integers grow, so it does not model `unsigned long long` overflow.

The Mermaid edges describe “this call waits for that call.” They do not claim
that Python frames have the C ABI layout, register saves, or stack size shown in
an assembly observation.

Euclid's algorithm remains direct:

```python
def greatest_common_divisor(a: int, b: int) -> int:
    if b == 0:
        return abs(a)
    return greatest_common_divisor(b, a % b)
```

For negative operands, Python and C define remainder signs differently. The
absolute final result is the intended shared contract; keep intermediate-sign
reasoning language-specific.

Fast exponentiation also retains its structure:

```python
def power(base: int, exponent: int) -> int:
    if exponent < 0:
        raise ValueError("this integer version requires a nonnegative exponent")
    if exponent == 0:
        return 1
    half = power(base, exponent // 2)
    if exponent % 2 == 0:
        return half * half
    return half * half * base
```

The O(log exponent) call structure transfers. Python's result does not prove
that the C multiplications fit in `long long`.

## Hanoi and backtracking

Separating computation from output makes the Hanoi translation easy to test:

```python
def hanoi(
    n: int, source: str, temporary: str, destination: str
) -> list[tuple[int, str, str]]:
    if n < 0:
        raise ValueError("negative disk count")
    if n == 0:
        return []
    return (
        hanoi(n - 1, source, destination, temporary)
        + [(n, source, destination)]
        + hanoi(n - 1, temporary, source, destination)
    )
```

The order matches the C print sequence. Building lists costs additional memory;
a generator or callback can stream moves when that matters.

The N-queens invariant translates directly:

```python
def count_queens(size: int) -> int:
    if size < 0:
        raise ValueError("negative board size")
    columns: set[int] = set()
    descending: set[int] = set()
    ascending: set[int] = set()

    def search(row: int) -> int:
        if row == size:
            return 1
        result = 0
        for column in range(size):
            down = row - column
            up = row + column
            if column in columns or down in descending or up in ascending:
                continue
            columns.add(column)
            descending.add(down)
            ascending.add(up)
            result += search(row + 1)
            columns.remove(column)
            descending.remove(down)
            ascending.remove(up)
        return result

    return search(0)
```

Choose, mark, recurse, and undo are the same proof steps. Python sets replace
fixed arrays or bit masks without changing the backtracking invariant.

## Tree representation and traversals

```python
from __future__ import annotations
from dataclasses import dataclass


@dataclass
class TreeNode:
    value: int
    left: TreeNode | None = None
    right: TreeNode | None = None
```

The attributes express the same tree shape. They do not enforce unique
ownership, and Python manages object storage.

```python
def preorder(node: TreeNode | None) -> list[int]:
    if node is None:
        return []
    return [node.value] + preorder(node.left) + preorder(node.right)


def inorder(node: TreeNode | None) -> list[int]:
    if node is None:
        return []
    return inorder(node.left) + [node.value] + inorder(node.right)


def postorder(node: TreeNode | None) -> list[int]:
    if node is None:
        return []
    return postorder(node.left) + postorder(node.right) + [node.value]
```

Returning lists makes the order explicit, though repeated concatenation adds
cost. A production traversal can append into one output list or yield values.

Aggregate queries translate line-for-line:

```python
def tree_size(node: TreeNode | None) -> int:
    if node is None:
        return 0
    return 1 + tree_size(node.left) + tree_size(node.right)


def tree_height(node: TreeNode | None) -> int:
    if node is None:
        return -1
    return 1 + max(tree_height(node.left), tree_height(node.right))
```

## BST insertion, search, and minimum

Python cannot take a `TreeNode**` designating either the root variable or a
child field. Returning the possibly new subtree root is the usual recursive
rewrite:

```python
def bst_insert(node: TreeNode | None, value: int) -> TreeNode:
    if node is None:
        return TreeNode(value)
    if value < node.value:
        node.left = bst_insert(node.left, value)
    elif value > node.value:
        node.right = bst_insert(node.right, value)
    return node
```

The search comparisons and duplicate policy are the same. Construction raises
on failure rather than returning a Boolean allocation status.

```python
def bst_find(node: TreeNode | None, target: int) -> TreeNode | None:
    while node is not None:
        if target < node.value:
            node = node.left
        elif target > node.value:
            node = node.right
        else:
            return node
    return None


def bst_minimum(node: TreeNode | None) -> TreeNode | None:
    if node is None:
        return None
    while node.left is not None:
        node = node.left
    return node
```

## Destruction and root rebinding

Python needs no `free` traversal. A caller can remove its root reference:

```python
root = None
```

If no other references reach the nodes, they become reclaimable. If another
name reaches a subtree, it remains alive. An explicit recursive function can set
child attributes to `None`, but that only changes reachability; it is not a
translation of postorder deallocation and cannot prove C cleanup correctness.

## Reconstructing from traversals

```python
def reconstruct(
    preorder_values: list[int], inorder_values: list[int]
) -> TreeNode | None:
    if (
        len(preorder_values) != len(inorder_values)
        or len(set(preorder_values)) != len(preorder_values)
        or set(preorder_values) != set(inorder_values)
    ):
        raise ValueError("traversals must contain the same unique values")
    if not preorder_values:
        return None
    root_value = preorder_values[0]
    middle = inorder_values.index(root_value)
    left_size = middle
    return TreeNode(
        root_value,
        reconstruct(
            preorder_values[1 : 1 + left_size], inorder_values[:middle]
        ),
        reconstruct(
            preorder_values[1 + left_size :],
            inorder_values[middle + 1 :],
        ),
    )
```

This assumes unique values and consistent traversals, exactly the contract that
must be stated for the C reconstruction. Slicing allocates sublists; index-range
parameters avoid that overhead. Preorder alone still permits both two-node
shapes shown in the source note, so Python does not remove the ambiguity.

For the exercise's inorder-plus-postorder form, the root moves from the first
preorder element to the last postorder element. The inorder split still
determines both subtree sizes:

```python
def reconstruct_from_inorder_postorder(
    inorder_values: list[int], postorder_values: list[int]
) -> TreeNode | None:
    if (
        len(inorder_values) != len(postorder_values)
        or len(set(inorder_values)) != len(inorder_values)
        or len(set(postorder_values)) != len(postorder_values)
        or set(inorder_values) != set(postorder_values)
    ):
        raise ValueError("traversals must contain the same unique values")
    if not inorder_values:
        return None
    root_value = postorder_values[-1]
    middle = inorder_values.index(root_value)
    return TreeNode(
        root_value,
        reconstruct_from_inorder_postorder(
            inorder_values[:middle], postorder_values[:middle]
        ),
        reconstruct_from_inorder_postorder(
            inorder_values[middle + 1 :], postorder_values[middle:-1]
        ),
    )
```

## What the Python versions must not hide

- Python recursion limits and arbitrary integers differ from C stack and
  overflow behavior.
- Child attributes are not uniquely owning C pointers.
- Returning a subtree root is not the pointer-to-pointer insertion technique.
- Garbage collection is not postorder `free`.
- Traversal, backtracking, and reconstruction invariants transfer directly and
  make strong reference models for testing.

Runnable versions are collected in [`python_examples.py`](python_examples.py).
