# Week 14 Lecture Notes — Graphs and State-Space Search in Modern C++

> December 8, 2026 · Source lineage: previous path-finding, Water Jugs,
> Missionary/Torch notes and the 2025 Week 11–13 problem-solving notebooks

> Python bridge: [Python Contrast Companion for Week 14](week14_python_companion.md)

## Learning objectives

By the end of this lecture, you should be able to:

1. Model an explicit graph with an adjacency list.
2. Implement depth-first and breadth-first search with STL containers.
3. Reconstruct a shortest unweighted path and return expected absence with `optional`.
4. Model a puzzle as an implicit state graph.
5. Separate state representation, successor generation, and search policy.
6. Model proximity data as an implicit graph and classify connected components
   without constructing every edge explicitly.
7. Use multi-source BFS to compute simultaneous arrival times and detect
   required cells that no source can reach.

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | How do graph representations change traversal code and cost? | Compare explicit and geometric implicit graphs with DFS components |
| 2 | Why does BFS find shortest unweighted distances? | Trace single- and multi-source frontier layers and aggregate target times |
| 3 | How does a puzzle become an implicit graph with explainable actions? | Implement and test a Water Jugs solver |

## Hour 1 — Graph representation and depth-first exploration

> **Core implementation:** build the geometric connected-component DFS. The
> explicit adjacency-list DFS below supplies the traversal skeleton; alternative
> representations and iterative DFS are optional reading after the summary.

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

void AddUndirectedEdge(Graph& graph, int a, int b) {
  graph.at(a).push_back(b);
  graph.at(b).push_back(a);
}
```

An adjacency matrix uses O(V²) storage and gives O(1) edge queries. An adjacency
list uses O(V + E) storage and iterates neighbors efficiently. Choose based on
density and operations.

Validate external vertex numbers before converting them to vector indices.

### 3. Depth-first search

```cpp
void DfsVisit(const Graph& graph, int vertex, std::vector<bool>& visited) {
  visited.at(vertex) = true;
  for (int neighbor : graph.at(vertex)) {
    if (!visited.at(neighbor)) {
      DfsVisit(graph, neighbor, visited);
    }
  }
}

bool ReachableDfs(const Graph& graph, int start, int goal) {
  std::vector<bool> visited(graph.size(), false);
  DfsVisit(graph, start, visited);
  return visited.at(goal);
}
```

DFS explores one branch deeply before backtracking. It is useful for reachability,
connected components, cycle-related algorithms, and exhaustive backtracking.
Its recursion depth can be O(V); an explicit `std::stack` avoids call-stack limits.

### 4. Geometric proximity defines an implicit graph

Suppose monitoring stations have integer coordinates. Two stations can
communicate directly when their Euclidean distance is at most a configured
radius. Model each station as a vertex and the proximity relation as an
undirected edge.

The graph need not be stored as an adjacency list. A DFS at station `u` can
consider every unvisited station `v` and apply a neighbor predicate. This saves
an explicit O(V²) matrix, but it does **not** make the computation linear: every
visited vertex may scan all V candidates, so the straightforward traversal
performs O(V²) proximity checks and uses O(V) auxiliary state.

Connectivity is transitive, while direct proximity is not. If `A` reaches `B`
and `B` reaches `C`, all three are in one component even when `A` is farther
than the radius from `C`. Draw edges first; do not classify groups by distance
from one chosen representative.

### Squared distance and integer safety

Comparing squared distances avoids floating-point square roots:

```text
dx * dx + dy * dy <= radius * radius
```

The comparison is equivalent for a nonnegative radius, but only if the integer
arithmetic itself is valid. Subtraction and multiplication occur in the operand
type, so widening after `dx * dx` is too late. Derive the maximum possible
coordinate difference from the input contract, choose a sufficiently wide
signed type, and widen before subtracting and multiplying. Also reject a
negative radius rather than treating its square as meaningful.

Unit-test the neighbor predicate independently with identical points, exactly
on the boundary, just outside the boundary, negative coordinates, and the
largest permitted coordinates. When the published bounds do not fit safely in
64 bits, use checked arithmetic or a wider intermediate type.

### Component sweep and size classification

One DFS marks exactly the vertices in the start vertex's connected component.
An outer sweep starts a traversal only at an unvisited vertex, so each such
start discovers one new component. The traversal may return its component size
or append visited vertices to a component record.

The invariant is:

```text
before the next outer iteration, every previously encountered vertex belongs
to exactly one completed component and is marked visited
```

This supports classifications such as singleton stations versus multi-station
networks. A singleton is a component of size one, not merely a vertex whose
first few candidate checks found no neighbor.

Mark a vertex visited when the traversal discovers it, before exploring its
neighbors. Marking later permits cycles to rediscover the same vertex and can
cause repeated work or unbounded recursion. Recursive DFS may reach depth V on
a chain, so an explicit stack is the robust choice when V can be large.

### Hour 1 graph lab

First read an explicit undirected graph, reject invalid endpoints, and compute
connected components. Then receive a small point set and radius, draw the
implicit edges, and classify singleton and multi-vertex components without
materializing an adjacency matrix. Test isolated vertices, an exact-boundary
edge, a transitive chain, duplicate coordinates, and an empty graph.

## Hour 2 — Breadth-first layers and shortest paths

> **Core implementation:** implement multi-source grid BFS. Read the
> single-source `ShortestPath` as a reference for the queue/visited invariant;
> do not live-code both programs.

### 5. Breadth-first search finds shortest unweighted paths

```cpp
#include <optional>
#include <queue>

