# Python Contrast Companion for Week 14

Read this companion beside the [Week 14 lecture
notes](week14_lecture.md). Graph representation, DFS, BFS, shortest-path
reconstruction, multi-source search, and state-space modeling transfer directly
to Python. The most important differences are container interfaces, integer
arithmetic, recursion limits, and the strength of type and ownership guarantees.

## Relationship labels

- **Direct algorithm:** the state, invariant, and complexity transfer closely.
- **Direct idea, different container:** Python uses `list`, `dict`, `set`, and
  `collections.deque` in place of STL containers.
- **Same behavior, different numeric guarantee:** Python integers grow as
  needed, while fixed-width C++ arithmetic must be widened before overflow.
- **Closest Python model:** dataclasses, enums, and tuple unpacking preserve the
  design intent without C++ compile-time enforcement.

## Coverage map

Every fenced example in the Week 14 note is accounted for below.

| Source example | Relationship | Companion treatment |
|---|---|---|
| `Graph` and `AddUndirectedEdge` | Direct idea, different container | Use a list of neighbor lists and validate indices explicitly |
| Recursive `DfsVisit` and `ReachableDfs` | Direct algorithm | Preserve discovery-before-recursion and discuss Python's recursion limit |
| Squared-distance predicate | Same behavior, different numeric guarantee | Use the same comparison; Python integers avoid fixed-width overflow |
| Component-sweep invariant | Direct algorithm | Return component sizes while maintaining one visited set |
| `ShortestPath` with `optional` and `queue` | Direct algorithm, different result type | Return `None` or a path and use `deque` as a FIFO queue |
| Multi-source distance invariant | Direct algorithm | Seed every source at distance zero before ordinary BFS |
| Structured binding for a coordinate | Closest syntax | Unpack the tuple returned by `popleft()` |
| Ordered Water Jugs `State` | Closest model | Use a frozen, ordered dataclass as a dictionary key |
| Water Jugs `Successors` | Direct algorithm | Generate the same six transitions and test every result |
| `Action`, `Step`, and `map<State, Step>` | Closest model | Use `Enum`, a frozen dataclass, and `dict[State, Step]` |
| Jug-state bounds invariant | Direct | Validate both amounts against their capacities |
| Parent map and BFS frontier declarations | Direct idea, different container | Use a dictionary plus `deque` |

## Adjacency lists

An adjacency list has the same shape in both languages: the outer sequence is
indexed by vertex, and each inner sequence stores its neighbors.

```python
Graph = list[list[int]]


def validate_vertex(graph: Graph, vertex: int) -> None:
    if not 0 <= vertex < len(graph):
        raise IndexError("vertex outside graph")


def add_undirected_edge(graph: Graph, a: int, b: int) -> None:
    validate_vertex(graph, a)
    validate_vertex(graph, b)
    graph[a].append(b)
    try:
        graph[b].append(a)
    except MemoryError:
        graph[a].pop()
        raise
```

Python's index `-1` is valid and means the final element. That convenience is
dangerous for externally supplied vertex numbers, so the explicit range check
must reject negative values before indexing. The representation still uses
O(V + E) storage.

## Recursive DFS and reachability

```python
def dfs_visit(graph: Graph, vertex: int, visited: list[bool]) -> None:
    visited[vertex] = True
    for neighbor in graph[vertex]:
        validate_vertex(graph, neighbor)
        if not visited[neighbor]:
            dfs_visit(graph, neighbor, visited)


def reachable_dfs(graph: Graph, start: int, goal: int) -> bool:
    validate_vertex(graph, start)
    validate_vertex(graph, goal)
    visited = [False] * len(graph)
    dfs_visit(graph, start, visited)
    return visited[goal]
```

The algorithmic invariant is unchanged: a vertex becomes visited before any
recursive exploration from it. Python does not turn a deep recursive DFS into
an iterative one automatically. In fact, Python's default recursion limit is
usually much smaller than the largest graph used in programming problems. Use
an explicit list as a stack when a chain can contain many vertices; raising the
recursion limit does not remove the underlying call-stack risk.

## Squared distance and numeric behavior

The proximity predicate translates directly:

```python
def are_neighbors(
    left: tuple[int, int], right: tuple[int, int], radius: int
) -> bool:
    if radius < 0:
        raise ValueError("negative radius")
    dx = left[0] - right[0]
    dy = left[1] - right[1]
    return dx * dx + dy * dy <= radius * radius
```

Python integers expand to represent these products, subject to available
memory. C and C++ integers are fixed width, so their version must choose and
apply a sufficiently wide type before subtraction and multiplication. The
mathematical predicate is the same; the machine-arithmetic proof is not.

## Component sweep

```python
def component_sizes(graph: Graph) -> list[int]:
    visited = [False] * len(graph)
    sizes: list[int] = []

    for start in range(len(graph)):
        if visited[start]:
            continue
        visited[start] = True
        stack = [start]
        size = 0

        while stack:
            current = stack.pop()
            size += 1
            for neighbor in graph[current]:
                validate_vertex(graph, neighbor)
                if not visited[neighbor]:
                    visited[neighbor] = True
                    stack.append(neighbor)
        sizes.append(size)
    return sizes
```

Before each outer iteration, every earlier vertex belongs to exactly one
completed component and is marked visited. This version uses an explicit stack
so the traversal does not depend on Python recursion depth. A singleton is a
completed component whose returned size is one.

## BFS with parent reconstruction

`collections.deque` supplies O(1) insertion at the right and removal from the
left. Removing index zero from a normal list would shift all remaining items
and make the queue inefficient.

