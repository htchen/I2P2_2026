"""Runnable Python contrasts for the Week 9 C++ class examples."""

from __future__ import annotations

import math
from math import gcd


class Rational:
    def __init__(self, numerator: int = 0, denominator: int = 1) -> None:
        if denominator == 0:
            raise ValueError("zero denominator")
        if denominator < 0:
            numerator = -numerator
            denominator = -denominator
        divisor = gcd(numerator, denominator)
        self._numerator = numerator // divisor
        self._denominator = denominator // divisor

    @property
    def numerator(self) -> int:
        return self._numerator

    @property
    def denominator(self) -> int:
        return self._denominator

    def __iadd__(self, other: Rational) -> Rational:
        replacement = self + other
        self._numerator = replacement.numerator
        self._denominator = replacement.denominator
        return self

    def __add__(self, other: Rational) -> Rational:
        if not isinstance(other, Rational):
            return NotImplemented
        return Rational(
            self.numerator * other.denominator
            + other.numerator * self.denominator,
            self.denominator * other.denominator,
        )

    def __str__(self) -> str:
        return f"{self.numerator}/{self.denominator}"

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, Rational):
            return NotImplemented
        return (
            self.numerator == other.numerator
            and self.denominator == other.denominator
        )

    @classmethod
    def from_whole(cls, whole: int) -> Rational:
        return cls(whole, 1)


class Interval:
    def __init__(self, lower: int, upper: int) -> None:
        self._lower = lower
        self._upper = upper
        if self._lower > self._upper:
            raise ValueError("reversed interval")


class SampleWindow:
    def __init__(self, start: float, finish: float) -> None:
        if not math.isfinite(start) or not math.isfinite(finish):
            raise ValueError("non-finite endpoint")
        if start > finish:
            raise ValueError("reversed sampling window")
        self._start = start
        self._finish = finish

    @classmethod
    def from_endpoints(cls, start: float, finish: float) -> SampleWindow:
        return cls(start, finish)


def make_half() -> Rational:
    local = Rational(1, 2)
    return local


def main() -> None:
    value = Rational(2, -4)
    assert value.numerator == -1
    assert value.denominator == 2
    assert value == Rational(-1, 2)
    assert str(value) == "-1/2"
    value += Rational(3, 2)
    assert value == Rational(1, 1)
    assert Rational.from_whole(3) == Rational(3, 1)
    Interval(1, 4)
    SampleWindow.from_endpoints(0.0, 1.0)
    assert make_half() == Rational(1, 2)
    print("Week 9 Python contrasts passed.")


if __name__ == "__main__":
    main()
