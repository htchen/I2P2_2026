"""Runnable Python contrasts for the Week 14 graph-search examples."""

from __future__ import annotations

from collections import deque
from dataclasses import dataclass
from enum import Enum, auto
from typing import Optional

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


def multi_source_distances(
    graph: Graph, sources: list[int]
) -> list[Optional[int]]:
    distance: list[Optional[int]] = [None] * len(graph)
    frontier: deque[int] = deque()
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


def are_neighbors(
    left: tuple[int, int], right: tuple[int, int], radius: int
) -> bool:
    if radius < 0:
        raise ValueError("negative radius")
    dx = left[0] - right[0]
    dy = left[1] - right[1]
    return dx * dx + dy * dy <= radius * radius


@dataclass(frozen=True, order=True)
class State:
    a: int
    b: int


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


def is_valid_state(state: State, cap_a: int, cap_b: int) -> bool:
    return 0 <= state.a <= cap_a and 0 <= state.b <= cap_b


def main() -> None:
    graph: Graph = [[] for _ in range(6)]
    add_undirected_edge(graph, 0, 1)
    add_undirected_edge(graph, 1, 2)
    add_undirected_edge(graph, 3, 4)
    assert reachable_dfs(graph, 0, 2)
    assert not reachable_dfs(graph, 0, 5)
    assert component_sizes(graph) == [3, 2, 1]
    assert shortest_path(graph, 0, 2) == [0, 1, 2]
    assert shortest_path(graph, 0, 5) is None
    assert multi_source_distances(graph, [0, 4]) == [0, 1, 2, 1, 0, None]
    try:
        reachable_dfs(graph, -1, 2)
    except IndexError:
        pass
    else:
        raise AssertionError("negative vertex was accepted")

    assert are_neighbors((0, 0), (3, 4), 5)
    assert not are_neighbors((0, 0), (3, 4), 4)

    initial = State(0, 0)
    next_states = successors(initial, 3, 5)
    assert len(next_states) == 6
    assert all(is_valid_state(state, 3, 5) for state in next_states)
    discovered = {
        State(3, 0): Step(initial, Action.FILL_A),
        State(0, 5): Step(initial, Action.FILL_B),
    }
    assert discovered[State(3, 0)].action is Action.FILL_A
    try:
        successors(State(4, 0), 3, 5)
    except ValueError:
        pass
    else:
        raise AssertionError("invalid jug state was accepted")
    print("Week 14 Python contrasts passed.")


if __name__ == "__main__":
    main()
