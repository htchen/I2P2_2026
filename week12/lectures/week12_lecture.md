# Lecture 11 — Inheritance and Runtime Polymorphism

> November 24, 2026 · Source lineage: the legacy Classes III material and the
> class-hierarchy section of the 2025 Week 7 notebook

## Learning objectives

By the end of this lecture, you should be able to:

1. Define an abstract interface with virtual functions.
2. Override behavior safely through references and pointers.
3. Explain why a polymorphic base needs a virtual destructor.
4. Avoid object slicing and unsafe downcasts.
5. Decide between inheritance, composition, and `std::variant`.

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | What contract makes derived objects genuinely substitutable? | Implement and test a Shape hierarchy |
| 2 | How do dispatch, destruction, and ownership interact? | Trace polymorphic calls and container lifetimes |
| 3 | When should a design use inheritance, composition, or variants? | Refactor one final-project hierarchy and defend the choice |

## Hour 1 — Abstract interfaces and substitutable overrides

### 1. Substitutability, not code reuse, is the starting point

Use public inheritance when every derived object can be used wherever the base
interface is expected without violating its contract.

```cpp
struct Point {
    double x;
    double y;
};

class Shape {
public:
    virtual Point center() const = 0;
    virtual void translate(Point offset) = 0;
    virtual double area() const = 0;
    virtual ~Shape() = default;
};
```

A pure virtual function (`= 0`) makes `Shape` abstract. It describes an
interface; `Shape shape;` is ill-formed because no complete base behavior exists.

### 2. Override the contract

```cpp
class Circle final : public Shape {
public:
    Circle(Point center, double radius)
        : center_{center}, radius_{radius}
    {
        if (radius < 0.0) {
            throw std::invalid_argument{"negative radius"};
        }
    }

    Point center() const override { return center_; }

    void translate(Point offset) override
    {
        center_.x += offset.x;
        center_.y += offset.y;
    }

    double area() const override
    {
        constexpr double pi = 3.141592653589793;
        return pi * radius_ * radius_;
    }

private:
    Point center_;
    double radius_;
};
```

`override` asks the compiler to verify that a base virtual function is actually
being overridden. It catches parameter, `const`, and spelling mismatches.
`final` prevents further derivation when the design intends a leaf class.

### Strengthen neither preconditions nor surprises

If `Shape::translate` accepts every finite offset, `Circle::translate` cannot
silently reject negative offsets. A derived operation may guarantee more in its
result but must not demand more from callers using the base contract.

Create contract tests that run against a `Shape&` and reuse them for every
derived type. This catches behavioral incompatibility that `override` syntax
alone cannot detect.

### Non-virtual interface pattern

```cpp
class GameObject {
public:
    void update(double seconds)
    {
        if (seconds < 0.0) throw std::invalid_argument{"negative time"};
        before_update();
        do_update(seconds);
        after_update();
    }
    virtual ~GameObject() = default;

private:
    virtual void do_update(double seconds) = 0;
    void before_update();
    void after_update();
};
```

The public nonvirtual function enforces common checks and sequencing; derived
classes customize only the intended step.

### Hour 1 studio

Add `Rectangle` and `CompositeShape`. Run the same center/translate/area contract
tests through `Shape&`. Intentionally omit `const` or change a parameter type and
observe how `override` converts a silent overload into a compile error.

## Hour 2 — Dynamic dispatch, ownership, and destruction

### 3. Dynamic dispatch requires indirection

```cpp
void print_area(const Shape& shape)
{
    std::cout << shape.area() << '\n';
}

Circle circle{{0.0, 0.0}, 2.0};
print_area(circle); /* calls Circle::area */
```

The function accepts a base reference, but the virtual call selects behavior
using the object's dynamic type at run time.

Passing by value would slice:

```cpp
void wrong(Shape value); /* abstract Shape makes this impossible here */
```

For a nonabstract base, copying a derived value into a base object discards the
derived part. Polymorphic APIs use references or pointers.

### 4. Polymorphic ownership

```cpp
#include <memory>
#include <vector>

std::vector<std::unique_ptr<Shape>> shapes;
shapes.push_back(std::make_unique<Circle>(Point{0, 0}, 2.0));
shapes.push_back(std::make_unique<Rectangle>(Point{1, 1}, 3.0, 4.0));

for (const auto& shape : shapes) {
    std::cout << shape->area() << '\n';
}
```

`unique_ptr<Shape>` owns one dynamically allocated object whose concrete type
may vary. Destroying through the base pointer calls the correct derived
destructor only because `Shape::~Shape` is virtual.

### What virtual dispatch stores conceptually

Typical implementations give a polymorphic object a hidden pointer to a table
of virtual functions. A call through `Shape&` uses that table to select the
derived override. The standard specifies behavior, not a particular table
layout; use this model for reasoning, not portable pointer arithmetic.