std::optional<std::vector<int>> ShortestPath(const Graph& graph, int start,
                                             int goal) {
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

As established in the Week 11 lecture notes, the optional result distinguishes “no path exists”
from every valid path, including one that starts at vertex zero or uses zero
edges because `start == goal`.

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

### Multi-source BFS: simultaneous starts

Suppose several rescue stations begin operating at time zero and each move to
one orthogonally adjacent open grid cell per minute. Running a separate BFS from
every station repeats work. Instead, initialize one FIFO queue with **all**
stations at distance zero, mark every one discovered, and then perform ordinary
BFS.

For a proof, imagine adding a super-source with one unweighted edge to every
real source. Ordinary BFS distance from that vertex, minus one, is:

```text
minimum distance from any real source to that cell
```

Seeding all real sources directly at distance zero performs that subtraction
implicitly without adding a vertex to the implementation.

The layer invariant still holds. All initial sources occupy layer zero; their
previously unseen neighbors enter layer one; then layer two, and so on. Source
insertion order may change which equally near source is considered the parent,
but not the distance.

For a grid, keep the input representation separate from search state:

- the grid says whether a cell is blocked, open, a source, or a required target;
- a distance matrix starts at an `unseen` sentinel;
- the queue stores coordinates whose shortest distance is already known;
- four direction offsets generate candidate neighbors.

For each candidate, check row bounds, column bounds, traversability, and unseen
status **before** enqueueing it. Set its distance when it is enqueued. This
ensures that every reachable cell enters the queue once and avoids using a
mutated input character as both terrain and visited state.

In C++17, a coordinate may be represented by `std::pair<int, int>` and unpacked
with a structured binding:

```cpp
auto [row, column] = frontier.front();
frontier.pop();
```

Distance usually belongs in the distance matrix rather than in every queue
entry. That makes the discovery invariant visible and leaves the grid available
for later validation or rendering. If a sparse problem genuinely needs more
metadata in each queue item, prefer a small named `struct` when positional
`tuple<int, int, int>` fields would obscure which integer means what.

### From distances to a completion result

After BFS, scan the required targets:

- if any target still has the unseen distance, the requested completion is
  impossible;
- otherwise, the time when all targets have been reached is the maximum target
  distance;
- if there are no targets, the specification must say whether completion time
  is zero or whether the input is invalid.

The maximum is correct because targets are reached simultaneously over time,
not visited one after another by a single route. Summing their distances would
answer a different question. Counting targets as they are discovered is also
valid, but a final scan often gives a simpler invariant and keeps target
classification independent of queue order.

Trace a small map with two sources, one wall-separated target, and one target
equidistant from both sources. Record the queue by layers and the distance
matrix after each layer. Then remove one wall and identify exactly which result
changes.

Multi-source BFS assumes interchangeable sources, unit-time edges, and one
arrival-time objective. Competing agents with different speeds or effects need
additional state or a different shortest-path algorithm; seeding one queue does
not automatically model those rules.

### 6. Complexity

With adjacency lists, DFS and BFS both take O(V + E) time and O(V) auxiliary
space. Path reconstruction takes O(L), where `L` is the path length.

On a rectangular grid with constant-degree moves, multi-source BFS takes
O(rows × columns) time and space because each traversable cell is discovered at
most once. Adding more initial sources does not multiply the asymptotic search
cost.

BFS minimizes number of edges only when all edges have equal cost. Weighted
nonnegative graphs require Dijkstra's algorithm; negative edges require other
methods.

### Hour 2 checkpoint

Trace the multi-source implementation by queue layer and verify the final
distance matrix against direct shortest paths on a tiny grid. Explain why the
maximum required-target distance is the completion time and why an unseen target
must produce failure.

## Hour 3 — Implicit state graphs and puzzle solving

> **Core implementation:** complete one Water Jugs solver with successor,
> goal, BFS, parent/action, and reconstruction boundaries. Other classic puzzles
> and weighted-policy comparisons are optional reading.

### 7. Puzzles are implicit graphs

For water jugs with capacities A and B, one state is the current amount in each
jug:

```cpp
struct State {
  int a;
  int b;

  friend bool operator<(const State& left, const State& right) {
    return std::tie(left.a, left.b) < std::tie(right.a, right.b);
  }
};
```

Vertices do not need to be stored in advance. A successor function generates
legal next states:

```cpp
std::vector<State> Successors(State state, int cap_a, int cap_b) {
  std::vector<State> result{
      {cap_a, state.b}, {state.a, cap_b}, {0, state.b}, {state.a, 0}};

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

For positive jug capacities and standard fill/empty/pour actions, a target is
reachable only if it lies in `[0, max(cap_a, cap_b)]` and is divisible by
`gcd(cap_a, cap_b)`. Use this as a fast rejection and a property check against
exhaustive BFS on small capacities. BFS remains necessary when the output
requires a shortest action sequence.

### 8. State invariants and duplicate control

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

### 9. Search is an engineering boundary

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

### 10. Test strategy

- start equals goal;
- goal unreachable;
- no sources, one source, and several simultaneous sources;
- every required grid cell reachable versus at least one isolated target;
- a target equally near two sources and a map with no targets;
- one-edge solution;
- graph with a cycle;
- multiple shortest paths;
- invalid vertex input;
- puzzle successors at empty, full, and partially filled boundaries;
- returned path begins at start, ends at goal, and every adjacent pair is legal.

For BFS, verify not only that a path works but that no shorter path exists on
small exhaustively enumerable cases.

### Hour 3 integration studio

Implement Water Jugs with `State`, `Successors`, `goal`, BFS, parent/action
records, and formatted output. Assert invariants for every successor. Compare
with hand solutions and exhaustive small cases.

## Final project handoff — Test logic without the window

Choose an approved project behavior with a clean state transition: an enemy
mode, cooldown, targeting rule, wave state, or pathfinding policy. Represent its
inputs and outputs without Allegro handles so normal, boundary, and impossible
states can be tested deterministically. The graphical layer should translate
events into model inputs and render resulting state; it should not contain the
only copy of the rule.

For AI-assisted algorithms, require tiny counterexamples and properties in
addition to example outputs. A path must use legal adjacent states; an FSM must
define every relevant state/event pair; a target selector must state its tie
rule. Thursday's deliverable combines automated logic tests with a repeatable
manual integration plan.

## Check yourself

1. Why does a graph traversal need visited state while a tree traversal may not?
2. Why can an implicit geometric DFS take O(V²) even though each vertex is
   marked only once?
3. Why must coordinate arithmetic be widened before subtraction and squaring?
4. Prove briefly that BFS discovers vertices in distance order.
5. Why is a parent map also a visited set in the example?
6. Why does seeding a queue with every source compute distance to the nearest
   source rather than the first source listed?
7. When is the maximum target distance the correct aggregate, and what does an
   unseen target mean?
8. List all successors of jug state `(0, 0)` for capacities `(3, 5)`.
9. Change the search objective from fewest moves to least total pouring cost.

## Summary

- Adjacency lists represent sparse explicit graphs efficiently.
- DFS explores depth; BFS gives shortest paths in unweighted graphs.
- Multi-source BFS models simultaneous unit-cost propagation and computes each
  reachable vertex's distance from its nearest source.
- A completion-time query takes the maximum required-target distance and must
  report unreachable targets explicitly.
- A neighbor predicate can define an implicit geometric graph, but its cost
  depends on how candidates are generated.
- An outer visited sweep partitions an undirected graph into connected
  components; component size supports later classification.
- Parent links turn reachability into an explainable path.
- State-space search treats puzzles as implicit graphs.
- Clean separation of model, successors, goal, and policy makes search testable.

## Optional enrichment

These comparisons support further problem solving but are not part of the three
live implementations in the core lecture.

### Representation choices and iterative DFS

| Representation | Storage | Edge query | Neighbor iteration |
|----------------|---------|------------|--------------------|
| Edge list | O(E) | O(E) | O(E) scan |
| Matrix | O(V²) | O(1) | O(V) per vertex |
| Adjacency list | O(V+E) | O(degree) typically | O(degree) |

An iterative DFS uses `std::stack<int>` instead of call frames. Mark a vertex at
discovery or deliberately tolerate duplicate stack entries, and document whether
neighbor insertion order matters to tests. For proximity graphs, compare an
O(V²) matrix, materialized lists, an on-demand predicate, and spatial buckets;
do not claim O(V+E) unless neighbor generation actually enumerates only edges.

### Search-policy comparison

- BFS: FIFO queue and unit edge cost.
- DFS: stack/recursion for reachability or exhaustive exploration.
- Dijkstra: priority queue and nonnegative weighted cost.
- A*: priority queue plus an admissible heuristic toward a goal.

Construct a weighted graph where BFS uses fewer edges but higher total cost.
Changing the worklist alone is insufficient if visited/finalization logic still
assumes BFS.

### Additional classic state spaces

| Puzzle | State | Critical invariant | Typical action |
|--------|-------|--------------------|----------------|
| Water Jugs | `(amount_a, amount_b)` | capacities respected | fill, empty, pour |
| Missionaries/Cannibals | counts on each side + boat side | missionaries safe on both sides | move one/two people |
| Bridge and Torch | side-set + torch side + elapsed cost | torch travels with movers | one/two cross |
| Maze | cell position | open and in bounds | move to neighbor |

Bridge and Torch has weighted actions, so shortest move count and minimum time
are different objectives. As a final extension, change Water Jugs from fewest
moves to least total poured volume and explain why ordinary BFS is insufficient.

## References and source materials

- [Finding paths](<https://github.com/htchen/i2p-nthu/blob/master/程式設計二/week%2013%20找路徑/week%2013%20找路徑.md>)
- [Water Jugs Problem](<https://github.com/htchen/i2p-nthu/blob/master/程式設計二/week%2014%20Water%20Jugs%20Problem/Water%20Jugs%20Problem%20（倒水問題）.md>)
- [Missionary and Torch](<https://github.com/htchen/i2p-nthu/blob/master/程式設計二/week%2016%20Missionary%20and%20Torch/week%2016%20Missionary%20and%20Torch.md>)
- [2025 Week 11 notebook (Colab)](https://colab.research.google.com/drive/1RjtHSu-82v1dQt-p2teRmKwjV9bLlQsK)
- [2025 Week 12 notebook (Colab)](https://colab.research.google.com/drive/1ptTly6n8Aplezu-3jUSrFZkeSW7UOdq4)
- [2025 Week 13 notebook (Colab)](https://colab.research.google.com/drive/1193hkL5wgE4RY4zmSHNLs4XXay0gktfM)
