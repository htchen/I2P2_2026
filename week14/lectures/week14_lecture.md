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

## 1. Graph vocabulary

A graph consists of vertices and edges. Edges may be directed or undirected,
weighted or unweighted.

- A **path** is a sequence of vertices connected by edges.
- A **cycle** returns to an earlier vertex.
- Two vertices are **connected** if a path exists between them.
- The **distance** in an unweighted graph is the fewest edges in a path.

Trees are graphs with additional structure. General graphs may contain cycles,
so traversal needs a visited set.

## 2. Adjacency-list representation

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

## 3. Depth-first search

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

## 4. Breadth-first search finds shortest unweighted paths

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

Mark a vertex visited when it is enqueued, not when it is removed; otherwise
many frontier entries may duplicate the same vertex.

## 5. Complexity

With adjacency lists, DFS and BFS both take O(V + E) time and O(V) auxiliary
space. Path reconstruction takes O(L), where `L` is the path length.

BFS minimizes number of edges only when all edges have equal cost. Weighted
nonnegative graphs require Dijkstra's algorithm; negative edges require other
methods.

## 6. Puzzles are implicit graphs

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

## 7. State invariants and duplicate control

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

## 8. Search is an engineering boundary

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

## 9. Test strategy

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