Virtual dispatch usually adds one indirection and may limit inlining. That cost
is often negligible next to game rendering or allocation, but measure when it
matters instead of eliminating abstraction speculatively.

### Destruction trace

Create a derived class with an owned vector/resource and instrument base and
derived destructors. Destroy through `unique_ptr<Shape>` and confirm derived
cleanup precedes base cleanup. Discuss what a nonvirtual base destructor would
fail to do without executing that undefined behavior.

Rule: if a class has any virtual function and objects may be deleted through a
base pointer, give it a public virtual destructor (or deliberately prevent such
deletion with a protected nonvirtual destructor in advanced designs).

### 5. Access control

- `public`: accessible through the interface.
- `protected`: accessible in the class and derived classes.
- `private`: accessible only to the class and its friends.

Prefer private data even in base classes. Protected data couples every derived
class to the representation. A protected helper function can expose a narrower
extension point.

Public inheritance normally preserves the public interface. Private inheritance
is closer to an implementation technique; composition is usually clearer.

### 6. Composition before inheritance

“A game entity has a position” suggests composition:

```cpp
class Entity {
private:
    Transform transform_;
    Sprite sprite_;
};
```

“A tower is a kind of game object that satisfies the game-object interface” may
justify inheritance. Ask:

1. Does the derived type preserve every base precondition and postcondition?
2. Will clients benefit from treating different derived types uniformly?
3. Is the hierarchy stable enough to become a public design commitment?
4. Would a member object or strategy object be simpler?

Inheritance solely to reuse a few lines often creates tighter coupling than a
composed helper.

### Strategy through composition

```cpp
class Movement {
public:
    virtual Point next(Point current, double seconds) = 0;
    virtual ~Movement() = default;
};

class Monster {
public:
    explicit Monster(std::unique_ptr<Movement> movement)
        : movement_{std::move(movement)} {}

private:
    std::unique_ptr<Movement> movement_;
};
```

Monster type and movement policy now vary independently. The monster uniquely
owns its strategy; a shared immutable strategy could instead be borrowed or
shared under an explicit lifetime design.

## Hour 3 — Downcasting, variants, and brownfield architecture

### 7. Downcasting is a design signal

```cpp
if (auto* circle = dynamic_cast<Circle*>(shape)) {
    /* Circle-specific behavior */
}
```

`dynamic_cast` safely checks a polymorphic dynamic type, but repeated type tests
may mean the base interface is missing an operation or the behavior belongs in
a visitor/variant. Never use `static_cast` to downcast unless a separate
invariant proves the dynamic type; otherwise behavior is undefined.

### 8. Closed alternatives with `std::variant`

When the set of alternatives is small and known, value-based polymorphism can be
clearer:

```cpp
using ShapeValue = std::variant<Circle, Rectangle>;

double area(const ShapeValue& shape)
{
    return std::visit([](const auto& value) { return value.area(); }, shape);
}
```

Tradeoff:

- Virtual hierarchy: easy to add a new derived type without changing consumers.
- Variant: easy to add a new operation with exhaustive handling of known types.

Neither is universally superior; choose based on which axis changes.

### Variant exhaustiveness exercise

Add a `Triangle` to `ShapeValue`. Every `std::visit` operation must compile for
the new alternative, so the compiler exposes incomplete operations. Contrast
this with adding a derived virtual class, which requires no edits to existing
virtual-call consumers but must implement every pure virtual operation.

### Multiple inheritance and interfaces

The course does not use implementation-heavy multiple inheritance. A class may
implement multiple small pure interfaces when it truly satisfies independent
contracts, for example `Updatable` and `Drawable`. Ownership should still have
one clear path; avoid diamond-shaped shared implementation hierarchies.

### 9. Final-project reading strategy

For a legacy game hierarchy:

1. Find the abstract or common base.
2. List virtual functions and their contracts.
3. Find where objects are constructed and who owns them.
4. Trace one update and one draw call through dynamic dispatch.
5. Check whether destruction is virtual.
6. Identify where a composition or strategy would reduce subclass duplication.

### Hour 3 architecture review

Choose one repeated type-switch or `dynamic_cast` chain in the project template.
Propose three alternatives: add a virtual operation, introduce a composed
strategy, or use a variant/visitor. Evaluate extension direction, ownership,
testability, number of affected files, and migration risk before choosing.

## Check yourself

1. Why is `override` more than documentation?
2. Explain object slicing with a concrete example.
3. Why does `unique_ptr<Base>` require a virtual base destructor?
4. Model “monster has movement behavior” using composition.
5. When would `variant` be preferable to a virtual hierarchy?

## Summary

- Public inheritance models substitutable interfaces.
- Virtual dispatch operates through references and pointers.
- Polymorphic bases need a deliberate destruction policy.
- `override`, private state, and ownership-aware containers prevent common bugs.
- Prefer composition unless run-time substitutability provides real value.
