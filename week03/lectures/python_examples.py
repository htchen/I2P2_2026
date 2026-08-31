"""Runnable Python contrasts for the Week 3 modular-C examples."""

from __future__ import annotations

import copy
from collections.abc import Iterable, Sequence
from dataclasses import dataclass
from enum import Enum, auto
from math import gcd


@dataclass
class Student:
    student_id: int
    name: str
    grade: float


class TokenKind(Enum):
    INTEGER = auto()
    PLUS = auto()
    MINUS = auto()
    END = auto()
    INVALID = auto()


@dataclass(frozen=True)
class Token:
    kind: TokenKind
    value: int = 0


@dataclass(frozen=True)
class Rational:
    numerator: int
    denominator: int

    def __post_init__(self) -> None:
        if self.denominator == 0:
            raise ValueError("zero denominator")
        numerator = self.numerator
        denominator = self.denominator
        if denominator < 0:
            numerator = -numerator
            denominator = -denominator
        divisor = gcd(numerator, denominator)
        object.__setattr__(self, "numerator", numerator // divisor)
        object.__setattr__(self, "denominator", denominator // divisor)

    def __str__(self) -> str:
        return f"{self.numerator}/{self.denominator}"


class ValueKind(Enum):
    INTEGER = auto()
    REAL = auto()
    ERROR = auto()


@dataclass(frozen=True)
class Value:
    kind: ValueKind
    payload: int | float | str

    def __post_init__(self) -> None:
        expected = {
            ValueKind.INTEGER: int,
            ValueKind.REAL: float,
            ValueKind.ERROR: str,
        }[self.kind]
        if type(self.payload) is not expected:
            raise TypeError("payload does not match its tag")


class Counter:
    def __init__(self) -> None:
        self._value = 0

    def increment(self) -> None:
        self._value += 1

    @property
    def value(self) -> int:
        return self._value


def square(value: int) -> int:
    return value * value


def array_sum(values: Sequence[int]) -> int:
    assert values is not None
    return sum(values)


def students_read(lines: Iterable[str], capacity: int) -> list[Student]:
    students: list[Student] = []
    for line in lines:
        if not line.strip():
            continue
        if len(students) == capacity:
            raise ValueError("too many records")
        fields = line.split()
        if len(fields) != 3:
            raise ValueError("expected id, name, and grade")
        students.append(
            Student(
                student_id=int(fields[0]),
                name=fields[1],
                grade=float(fields[2]),
            )
        )
    return students


def main() -> None:
    student = Student(1001, "Ada", 92.5)
    duplicate = copy.copy(student)
    assert duplicate == student and duplicate is not student
    assert Token(TokenKind.INTEGER, 7).value == 7
    assert str(Rational(2, -4)) == "-1/2"
    assert Value(ValueKind.ERROR, "bad input").payload == "bad input"
    counter = Counter()
    counter.increment()
    assert counter.value == 1
    assert square(9) == 81
    assert array_sum([1, 2, 3]) == 6
    students = students_read(["1001 Ada 92.5\n", "1002 Lin 88\n"], 2)
    assert [entry.name for entry in students] == ["Ada", "Lin"]
    print("Week 3 Python contrasts passed.")


if __name__ == "__main__":
    main()
