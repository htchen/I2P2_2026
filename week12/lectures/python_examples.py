"""Runnable Python contrasts for the Week 12 C++ ownership examples."""

from __future__ import annotations

import copy
from dataclasses import dataclass, field


class ShallowIntVec:
    def __init__(self, size: int) -> None:
        self.values = [0] * size


class IntVec:
    def __init__(self, size: int = 0) -> None:
        if size < 0:
            raise ValueError("negative size")
        self._values = [0] * size

    def __copy__(self) -> IntVec:
        result = IntVec()
        result._values = self._values.copy()
        return result

    def assign_copy(self, other: IntVec) -> None:
        replacement = other._values.copy()
        self._values = replacement

    @classmethod
    def move_from(cls, other: IntVec) -> IntVec:
        result = cls()
        result._values = other._values
        other._values = []
        return result

    def set(self, index: int, value: int) -> None:
        self._values[index] = value

    def values(self) -> tuple[int, ...]:
        return tuple(self._values)


@dataclass
class ScoreBuffer:
    values: list[int] = field(default_factory=list)


@dataclass
class Node:
    value: int = 0
    left: Node | None = None
    right: Node | None = None


class Connection:
    def __copy__(self) -> Connection:
        raise TypeError("Connection cannot be copied")

    def __deepcopy__(self, memo: dict[int, object]) -> Connection:
        del memo
        raise TypeError("Connection cannot be copied")


def main() -> None:
    shallow_original = ShallowIntVec(3)
    shallow_copy = copy.copy(shallow_original)
    shallow_copy.values[0] = 99
    assert shallow_original.values[0] == 99

    original = IntVec(3)
    original.set(0, 7)
    duplicate = copy.copy(original)
    duplicate.set(0, 11)
    assert original.values() == (7, 0, 0)
    assert duplicate.values() == (11, 0, 0)

    destination = IntVec(1)
    destination.assign_copy(original)
    assert destination.values() == original.values()

    moved = IntVec.move_from(original)
    assert moved.values() == (7, 0, 0)
    assert original.values() == ()

    first_buffer = ScoreBuffer()
    second_buffer = ScoreBuffer()
    first_buffer.values.append(5)
    assert second_buffer.values == []

    root = Node(value=10, left=Node(value=5))
    assert root.left is not None
    assert root.left.value == 5

    connection = Connection()
    try:
        copy.copy(connection)
    except TypeError as error:
        assert str(error) == "Connection cannot be copied"
    else:
        raise AssertionError("Connection was copied")

    print("Week 12 Python contrasts passed.")


if __name__ == "__main__":
    main()
