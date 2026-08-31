# Python Contrast Companion for Week 13

Read this companion beside the [Week 13 lecture
notes](week13_lecture.md). Dynamic dispatch, abstract interfaces, template-method
control flow, composition, strategy, and composite structure all have useful
Python counterparts. C++ `override` checking, slicing, virtual destruction,
`unique_ptr`, `dynamic_cast`, and closed `variant` visitation do not.

## Relationship labels

- **Direct design:** the abstraction or dispatch pattern transfers closely.
- **Same behavior, weaker static guarantee:** Python dispatches dynamically but
  does not compile-check the same signatures or ownership.
- **Closest Python model:** `ABC`, `isinstance`, or a union annotation preserves
  one intent with different enforcement.
- **No faithful equivalent:** C++ object slicing or virtual destruction is absent.

## Coverage map

Every fenced example in the Week 13 note is accounted for below.

| Source example | Relationship | Companion treatment |
|---|---|---|
| Tag-based circle/rectangle branch | Direct | Show the same conditional and motivate moving behavior behind objects |
| Shape-interface Mermaid diagram | Direct design | Preserve client → interface → concrete implementation relationship |
| `Point` and abstract `Shape` | Closest model | Dataclass plus `ABC`/`abstractmethod`; no virtual destructor requirement |
| `Circle`/`Rectangle final : Shape` | Direct behavior, weaker guarantees | Implement methods and invariants; Python has no `final` enforcement by default |
| Incorrect `Attack()` override | Important difference | ABC checks the method name, not compatible call signature; failure can occur at call time |
| Non-virtual-interface `GameObject` | Direct template-method pattern | Public `update` validates and calls protected-by-convention hook |
| Update sequence text | Direct | Preserve the ordered skeleton |
| `PrintArea(const Shape&)` | Direct dynamic dispatch | Pass an object reference and call `area` |
| By-value `Wrong(Shape)` slicing warning | No slicing equivalent | Python passes object references and preserves dynamic type |
| `vector<unique_ptr<Shape>>` | Same heterogeneous traversal, no ownership type | Store ordinary object references in a list |
| Recursive `Rule` composite tree | Direct design, different ownership | ABC nodes refer to children; exclusivity is not enforced |
| `Entity` composition | Direct | Store transform and sprite members/attributes |
| `Movement` strategy ownership | Direct strategy, weaker ownership | Inject a movement object; Python cannot enforce unique ownership |
| `dynamic_cast<Circle*>` | Closest model | Use `isinstance`, while questioning why concrete behavior is needed |
| `variant<Circle, Rectangle>` and `visit` | Partial | Use a union annotation and `isinstance`/pattern match; no closed exhaustive visit guarantee |

## From tags to an interface

The original conditional translates directly:

```python
if kind is ShapeKind.CIRCLE:
    draw_circle(circle)
elif kind is ShapeKind.RECTANGLE:
    draw_rectangle(rectangle)
```

It has the same maintenance problem: every new shape can require edits in many
conditionals. The interface diagram therefore transfers unchanged—client code
uses a `Shape`, while concrete objects supply their behavior.

## Abstract shape and concrete circle

```python
from abc import ABC, abstractmethod
from dataclasses import dataclass


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
```

`ABC` prevents direct construction while abstract methods remain. Python has no
need for a virtual destructor to destroy a derived object through a base
pointer; reclamation follows Python reachability and finalization rules.

```python
import math


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
    def __init__(
        self, corner: Point, width: float, height: float
    ) -> None:
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
            self._corner.x + self._width / 2.0,
            self._corner.y + self._height / 2.0,
        )

    def translate(self, offset: Point) -> None:
        self._corner = Point(
            self._corner.x + offset.x,
            self._corner.y + offset.y,
        )

    @property
    def area(self) -> float:
        return self._width * self._height
```