```python
from collections import deque
from typing import Optional


def shortest_path(
    graph: Graph, start: int, goal: int
) -> Optional[list[int]]:
    validate_vertex(graph, start)
    validate_vertex(graph, goal)
    parent: list[Optional[int]] = [None] * len(graph)
    parent[start] = start
    frontier = deque([start])

    while frontier:
        current = frontier.popleft()
        if current == goal:
            break
        for next_vertex in graph[current]:
            validate_vertex(graph, next_vertex)
            if parent[next_vertex] is None:
                parent[next_vertex] = current
                frontier.append(next_vertex)

    if parent[goal] is None:
        return None

    path = []
    vertex = goal
    while True:
        path.append(vertex)
        if vertex == start:
            break
        previous = parent[vertex]
        assert previous is not None
        vertex = previous
    path.reverse()
    return path
```

`None` plays the absence role of `std::nullopt`; `Optional[list[int]]` records
that contract for readers and type checkers. It does not enforce the contract
at runtime. Parent assignment still marks discovery before enqueueing, and the
first parent still belongs to a shortest unweighted path.

## Multi-source BFS

The central idea is language-independent: enqueue every source at distance
zero, then run one BFS.

```python
from collections import deque


def multi_source_distances(
    graph: Graph, sources: list[int]
) -> list[Optional[int]]:
    distance: list[Optional[int]] = [None] * len(graph)
    frontier = deque()

    for source in sources:
        validate_vertex(graph, source)
        if distance[source] is None:
            distance[source] = 0
            frontier.append(source)

    while frontier:
        current = frontier.popleft()
        current_distance = distance[current]
        assert current_distance is not None
        for neighbor in graph[current]:
            validate_vertex(graph, neighbor)
            if distance[neighbor] is None:
                distance[neighbor] = current_distance + 1
                frontier.append(neighbor)
    return distance
```

Every discovered distance is the minimum distance from any source. Duplicate
source entries are harmless because the discovery test inserts each vertex at
most once. With adjacency lists, the total work remains O(V + E), rather than
O(number of sources × (V + E)).

The structured binding used for a C++ grid coordinate has a direct readability
counterpart:

```python
row, column = frontier.popleft()
```

Both forms unpack a two-field value. Python performs this at runtime and does
not require a declared `pair<int, int>` element type.

## Water Jugs state and ordering

```python
from dataclasses import dataclass


@dataclass(frozen=True, order=True)
class State:
    a: int
    b: int
```

`frozen=True` prevents ordinary field reassignment and makes the generated
state hashable, so it can be a dictionary or set key. `order=True` gives
lexicographic field ordering similar to the C++ `tie` comparison. BFS itself
only needs equality and hashing; total ordering is included to match the
ordered-map source example.

## Successor generation and invariants

```python
def successors(state: State, cap_a: int, cap_b: int) -> list[State]:
    if (
        cap_a < 0
        or cap_b < 0
        or not 0 <= state.a <= cap_a
        or not 0 <= state.b <= cap_b
    ):
        raise ValueError("invalid jug state or capacity")
    result = [
        State(cap_a, state.b),
        State(state.a, cap_b),
        State(0, state.b),
        State(state.a, 0),
    ]

    a_to_b = min(state.a, cap_b - state.b)
    result.append(State(state.a - a_to_b, state.b + a_to_b))

    b_to_a = min(state.b, cap_a - state.a)
    result.append(State(state.a + b_to_a, state.b - b_to_a))
    return result
```

This intentionally produces the same six candidate actions, including
duplicates and no-op states. The visited structure removes duplicate search
work. Keeping one candidate per named action is useful when the reconstructed
answer must explain exactly which action was considered.

```python
def is_valid_state(state: State, cap_a: int, cap_b: int) -> bool:
    return 0 <= state.a <= cap_a and 0 <= state.b <= cap_b
```

Test the successor function separately and assert this predicate for every
result. Validation belongs at the model boundary; a search should not quietly
explore illegal negative or over-capacity states.

## Actions, steps, and the BFS boundary

```python
from dataclasses import dataclass
from enum import Enum, auto


class Action(Enum):
    FILL_A = auto()
    FILL_B = auto()
    EMPTY_A = auto()
    EMPTY_B = auto()
    POUR_A_TO_B = auto()
    POUR_B_TO_A = auto()


@dataclass(frozen=True)
class Step:
    parent: State
    action: Action


discovered: dict[State, Step] = {}
```

The enum and record preserve the explanation boundary: a result can name an
action rather than only list amounts. Python's dictionary combines the C++
map's key-to-step relationship with average O(1) lookup, but iteration follows
insertion order rather than sorted state order.

The minimal parent/frontier declarations are correspondingly:

```python
from collections import deque


parent: dict[State, State] = {}
frontier = deque[State]()
```

As in C++, the parent or discovered dictionary can also be the visited set.
Insert a state when enqueueing it. If it is inserted only when removed, cycles
can add the same state to the frontier many times.

## What should transfer back to C++

The course goal is not to memorize spelling differences. Carry these ideas
back to the Week 14 C++ implementation:

1. State the discovery and layer invariants before choosing containers.
2. Separate graph/state representation, successor generation, goal testing,
   search policy, and reconstruction.
3. Mark a vertex or state when it enters the frontier.
4. Use parent information both to suppress duplicates and explain a result.
5. Test tiny cyclic, unreachable, boundary, and multiple-shortest-path cases.
6. Revisit arithmetic and recursion assumptions when changing languages.

Runnable versions are collected in
[`python_examples.py`](python_examples.py). Run them with:

```sh
python3 week14/lectures/python_examples.py
```
