"""Runnable Python contrasts for the Week 2 C examples."""

from bisect import bisect_left, bisect_right
from collections.abc import Callable, Sequence
from typing import Optional


def mean(values: Sequence[int]) -> float:
    return 0.0 if not values else sum(values) / len(values)


def ineffective_swap(a: int, b: int) -> None:
    a, b = b, a


def absolute_value(value: int) -> int:
    return -value if value < 0 else value


def swapped(left: int, right: int) -> tuple[int, int]:
    return right, left


def make_sequence() -> Callable[[], int]:
    value = 0

    def next_sequence() -> int:
        nonlocal value
        value += 1
        return value

    return next_sequence


def maximum(values: Sequence[int]) -> Optional[int]:
    if not values:
        return None
    result = values[0]
    for value in values[1:]:
        if value > result:
            result = value
    return result


def build_prefix(values: Sequence[int]) -> list[int]:
    prefix = [0]
    for value in values:
        prefix.append(prefix[-1] + value)
    return prefix


def query_total(prefix: Sequence[int], left: int, right: int) -> int:
    if not 0 <= left <= right < len(prefix):
        raise IndexError("invalid half-open range")
    return prefix[right] - prefix[left]


def count_sorted(values: Sequence[int], target: int) -> int:
    return bisect_right(values, target) - bisect_left(values, target)


def require_name_capacity(name: str, capacity: int = 32) -> str:
    if len(name) >= capacity:
        raise ValueError("name does not fit the C buffer")
    return name


def string_length(text: str) -> int:
    length = 0
    for _ in text:
        length += 1
    return length


def minimum(values: Sequence[int]) -> int:
    if not values:
        raise ValueError("minimum requires a nonempty sequence")
    result = values[0]
    for value in values[1:]:
        if value < result:
            result = value
    return result


def sum_matrix(matrix: Sequence[Sequence[int]]) -> int:
    if not matrix:
        return 0
    columns = len(matrix[0])
    if any(len(row) != columns for row in matrix):
        raise ValueError("matrix must be rectangular")
    return sum(value for row in matrix for value in row)


def insertion_sort(values: list[int]) -> None:
    for index in range(1, len(values)):
        current = values[index]
        position = index
        while position > 0 and values[position - 1] > current:
            values[position] = values[position - 1]
            position -= 1
        values[position] = current


def main() -> None:
    assert mean([2, 4, 6]) == 4.0
    assert absolute_value(-7) == 7
    assert swapped(1, 2) == (2, 1)
    sequence = make_sequence()
    assert [sequence(), sequence(), sequence()] == [1, 2, 3]
    assert maximum([]) is None
    assert maximum([3, 8, 2]) == 8
    prefix = build_prefix([3, -1, 4, 2])
    assert prefix == [0, 3, 2, 6, 8]
    assert query_total(prefix, 1, 3) == 3
    assert count_sorted([1, 2, 2, 2, 5], 2) == 3
    assert require_name_capacity("Ada") == "Ada"
    assert string_length("C17") == 3
    assert minimum([3, -1, 4]) == -1
    assert sum_matrix([[1, 2], [3, 4]]) == 10
    values = [5, 2, 4, 1]
    insertion_sort(values)
    assert values == [1, 2, 4, 5]
    print("Week 2 Python contrasts passed.")


if __name__ == "__main__":
    main()