The invariant and dispatched behavior are direct. Python 3.9 has no built-in
`@override` decorator, and it does not enforce C++ `final`; tests and static
analysis must catch accidental mismatches.

## Override mismatch is less safely diagnosed

```python
class Actor(ABC):
    @abstractmethod
    def attack(self, damage: int) -> None:
        raise NotImplementedError


class BadMonster(Actor):
    def attack(self) -> None:
        pass
```

The method name satisfies Python's abstract-method bookkeeping, so construction
may succeed. `monster.attack(10)` then raises `TypeError` because the signature
does not accept the argument. C++ `override` would reject the mismatch during
compilation. This is a case where the C++ static guarantee is stronger.

## Template method / non-virtual interface

```python
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
```

The ordered skeleton—validate, before hook, derived hook, after hook—is direct.
The leading underscore asks clients not to call the hook directly, but it is a
convention rather than C++ private access. Python subclasses can override it
because method lookup remains dynamic.

## Dynamic dispatch and the absence of slicing

```python
def print_area(shape: Shape) -> None:
    print(shape.area)


circle = Circle(Point(0.0, 0.0), 2.0)
print_area(circle)
```

The call dispatches to `Circle.area`. Python passes an object reference, so
there is no by-value base copy that discards derived fields. A function
annotated with `Shape` still receives the original circle. The C++ slicing
example therefore has no faithful Python counterpart.

## Heterogeneous collections and ownership

```python
shapes: list[Shape] = [
    Circle(Point(0.0, 0.0), 2.0),
    Rectangle(Point(1.0, 1.0), 3.0, 4.0),
]

for shape in shapes:
    print(shape.area)
```

The heterogeneous traversal and dynamic dispatch are direct. The list holds
ordinary references, not `unique_ptr<Shape>`, and aliases elsewhere are allowed.
Removing an item does not prove immediate destruction or exclusive ownership.

## Composite rule tree

```python
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
        return (
            self.left.evaluate(input_value)
            + self.right.evaluate(input_value)
        )
```

Leaf/branch polymorphism and recursive evaluation transfer. Python attributes
do not enforce unique child ownership, so a subtree may be shared or cyclic
unless the design forbids it.

## Composition and strategy

```python
from dataclasses import dataclass


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
```

Composition is as direct as in C++: the entity is built from components rather
than inheriting their implementation.

```python
class Movement(ABC):
    @abstractmethod
    def next_position(self, current: Point, seconds: float) -> Point:
        raise NotImplementedError


class MovingMonster:
    def __init__(self, movement: Movement) -> None:
        if movement is None:
            raise ValueError("movement is required")
        self._movement = movement
```

Injecting behavior through a strategy object transfers directly. Python cannot
state that the monster uniquely owns the movement object; another reference may
reach the same strategy.

## Downcasting and closed alternatives

The closest dynamic-cast test is:

```python
if isinstance(shape, Circle):
    circle_specific_operation(shape)
```

It is safe as a run-time type test, but frequent use often signals that behavior
belongs in the interface or a visitor/operation object.

A union annotation describes a limited intended set:

```python
from typing import Union


ShapeValue = Union[Circle, Rectangle]


def area_of(shape: ShapeValue) -> float:
    return shape.area
```

Python still uses ordinary dynamic dispatch and does not store a C++ `variant`
tagged value or require an exhaustive `visit` at compile time. A static checker
may help, but run-time code can receive another object unless validated.

## What the Python versions must not hide

- Python ABCs do not provide C++ `override` signature checking or `final`.
- Python object passing has no C++ base-value slicing counterpart.
- Heterogeneous lists do not encode `unique_ptr` ownership or virtual
  destruction.
- `isinstance` resembles a checked downcast but does not justify concrete-type
  coupling.
- Union annotations are not closed `variant` storage with exhaustive visitation.
- The abstraction, template-method, Composite, composition, and strategy
  designs transfer strongly despite those differences.

Runnable examples are collected in [`python_examples.py`](python_examples.py).
