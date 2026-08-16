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

## 1. Substitutability, not code reuse, is the starting point

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

## 2. Override the contract

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

## 3. Dynamic dispatch requires indirection

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

## 4. Polymorphic ownership

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

Rule: if a class has any virtual function and objects may be deleted through a
base pointer, give it a public virtual destructor (or deliberately prevent such
deletion with a protected nonvirtual destructor in advanced designs).

## 5. Access control

- `public`: accessible through the interface.
- `protected`: accessible in the class and derived classes.
- `private`: accessible only to the class and its friends.

Prefer private data even in base classes. Protected data couples every derived
class to the representation. A protected helper function can expose a narrower
extension point.

Public inheritance normally preserves the public interface. Private inheritance
is closer to an implementation technique; composition is usually clearer.

## 6. Composition before inheritance

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

## 7. Downcasting is a design signal

```cpp
if (auto* circle = dynamic_cast<Circle*>(shape)) {
    /* Circle-specific behavior */
}
```

`dynamic_cast` safely checks a polymorphic dynamic type, but repeated type tests
may mean the base interface is missing an operation or the behavior belongs in
a visitor/variant. Never use `static_cast` to downcast unless a separate
invariant proves the dynamic type; otherwise behavior is undefined.

## 8. Closed alternatives with `std::variant`

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

## 9. Final-project reading strategy

For a legacy game hierarchy:

1. Find the abstract or common base.
2. List virtual functions and their contracts.
3. Find where objects are constructed and who owns them.
4. Trace one update and one draw call through dynamic dispatch.
5. Check whether destruction is virtual.
6. Identify where a composition or strategy would reduce subclass duplication.

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
