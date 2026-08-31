# Python Contrast Companion for Week 5

Read this companion beside the [Week 5 lecture
notes](week05_lecture.md). Python can represent linked nodes and run the same
traversal algorithms, but it does not expose the address of a link field. The
pointer-to-pointer technique and explicit node ownership therefore remain C
concepts even when a Python list model produces the same sequence.

## Relationship labels

- **Direct:** the sequence algorithm transfers closely.
- **Closest Python model:** object attributes express links but not typed pointer
  storage or link addresses.
- **No faithful equivalent:** the C operation depends on `Node**`, `malloc`,
  `free`, or explicit ownership.
- **Diagram/invariant:** the conceptual structure transfers, while its storage
  interpretation changes.

## Coverage map

Every fenced example in the Week 5 note is accounted for below.

| Source example | Relationship | Companion treatment |
|---|---|---|
| Three-node text diagram | Diagram/invariant | Reuse as object reachability, not byte/address layout |
| `Node` and `List` structures | Closest model | Dataclasses hold a head reference and explicit size |
| `list_init` | Same initial state | Constructor/defaults establish `head is None` and size zero |
| `node_create` with `malloc` | No allocation equivalent | Construct `Node(value, next)`; Python reports allocation failure differently |
| Owning `StringNode` payload | No explicit string ownership | A node holds a string reference; exclusive ownership is not enforced |
| Push front | Direct link algorithm | Construct a node, redirect `head`, increment size |
| List invariant validator | Direct traversal, different lifetime | Count nodes and detect cycles by identity |
| Head-link Mermaid diagram | Diagram, no `Node**` equivalent | Explain the head attribute as a mutation target |
| Interior-link Mermaid diagram | Diagram, no `Node**` equivalent | Explain a node's `next` attribute as a mutation target |
| Previous/current removal | Direct algorithm | Preserve the head/interior branch |
| Pointer-to-pointer removal | No faithful equivalent | Contrast with Python's explicit branch or a sentinel node |
| Sorted insertion through `Node**` | Direct order goal, different mutation | Use previous/current and update an attribute |
| In-place reversal | Direct | Reassign `next` attributes with the same three-reference invariant |
| Remove all through `Node**` | Direct result, different mutation | Preserve a previous/current traversal |
| Link-location invariant text | C-specific mechanism | Restate as “the owner attribute that must change,” without claiming its address is available |
| Prefix/range/suffix invariant | Direct algorithmic invariant | Reuse for subrange rewiring |
| Read-only traversal/printing | Direct | Iterate over nodes and join their values |
| `list_clear` with `free` | No explicit release equivalent | Detach the head and size; optional link breaking only changes reachability |
| Circular-list representation | Closest model | Store a tail and close the cycle; Python does not enforce ownership or prompt reclamation |
| Iterative Josephus recurrence | Direct | Preserve the zero-based recurrence and one-based result |

## Nodes and reachability

```python
from __future__ import annotations
from dataclasses import dataclass


@dataclass
class Node:
    value: int
    next: Node | None = None


@dataclass
class LinkedList:
    head: Node | None = None
    size: int = 0
```

The text diagram still communicates that `head` reaches the node `10`, whose
`next` reaches `20`, and so on. A Python attribute contains an object reference,
not a C `Node*` whose address can be taken and changed through `Node**`.

Construction establishes the empty state directly, replacing `list_init`:

```python
values = LinkedList()
assert values.head is None and values.size == 0
```

Node creation is ordinary construction:

```python
node = Node(value=10, next=values.head)
```

There is no checked `malloc` result. Python may raise `MemoryError`, but normal
course code does not use that exception as a line-for-line allocation-failure
contract.

A string node can be written similarly:

```python
@dataclass
class StringNode:
    text: str
    next: StringNode | None = None
```

The node keeps the string reachable. Python does not state that this node owns
the string exclusively, and immutable strings may be shared freely.

## Front insertion and validation

```python
def push_front(values: LinkedList, value: int) -> None:
    values.head = Node(value=value, next=values.head)
    values.size += 1
```

The link change and size invariant are direct. Construction either returns a
node or raises; there is no Boolean allocation result in this interface.

```python
def is_valid(values: LinkedList) -> bool:
    observed = 0
    node = values.head
    seen: set[int] = set()
    while node is not None:
        identity = id(node)
        if identity in seen:
            return False
        seen.add(identity)
        observed += 1
        node = node.next
    return observed == values.size
```

