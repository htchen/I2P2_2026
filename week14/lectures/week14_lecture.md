# Lecture 13 — Graphs and State-Space Search in Modern C++

> December 8, 2026 · Source lineage: the legacy path-finding, Water Jugs,
> Missionary/Torch notes and the 2025 Week 11–13 problem-solving notebooks

## Learning objectives

By the end of this lecture, you should be able to:

1. Model an explicit graph with an adjacency list.
2. Implement depth-first and breadth-first search with STL containers.
3. Reconstruct a shortest unweighted path from parent links.
4. Model a puzzle as an implicit state graph.
5. Separate state representation, successor generation, and search policy.

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | How do graph representations change traversal code and cost? | Build an adjacency list and implement iterative/recursive DFS |
| 2 | Why does BFS find a shortest unweighted path? | Trace frontier layers and reconstruct paths |
| 3 | How does a puzzle become an implicit graph with explainable actions? | Implement and test a Water Jugs solver |

## Hour 1 — Graph representation and depth-first exploration

### 1. Graph vocabulary

A graph consists of vertices and edges. Edges may be directed or undirected,
weighted or unweighted.

- A **path** is a sequence of vertices connected by edges.
- A **cycle** returns to an earlier vertex.
- Two vertices are **connected** if a path exists between them.
- The **distance** in an unweighted graph is the fewest edges in a path.

Trees are graphs with additional structure. General graphs may contain cycles,
so traversal needs a visited set.

### 2. Adjacency-list representation

For integer vertices `0 .. n-1`:

```cpp
using Graph = std::vector<std::vector<int>>;

void add_undirected_edge(Graph& graph, int a, int b)
{
    graph.at(a).push_back(b);
    graph.at(b).push_back(a);
}
```

An adjacency matrix uses O(V²) storage and gives O(1) edge queries. An adjacency
list uses O(V + E) storage and iterates neighbors efficiently. Choose based on
density and operations.

Validate external vertex numbers before converting them to vector indices.

### Edge list, matrix, or adjacency list

| Representation | Storage | Edge query | Neighbor iteration |
|----------------|---------|------------|--------------------|
| Edge list | O(E) | O(E) | O(E) scan |
| Matrix | O(V²) | O(1) | O(V) per vertex |
| Adjacency list | O(V+E) | O(degree) typically | O(degree) |

An edge list is convenient when sorting edges, a matrix suits dense graphs, and
adjacency lists suit sparse traversal. Representation follows operations.

### 3. Depth-first search

```cpp
void dfs_visit(const Graph& graph, int vertex, std::vector<bool>& visited)
{
    visited.at(vertex) = true;
    for (int neighbor : graph.at(vertex)) {
        if (!visited.at(neighbor)) {
            dfs_visit(graph, neighbor, visited);
        }
    }
}

bool reachable_dfs(const Graph& graph, int start, int goal)
{
    std::vector<bool> visited(graph.size(), false);
    dfs_visit(graph, start, visited);
    return visited.at(goal);
}
```

DFS explores one branch deeply before backtracking. It is useful for reachability,
connected components, cycle-related algorithms, and exhaustive backtracking.
Its recursion depth can be O(V); an explicit `std::stack` avoids call-stack limits.

### Iterative DFS and discovery order

```cpp
std::vector<int> dfs_order(const Graph& graph, int start)
{
    std::vector<int> order;
    std::vector<bool> visited(graph.size(), false);
    std::stack<int> work;
    work.push(start);

    while (!work.empty()) {
        int current = work.top();
        work.pop();
        if (visited.at(current)) continue;
        visited.at(current) = true;
        order.push_back(current);

        for (auto it = graph.at(current).rbegin();
             it != graph.at(current).rend(); ++it) {
            if (!visited.at(*it)) work.push(*it);
        }
    }
    return order;
}
```

Reverse neighbor insertion matches a left-to-right recursive traversal for a
fixed adjacency order. DFS order is not inherently unique; tests should control
neighbor order or assert reachability rather than one accidental sequence.

