#include <algorithm>
#include <cstddef>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <queue>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using Graph = std::vector<std::vector<std::size_t>>;

static std::size_t DfsSize(const Graph& graph, std::size_t start,
                           std::vector<bool>& visited) {
  if (start >= graph.size() || visited.size() != graph.size()) {
    throw std::invalid_argument{"DFS vertex or visited-array size is invalid"};
  }
  visited[start] = true;
  std::size_t size = 1;
  for (const std::size_t neighbor : graph[start]) {
    if (neighbor >= graph.size()) {
      throw std::invalid_argument{"graph contains an invalid neighbor"};
    }
    if (!visited[neighbor]) {
      size += DfsSize(graph, neighbor, visited);
    }
  }
  return size;
}

static std::optional<std::size_t> ShortestDistance(
    const std::vector<std::vector<char>>& grid,
    const std::vector<std::pair<int, int>>& starts,
    std::pair<int, int> target) {
  if (grid.empty() || grid.front().empty()) {
    throw std::invalid_argument{"grid must be nonempty"};
  }
  if (grid.size() > static_cast<std::size_t>(std::numeric_limits<int>::max()) ||
      grid.front().size() >
          static_cast<std::size_t>(std::numeric_limits<int>::max()) ||
      grid.size() > std::numeric_limits<std::size_t>::max() /
                        grid.front().size()) {
    throw std::length_error{"grid dimensions are too large"};
  }
  const int rows = static_cast<int>(grid.size());
  const int columns = static_cast<int>(grid.front().size());
  for (const auto& row : grid) {
    if (row.size() != grid.front().size()) {
      throw std::invalid_argument{"grid must be rectangular"};
    }
  }

  const auto in_bounds = [rows, columns](int row, int column) {
    return row >= 0 && row < rows && column >= 0 && column < columns;
  };
  if (!in_bounds(target.first, target.second) ||
      grid[target.first][target.second] == '#') {
    throw std::invalid_argument{"target must be an open grid cell"};
  }

  constexpr std::size_t kUnvisited =
      std::numeric_limits<std::size_t>::max();
  std::vector<std::vector<std::size_t>> distance(
      rows, std::vector<std::size_t>(columns, kUnvisited));
  std::queue<std::pair<int, int>> frontier;
  for (const auto [row, column] : starts) {
    if (!in_bounds(row, column) || grid[row][column] == '#') {
      throw std::invalid_argument{"every source must be an open grid cell"};
    }
    if (distance[row][column] == kUnvisited) {
      distance[row][column] = 0;
      frontier.push({row, column});
    }
  }

  constexpr int kDirections[4][2]{{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  while (!frontier.empty()) {
    const auto [row, column] = frontier.front();
    frontier.pop();
    if (std::pair<int, int>{row, column} == target) {
      return distance[row][column];
    }
    for (const auto& direction : kDirections) {
      const int next_row = row + direction[0];
      const int next_column = column + direction[1];
      if (in_bounds(next_row, next_column) &&
          grid[next_row][next_column] != '#' &&
          distance[next_row][next_column] == kUnvisited) {
        distance[next_row][next_column] = distance[row][column] + 1;
        frontier.push({next_row, next_column});
      }
    }
  }
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

static std::vector<Transition> Successors(State state, int cap_a, int cap_b) {
  if (cap_a < 0 || cap_b < 0 || state.a < 0 || state.a > cap_a ||
      state.b < 0 || state.b > cap_b) {
    throw std::invalid_argument{"invalid jug state or capacity"};
  }

  std::vector<Transition> result{{{cap_a, state.b}, "fill A"},
                                 {{state.a, cap_b}, "fill B"},
                                 {{0, state.b}, "empty A"},
                                 {{state.a, 0}, "empty B"}};

  const int a_to_b = std::min(state.a, cap_b - state.b);
  result.push_back(
      {{state.a - a_to_b, state.b + a_to_b}, "pour A into B"});

  const int b_to_a = std::min(state.b, cap_a - state.a);
  result.push_back(
      {{state.a + b_to_a, state.b - b_to_a}, "pour B into A"});
  return result;
}

struct SolutionStep {
  State state;
  std::string action;
};

struct ParentRecord {
  std::optional<State> parent;
  std::string action;
};

static std::optional<std::vector<SolutionStep>> ShortestJugSolution(
    int cap_a, int cap_b, int target) {
  if (cap_a <= 0 || cap_b <= 0 || target < 0 ||
      target > std::max(cap_a, cap_b)) {
    throw std::invalid_argument{"invalid jug capacity or target"};
  }

  const State start{0, 0};
  std::map<State, ParentRecord> discovered;
  std::queue<State> frontier;
  discovered.emplace(start, ParentRecord{std::nullopt, "start"});
  frontier.push(start);

  while (!frontier.empty()) {
    const State current = frontier.front();
    frontier.pop();
    if (current.a == target || current.b == target) {
      std::vector<SolutionStep> reversed;
      State cursor = current;
      while (true) {
        const ParentRecord& record = discovered.at(cursor);
        reversed.push_back({cursor, record.action});
        if (!record.parent) {
          break;
        }
        cursor = *record.parent;
      }
      std::reverse(reversed.begin(), reversed.end());
      return reversed;
    }

    for (const Transition& transition : Successors(current, cap_a, cap_b)) {
      const auto [position, inserted] = discovered.emplace(
          transition.state,
          ParentRecord{std::optional<State>{current}, transition.action});
      (void)position;
      if (inserted) {
        frontier.push(transition.state);
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
  if (distance) {
    std::cout << "distance = " << *distance << '\n';
  } else {
    std::cout << "target is unreachable\n";
  }

  const std::vector<std::vector<char>> separated{{'.', '#', '.'}};
  if (ShortestDistance(separated, {{0, 0}}, {0, 2})) {
    throw std::logic_error{"blocked grid unexpectedly had a path"};
  }

  const auto solution = ShortestJugSolution(3, 5, 4);
  if (!solution) {
    std::cout << "jug target is unreachable\n";
    return 0;
  }
  for (const SolutionStep& step : *solution) {
    std::cout << step.action << ": (" << step.state.a << ", " << step.state.b
              << ")\n";
  }
  if (ShortestJugSolution(2, 4, 3)) {
    throw std::logic_error{"unreachable jug target was accepted"};
  }
}