The identity set detects a cycle exactly rather than using size only as a
cutoff. `id` is used as an opaque identity token, not dereferenced as an address.

## Why there is no Python pointer-to-pointer translation

In C, `link` can designate either `list->head` or a node's `next` field and
`*link = replacement` updates that exact pointer object. Python can write
`values.head = replacement` or `previous.next = replacement`, but it cannot
take a general modifiable reference to either attribute.

The conventional previous/current algorithm is therefore the honest rewrite:

```python
def remove_first(values: LinkedList, target: int) -> bool:
    previous: Node | None = None
    current = values.head
    while current is not None and current.value != target:
        previous = current
        current = current.next
    if current is None:
        return False
    if previous is None:
        values.head = current.next
    else:
        previous.next = current.next
    values.size -= 1
    return True
```

This corresponds to the first C removal block. A sentinel node can remove the
head branch in Python, but it is a data-structure redesign—not a translation of
`Node**` or the Torvalds link-location technique.

## Sorted insertion, reversal, and removal of all matches

```python
def insert_sorted(values: LinkedList, value: int) -> None:
    previous: Node | None = None
    current = values.head
    while current is not None and current.value < value:
        previous = current
        current = current.next
    node = Node(value=value, next=current)
    if previous is None:
        values.head = node
    else:
        previous.next = node
    values.size += 1
```

The ordering invariant transfers, but Python again selects the owner attribute
with an explicit branch.

Reversal is nearly line-for-line at the algorithm level:

```python
def reverse(values: LinkedList) -> None:
    reversed_head: Node | None = None
    remaining = values.head
    while remaining is not None:
        next_node = remaining.next
        remaining.next = reversed_head
        reversed_head = remaining
        remaining = next_node
    values.head = reversed_head
```

The prefix/range/suffix invariant remains useful: nodes already moved are
reachable from `reversed_head`, unprocessed nodes from `remaining`, and the two
regions do not overlap except through the link currently being changed.

```python
def remove_all(values: LinkedList, target: int) -> int:
    removed_count = 0
    previous: Node | None = None
    current = values.head
    while current is not None:
        if current.value == target:
            if previous is None:
                values.head = current.next
            else:
                previous.next = current.next
            values.size -= 1
            removed_count += 1
            current = values.head if previous is None else previous.next
        else:
            previous = current
            current = current.next
    return removed_count
```

The result and traversal invariant match the C function. Removed nodes become
eligible for reclamation only when no other Python reference reaches them; the
list cannot prove exclusive ownership.

## Traversal and clearing

```python
def to_text(values: LinkedList) -> str:
    parts: list[str] = []
    node = values.head
    while node is not None:
        parts.append(str(node.value))
        node = node.next
    return " -> ".join(parts)
```

The traversal order is direct. Returning text separates the algorithm from a
particular output stream.

```python
def clear(values: LinkedList) -> None:
    values.head = None
    values.size = 0
```

This removes the list's reachability. It is not a translation of walking every
node and calling `free`. If external aliases reach nodes, those nodes remain
alive; if no aliases remain, Python manages reclamation. Explicitly breaking
every `next` link may help prompt reference-counting implementations, but it is
not a portable ownership requirement like C destruction.

## Circular representation and Josephus

A circular list can store a tail whose `next` is the head:

```python
@dataclass
class CircularList:
    tail: Node | None = None
    size: int = 0
```

The structural invariant transfers. Python's cyclic garbage collector can
reclaim unreachable cycles, but destruction timing is not the C loop of
explicit `free` calls, and outside references can keep part of the cycle alive.

The recurrence is a direct algorithmic translation:

```python
def josephus_survivor(count: int, step: int) -> int:
    if count <= 0 or step <= 0:
        raise ValueError("count and step must be positive")
    survivor = 0
    for circle_size in range(2, count + 1):
        survivor = (survivor + step) % circle_size
    return survivor + 1
```

Python avoids fixed-width addition overflow, but the recurrence, zero-based
state, and one-based output are the same. It still computes only the survivor,
not the elimination order.

## What the Python versions must not hide

- A Python object reference is not a C pointer and has no address-of operator.
- Attribute assignment is not a general `Node**` link-location abstraction.
- Garbage collection does not prove C ownership or replace `free` on every path.
- Shared Python references mean a removed node may remain alive.
- The linked-list algorithms and invariants still transfer and provide a useful
  reference model for sequence results.

Runnable versions are collected in [`python_examples.py`](python_examples.py).