### Hour 1 graph lab

Read an undirected graph, reject invalid endpoints, and compute connected
components. Test isolated vertices, parallel edges, self-loops, a cycle, and an
empty graph. Decide whether parallel edges are preserved or normalized away.

## Hour 2 — Breadth-first layers and shortest paths

### 4. Breadth-first search finds shortest unweighted paths

```cpp
#include <optional>
#include <queue>

std::optional<std::vector<int>> shortest_path(
    const Graph& graph, int start, int goal)
{
    const int unseen = -1;
    std::vector<int> parent(graph.size(), unseen);
    std::queue<int> frontier;

    parent.at(start) = start;
    frontier.push(start);

    while (!frontier.empty()) {
        int current = frontier.front();
        frontier.pop();

        if (current == goal) break;

        for (int next : graph.at(current)) {
            if (parent.at(next) == unseen) {
                parent.at(next) = current; /* mark before enqueue */
                frontier.push(next);
            }
        }
    }

    if (parent.at(goal) == unseen) return std::nullopt;

    std::vector<int> path;
    for (int vertex = goal;; vertex = parent.at(vertex)) {
        path.push_back(vertex);
        if (vertex == start) break;
    }
    std::reverse(path.begin(), path.end());
    return path;
}
```

BFS processes vertices in nondecreasing distance from `start`. The first time a
vertex is discovered, its parent therefore defines a shortest path.

### Layer invariant

When BFS begins processing distance `d`, every queued vertex has distance `d` or
`d+1`, and all vertices at smaller distance have been processed. Each newly
discovered neighbor has distance exactly one more than its parent. This is the
reason the first recorded parent yields a shortest edge-count path.

Trace queue, parent, and distance after every pop on a graph with two shortest
paths. The selected path depends on adjacency order, but its length does not.

Mark a vertex visited when it is enqueued, not when it is removed; otherwise
many frontier entries may duplicate the same vertex.

### 5. Complexity

With adjacency lists, DFS and BFS both take O(V + E) time and O(V) auxiliary
space. Path reconstruction takes O(L), where `L` is the path length.

BFS minimizes number of edges only when all edges have equal cost. Weighted
nonnegative graphs require Dijkstra's algorithm; negative edges require other
methods.

### Search-policy boundary

- BFS: FIFO queue and unit edge cost.
- DFS: stack/recursion for reachability or exhaustive exploration.
- Dijkstra: priority queue and nonnegative weighted cost.
- A*: priority queue plus an admissible heuristic toward a goal.

Keep successor generation independent so the same model can use another policy.
Replacing a queue with a priority queue is not enough if visited/finalization
logic still assumes BFS.

### Hour 2 checkpoint

Modify `shortest_path` to return path and distance. Then add weighted edges and
construct a case where BFS uses fewer edges but higher total cost. State the
additional data and invariant Dijkstra needs.

## Hour 3 — Implicit state graphs and puzzle solving

### 6. Puzzles are implicit graphs

For water jugs with capacities A and B, one state is the current amount in each
jug:

```cpp
struct State {
    int a;
    int b;

    friend bool operator<(const State& left, const State& right)
    {
        return std::tie(left.a, left.b) < std::tie(right.a, right.b);
    }
};
```

Vertices do not need to be stored in advance. A successor function generates
legal next states:

```cpp
std::vector<State> successors(State state, int cap_a, int cap_b)
{
    std::vector<State> result{
        {cap_a, state.b}, {state.a, cap_b},
        {0, state.b},     {state.a, 0}
    };

    int a_to_b = std::min(state.a, cap_b - state.b);
    result.push_back({state.a - a_to_b, state.b + a_to_b});

    int b_to_a = std::min(state.b, cap_a - state.a);
    result.push_back({state.a + b_to_a, state.b - b_to_a});
    return result;
}
```

The full solver combines:

