#include <cstddef>
#include <optional>
#include <string>
#include <utility>
#include <vector>

using Graph = std::vector<std::vector<std::size_t>>;

void Dfs(const Graph& graph, std::size_t vertex, std::vector<bool>& visited) {
  // TODO: mark before recursively exploring unvisited neighbors.
  (void)graph;
  (void)vertex;
  (void)visited;
}

std::optional<std::size_t> ShortestDistance(
    const std::vector<std::vector<char>>& grid,
    const std::vector<std::pair<int, int>>& starts,
    std::pair<int, int> target) {
  // TODO: throw for malformed coordinates/grids; return absence only when a
  // valid target is unreachable from all valid starting cells.
  (void)grid;
  (void)starts;
  (void)target;
  return std::nullopt;
}

struct State {
  int a;
  int b;

  bool operator<(const State& other) const {
    if (a != other.a) {
      return a < other.a;
    }
    return b < other.b;
  }
};

struct Transition {
  State state;
  std::string action;
};

std::vector<Transition> Successors(State state, int cap_a, int cap_b) {
  // TODO: validate the state and generate all fill, empty, and pour actions.
  (void)state;
  (void)cap_a;
  (void)cap_b;
  return {};
}

struct SolutionStep {
  State state;
  std::string action;
};

std::optional<std::vector<SolutionStep>> ShortestJugSolution(
    int cap_a, int cap_b, int target) {
  // TODO: run BFS from {0, 0}, record parent plus action at discovery time, and
  // reconstruct a shortest explanation when either jug contains target.
  (void)cap_a;
  (void)cap_b;
  (void)target;
  return std::nullopt;
}
