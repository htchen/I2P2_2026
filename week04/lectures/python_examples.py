"""Runnable Python contrasts for the Week 4 pointer examples."""

from __future__ import annotations

from collections.abc import Iterable, Sequence
from dataclasses import dataclass


@dataclass
class Box:
    value: int


@dataclass
class Node:
    value: int
    next: Node | None = None


@dataclass(frozen=True)
class Student:
    student_id: int
    grade: float


def swapped(left: int, right: int) -> tuple[int, int]:
    return right, left


def swap_boxes(left: Box, right: Box) -> None:
    left.value, right.value = right.value, left.value


def contains_zero(values: Sequence[int]) -> bool:
    return any(value == 0 for value in values)


def read_values(tokens: Iterable[str], count: int) -> list[int]:
    if count < 0:
        raise ValueError("negative count")
    iterator = iter(tokens)
    values: list[int] = []
    for _ in range(count):
        try:
            token = next(iterator)
        except StopIteration as error:
            raise ValueError("not enough values") from error
        values.append(int(token))
    return values


class IntBuffer:
    def __init__(self) -> None:
        self._data: list[int] = []

    def push(self, value: int) -> None:
        self._data.append(value)

    def values(self) -> tuple[int, ...]:
        return tuple(self._data)


def values_clone(source: Sequence[int]) -> list[int]:
    return list(source)


def values_destroy(owned: list[int]) -> None:
    owned.clear()


def rank_students(students: Sequence[Student]) -> list[Student]:
    return sorted(students, key=lambda student: student.grade, reverse=True)


def main() -> None:
    left = Box(10)
    right = Box(20)
    swap_boxes(left, right)
    assert (left.value, right.value) == (20, 10)
    assert swapped(10, 20) == (20, 10)
    assert contains_zero([3, 0, 4])
    assert read_values(["10", "20", "30"], 3) == [10, 20, 30]
    try:
        read_values([], -1)
    except ValueError:
        pass
    else:
        raise AssertionError("negative count was accepted")

    buffer = IntBuffer()
    buffer.push(7)
    buffer.push(9)
    assert buffer.values() == (7, 9)

    original = [1, 2, 3]
    clone = values_clone(original)
    values_destroy(clone)
    assert original == [1, 2, 3]
    assert clone == []

    ranking = rank_students(
        [Student(1, 82.0), Student(2, 95.0), Student(3, 88.5)]
    )
    assert [student.student_id for student in ranking] == [2, 3, 1]
    assert Node(1).next is None
    print("Week 4 Python contrasts passed.")


if __name__ == "__main__":
    main()
