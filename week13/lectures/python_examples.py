"""Runnable Python contrasts for the Week 13 polymorphism examples."""

from __future__ import annotations

import math
from abc import ABC, abstractmethod
from dataclasses import dataclass
from typing import Union


@dataclass(frozen=True)
class Point:
    x: float
    y: float


class Shape(ABC):
    @property
    @abstractmethod
    def center(self) -> Point:
        raise NotImplementedError

    @abstractmethod
    def translate(self, offset: Point) -> None:
        raise NotImplementedError

    @property
    @abstractmethod
    def area(self) -> float:
        raise NotImplementedError


class Circle(Shape):
    def __init__(self, center: Point, radius: float) -> None:
        if not math.isfinite(radius) or radius < 0.0:
            raise ValueError("radius must be finite and nonnegative")
        self._center = center
        self._radius = radius

    @property
    def center(self) -> Point:
        return self._center

    def translate(self, offset: Point) -> None:
        self._center = Point(
            self._center.x + offset.x,
            self._center.y + offset.y,
        )

    @property
    def area(self) -> float:
        return math.pi * self._radius * self._radius


class Rectangle(Shape):
    def __init__(self, corner: Point, width: float, height: float) -> None:
        if (
            not math.isfinite(width)
            or not math.isfinite(height)
            or width < 0.0
            or height < 0.0
        ):
            raise ValueError(
                "rectangle dimensions must be finite and nonnegative"
            )
        self._corner = corner
        self._width = width
        self._height = height

    @property
    def center(self) -> Point:
        return Point(
            self._corner.x + self._width / 2,
            self._corner.y + self._height / 2,
        )

    def translate(self, offset: Point) -> None:
        self._corner = Point(
            self._corner.x + offset.x,
            self._corner.y + offset.y,
        )

    @property
    def area(self) -> float:
        return self._width * self._height


class Actor(ABC):
    @abstractmethod
    def attack(self, damage: int) -> None:
        raise NotImplementedError


class BadMonster(Actor):
    def attack(self) -> None:
        pass


class GameObject(ABC):
    def update(self, seconds: float) -> None:
        if seconds < 0.0:
            raise ValueError("negative time")
        self._before_update()
        self._do_update(seconds)
        self._after_update()

    @abstractmethod
    def _do_update(self, seconds: float) -> None:
        raise NotImplementedError

    def _before_update(self) -> None:
        pass

    def _after_update(self) -> None:
        pass


class Rule(ABC):
    @abstractmethod
    def evaluate(self, input_value: float) -> float:
        raise NotImplementedError


@dataclass(frozen=True)
class Literal(Rule):
    value: float

    def evaluate(self, input_value: float) -> float:
        del input_value
        return self.value


@dataclass(frozen=True)
class Add(Rule):
    left: Rule
    right: Rule

    def evaluate(self, input_value: float) -> float:
        return self.left.evaluate(input_value) + self.right.evaluate(
            input_value
        )


@dataclass(frozen=True)
class Transform:
    position: Point


@dataclass(frozen=True)
class Sprite:
    asset_id: int


@dataclass
class Entity:
    transform: Transform
    sprite: Sprite


class Movement(ABC):
    @abstractmethod
    def next_position(self, current: Point, seconds: float) -> Point:
        raise NotImplementedError


class LinearMovement(Movement):
    def next_position(self, current: Point, seconds: float) -> Point:
        return Point(current.x + seconds, current.y)


class MovingMonster:
    def __init__(self, movement: Movement) -> None:
        if movement is None:
            raise ValueError("movement is required")
        self._movement = movement

    def next_position(self, current: Point, seconds: float) -> Point:
        return self._movement.next_position(current, seconds)


ShapeValue = Union[Circle, Rectangle]


def area_of(shape: ShapeValue) -> float:
    return shape.area


def total_area(shapes: list[Shape]) -> float:
    return sum(shape.area for shape in shapes)


def main() -> None:
    circle = Circle(Point(0.0, 0.0), 2.0)
    rectangle = Rectangle(Point(1.0, 1.0), 3.0, 4.0)
    assert math.isclose(circle.area, 4.0 * math.pi)
    assert rectangle.area == 12.0
    assert math.isclose(
        total_area([circle, rectangle]), 4.0 * math.pi + 12.0
    )
    circle.translate(Point(1.0, -1.0))
    assert circle.center == Point(1.0, -1.0)
    try:
        Circle(Point(0.0, 0.0), math.nan)
    except ValueError:
        pass
    else:
        raise AssertionError("non-finite radius was accepted")

    bad_monster = BadMonster()
    try:
        bad_monster.attack(10)
    except TypeError:
        pass
    else:
        raise AssertionError("override mismatch was not observed")

    rule = Add(Literal(2.0), Literal(3.0))
    assert rule.evaluate(100.0) == 5.0
    entity = Entity(Transform(Point(0.0, 0.0)), Sprite(7))
    assert entity.transform.position == Point(0.0, 0.0)
    moving = MovingMonster(LinearMovement())
    assert moving.next_position(Point(1.0, 2.0), 0.5) == Point(1.5, 2.0)
    assert area_of(rectangle) == 12.0
    print("Week 13 Python contrasts passed.")


if __name__ == "__main__":
    main()
