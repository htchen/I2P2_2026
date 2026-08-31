#include <cstddef>
#include <iostream>
#include <optional>
#include <queue>
#include <utility>
#include <vector>

using Graph = std::vector<std::vector<std::size_t>>;

static std::size_t DfsSize(const Graph& graph, std::size_t start,
                           std::vector<bool>& visited) {
  visited[start] = true;
  std::size_t size = 1;
  for (const std::size_t neighbor : graph[start]) {
    if (!visited[neighbor]) {
      size += DfsSize(graph, neighbor, visited);
    }
  }
  return size;
}

static std::optional<int> ShortestDistance(
    const std::vector<std::vector<char>>& grid,
    const std::vector<std::pair<int, int>>& starts,
    std::pair<int, int> target) {
  if (grid.empty() || grid.front().empty()) {
    return std::nullopt;
  }
  const int rows = static_cast<int>(grid.size());
  const int columns = static_cast<int>(grid.front().size());
  for (const auto& row : grid) {
    if (static_cast<int>(row.size()) != columns) {
      return std::nullopt;
    }
  }
  std::vector distance(rows, std::vector<int>(columns, -1));
  std::queue<std::pair<int, int>> frontier;
  for (const auto [row, column] : starts) {
    if (row >= 0 && row < rows && column >= 0 && column < columns &&
        grid[row][column] != '#' && distance[row][column] == -1) {
      distance[row][column] = 0;
      frontier.push({row, column});
    }
  }
  constexpr int kDirections[4][2]{{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  while (!frontier.empty()) {
    const auto [row, column] = frontier.front();
    frontier.pop();
    if (std::pair{row, column} == target) {
      return distance[row][column];
    }
    for (const auto& direction : kDirections) {
      const int next_row = row + direction[0];
      const int next_column = column + direction[1];
      if (next_row >= 0 && next_row < rows && next_column >= 0 &&
          next_column < columns && grid[next_row][next_column] != '#' &&
          distance[next_row][next_column] == -1) {
        distance[next_row][next_column] = distance[row][column] + 1;
        frontier.push({next_row, next_column});
      }
    }
  }
  return std::nullopt;
}

int main() {
  const Graph graph{{1}, {0, 2}, {1}, {}};
  std::vector<bool> visited(graph.size());
  std::cout << "component size = " << DfsSize(graph, 0, visited) << '\n';

  const std::vector<std::vector<char>> grid{{'.', '.', '#'}, {'.', '.', '.'}};
  const auto distance = ShortestDistance(grid, {{0, 0}}, {1, 2});
  std::cout << "distance = " << (distance ? *distance : -1) << '\n';
}
