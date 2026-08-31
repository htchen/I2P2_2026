"""Runnable Python contrasts for the Week 11 template and STL examples."""

from bisect import bisect_left, bisect_right
from collections import Counter
from collections.abc import Sequence
from dataclasses import dataclass
from typing import Generic, Optional, TypeVar


T = TypeVar("T")


def maximum(left: T, right: T) -> T:
    return right if left < right else left


@dataclass(frozen=True)
class Box(Generic[T]):
    value: T


def index_of(values: Sequence[T], target: T) -> Optional[int]:
    for index, value in enumerate(values):
        if value == target:
            return index
    return None


def equal_range(values: Sequence[int], target: int) -> tuple[int, int]:
    return bisect_left(values, target), bisect_right(values, target)


def frequency_ranking(words: Sequence[str]) -> list[tuple[str, int]]:
    frequency = Counter(words)
    return sorted(frequency.items(), key=lambda item: (-item[1], item[0]))


def main() -> None:
    assert maximum(3, 8) == 8
    assert maximum("ant", "bee") == "bee"
    assert Box(7).value == 7

    values = [-10, 3, -2, 8]
    assert sorted(values, key=abs) == [-2, 3, 8, -10]
    threshold = 80
    passed = lambda score, threshold=threshold: score >= threshold
    threshold = 100
    assert passed(85)

    assert index_of([5, 8, 13], 5) == 0
    assert index_of([5, 8, 13], 7) is None
    assert equal_range([1, 2, 2, 2, 5], 2) == (1, 4)
    assert frequency_ranking(["bee", "ant", "bee", "cat", "ant"]) == [
        ("ant", 2),
        ("bee", 2),
        ("cat", 1),
    ]
    print("Week 11 Python contrasts passed.")


if __name__ == "__main__":
    main()
