#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

using Graph = std::vector<std::vector<std::size_t>>;

void Dfs(const Graph& graph, std::size_t vertex, std::vector<bool>& visited) {
  // TODO: mark before recursively exploring unvisited neighbors.
  (void)graph;
  (void)vertex;
  (void)visited;
}

std::optional<int> ShortestDistance(
    const std::vector<std::vector<char>>& grid,
    const std::vector<std::pair<int, int>>& starts,
    std::pair<int, int> target) {
  // TODO: implement multi-source BFS and explicit unreachable absence.
  (void)grid;
  (void)starts;
  (void)target;
  return std::nullopt;
}
