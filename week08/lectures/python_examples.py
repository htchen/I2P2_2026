"""Runnable Python contrasts for the Week 8 C++ examples."""

from collections.abc import Sequence
from dataclasses import dataclass
from pathlib import Path
from tempfile import TemporaryDirectory


@dataclass
class IntBox:
    value: int


def ineffective_swap(left: int, right: int) -> None:
    left, right = right, left


def swapped(left: int, right: int) -> tuple[int, int]:
    return right, left


def swap_boxes(left: IntBox, right: IntBox) -> None:
    left.value, right.value = right.value, left.value


def set_to_seventeen(value: IntBox) -> int:
    value.value = 17
    return 15


def mean(values: Sequence[int]) -> float:
    if not values:
        return 0.0
    return sum(values) / len(values)


def require_nonnegative(value: int) -> None:
    if value < 0:
        raise ValueError("negative value")


def write_report(path: str) -> None:
    with open(path, "w", encoding="utf-8") as output:
        output.write("complete\n")


def even_values(values: Sequence[int]) -> list[int]:
    return [value for value in values if value % 2 == 0]


def read_words(path: str) -> list[str]:
    with open(path, encoding="utf-8") as input_file:
        return input_file.read().split()


def main() -> None:
    x = 5
    y = 7
    ineffective_swap(x, y)
    assert (x, y) == (5, 7)
    x, y = swapped(x, y)
    assert (x, y) == (7, 5)

    left = IntBox(5)
    right = IntBox(7)
    swap_boxes(left, right)
    assert (left.value, right.value) == (7, 5)
    assert set_to_seventeen(left) == 15
    assert left.value == 17

    assert mean([1, 2, 3]) == 2.0
    assert even_values([1, 2, 3, 4]) == [2, 4]

    try:
        require_nonnegative(-1)
    except ValueError as error:
        assert str(error) == "negative value"
    else:
        raise AssertionError("negative value was accepted")

    with TemporaryDirectory() as directory:
        path = Path(directory) / "report.txt"
        write_report(str(path))
        assert read_words(str(path)) == ["complete"]

    print("Week 8 Python contrasts passed.")


if __name__ == "__main__":
    main()