- **state representation:** equality/order/hash and invariant;
- **successor generation:** every legal transition, no illegal transition;
- **goal test:** whether the desired amount is present;
- **search policy:** BFS for fewest moves, DFS for reachability;
- **parent/action record:** explanation of the resulting solution.

This separation also applies to missionaries-and-cannibals, bridge-and-torch,
maze routing, and game AI.

### Record actions, not only parents

```cpp
enum class Action { FillA, FillB, EmptyA, EmptyB, PourAToB, PourBToA };

struct Step {
    State parent;
    Action action;
};

std::map<State, Step> discovered;
```

Path reconstruction can now explain “fill the 5-liter jug” rather than printing
only coordinate pairs. Explainable paths help both demos and successor debugging.

### Water Jugs feasibility

With standard fill/empty/pour actions, a target is reachable only if it does not
exceed the larger capacity and is divisible by `gcd(cap_a, cap_b)`. Use this as a
fast rejection and a property check against exhaustive BFS on small capacities.
BFS remains necessary when the output requires a shortest action sequence.

### 7. State invariants and duplicate control

For the jug state:

```text
0 <= a <= cap_a
0 <= b <= cap_b
```

Test successor generation separately and assert the invariant for every result.
Several actions may produce the same state, especially when a jug is already
empty or full. The visited map prevents infinite cycles and unnecessary work.

```cpp
std::map<State, State> parent;
std::queue<State> frontier;
```

For larger state spaces, `unordered_map` can improve average lookup if `State`
has a correct equality operation and hash function.

### 8. Search is an engineering boundary

Keep I/O and visualization outside the solver. A solver that receives a model
and returns `optional<vector<State>>` can be unit-tested without a terminal or
game window. The final project should isolate non-graphical game logic in the
same way.

AI-generated search code often looks plausible while it:

- marks visited too late;
- omits one legal successor;
- admits an illegal state;
- reconstructs parents in the wrong direction;
- assumes BFS handles weighted costs;
- captures a reference that later dangles.

Use tiny hand-drawn state graphs and properties to audit generated code.

### 9. Test strategy

- start equals goal;
- goal unreachable;
- one-edge solution;
- graph with a cycle;
- multiple shortest paths;
- invalid vertex input;
- puzzle successors at empty, full, and partially filled boundaries;
- returned path begins at start, ends at goal, and every adjacent pair is legal.

For BFS, verify not only that a path works but that no shorter path exists on
small exhaustively enumerable cases.

### Compare classic state spaces

| Puzzle | State | Critical invariant | Typical action |
|--------|-------|--------------------|----------------|
| Water Jugs | `(amount_a, amount_b)` | capacities respected | fill, empty, pour |
| Missionaries/Cannibals | counts on each side + boat side | missionaries safe on both sides | move one/two people |
| Bridge and Torch | side-set + torch side + elapsed cost | torch travels with movers | one/two cross |
| Maze | cell position | open and in bounds | move to neighbor |

Bridge and Torch has weighted actions, so shortest move count and minimum time
are different objectives. This directly motivates separating model from policy.

### Hour 3 integration studio

Implement Water Jugs with `State`, `successors`, `goal`, BFS, parent/action
records, and formatted output. Assert invariants for every successor. Compare
with hand solutions and exhaustive small cases; then change the objective to
total poured volume and explain why ordinary BFS is no longer correct.

## Check yourself

1. Why does a graph traversal need visited state while a tree traversal may not?
2. Prove briefly that BFS discovers vertices in distance order.
3. Why is a parent map also a visited set in the example?
4. List all successors of jug state `(0, 0)` for capacities `(3, 5)`.
5. Change the search objective from fewest moves to least total pouring cost.

## Summary

- Adjacency lists represent sparse explicit graphs efficiently.
- DFS explores depth; BFS gives shortest paths in unweighted graphs.
- Parent links turn reachability into an explainable path.
- State-space search treats puzzles as implicit graphs.
- Clean separation of model, successors, goal, and policy makes search testable.
