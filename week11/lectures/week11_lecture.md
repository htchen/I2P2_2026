# Lecture 10 — Ownership, Copying, Moving, and the Rule of Zero

> November 17, 2026 · Source lineage: the legacy Classes II notes and IntVec
> implementation from the 2025 Week 8/10 notebooks

## Learning objectives

By the end of this lecture, you should be able to:

1. Explain destruction, copying, moving, and assignment for an owning class.
2. Diagnose shallow-copy and double-free defects.
3. Implement or deliberately disable special member functions.
4. Apply the Rule of Zero and use smart pointers appropriately.
5. Explain exception-safe resource transfer at a conceptual level.

## 1. Ownership must survive value operations

Consider a teaching vector that directly owns a dynamic array:

```cpp
class IntVec {
public:
    IntVec();
    explicit IntVec(std::size_t size);
    ~IntVec();

    friend void swap(IntVec& left, IntVec& right) noexcept;

private:
    int* data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
};
```

The destructor can release the array:

```cpp
IntVec::~IntVec()
{
    delete[] data_;
}
```

But the compiler-generated copy constructor copies the pointer value, not the
array. Two objects would then believe they own the same allocation and both
destructors would call `delete[]` on it.

## 2. The special member functions

The ownership-relevant operations are:

```cpp
IntVec();                              // default constructor
IntVec(const IntVec& other);           // copy constructor
IntVec& operator=(const IntVec& other);// copy assignment
IntVec(IntVec&& other) noexcept;       // move constructor
IntVec& operator=(IntVec&& other) noexcept; // move assignment
~IntVec();                             // destructor
```

They are invoked in different contexts:

```cpp
IntVec a{10};
IntVec b = a;            // copy construction
b = a;                   // copy assignment
IntVec c = std::move(a); // move construction; a remains valid but unspecified
```

`std::move` does not move by itself. It permits overload resolution to select an
rvalue-reference operation that may transfer resources.

## 3. Deep copying

```cpp
IntVec::IntVec(const IntVec& other)
    : data_{other.capacity_ == 0 ? nullptr : new int[other.capacity_]},
      size_{other.size_},
      capacity_{other.capacity_}
{
    if (size_ != 0) {
        std::copy_n(other.data_, size_, data_);
    }
}
```

Each copy owns a different allocation containing equal values. Be careful with
pointer arithmetic on null pointers. The guard avoids asking the algorithm to
form or use an empty range from `nullptr`.

Copy assignment must handle existing resources and self-assignment. The
copy-and-swap idiom gives a strong structure:

```cpp
void swap(IntVec& left, IntVec& right) noexcept
{
    using std::swap;
    swap(left.data_, right.data_);
    swap(left.size_, right.size_);
    swap(left.capacity_, right.capacity_);
}

IntVec& IntVec::operator=(const IntVec& other)
{
    IntVec copy{other};
    swap(*this, copy);
    return *this;
}
```

If copying throws, the original object is unchanged. On success, `copy` later
destroys the old allocation.

## 4. Moving transfers ownership

```cpp
IntVec::IntVec(IntVec&& other) noexcept
    : data_{other.data_},
      size_{other.size_},
      capacity_{other.capacity_}
{
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

IntVec& IntVec::operator=(IntVec&& other) noexcept
{
    if (this != &other) {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}
```

After a move, the source must remain valid and destructible. Its exact value is
usually unspecified unless the class documents something stronger.

Mark resource-transfer moves `noexcept` when true. Standard containers can then
move elements during reallocation without risking loss of the original data.

## 5. Rule of Three, Five, and Zero

- **Rule of Three:** if a class needs a custom destructor, copy constructor, or
  copy assignment, it probably needs all three.
- **Rule of Five:** in modern C++, also make a deliberate move-constructor and
  move-assignment decision.
- **Rule of Zero:** prefer members that already manage resources so your class
  needs none of these custom operations.

The best production `IntVec` representation is usually:

```cpp
class IntVec {
public:
    /* domain-specific operations */

private:
    std::vector<int> values_;
};
```

Now generated copy, move, assignment, and destruction have the correct meaning.
We implement a raw owning class once to understand the mechanism, then prefer
the Rule of Zero.

## 6. Smart pointers encode ownership

```cpp
#include <memory>

auto node = std::make_unique<Node>(value);
std::unique_ptr<Node> owner = std::move(node);
```

- `std::unique_ptr<T>`: exactly one owner; movable, not copyable.
- `std::shared_ptr<T>`: shared ownership using reference counting.
- `std::weak_ptr<T>`: observes a shared object without keeping it alive.

Do not use `shared_ptr` as a default replacement for deciding ownership. It has
run-time overhead and cycles of shared owners leak unless broken with `weak_ptr`.

Borrow with `T&`, `const T&`, or a non-owning `T*` according to nullability.
Ownership and access are different questions.

## 7. Explicitly disable unsupported operations

Some resources cannot sensibly be copied:

```cpp
class Connection {
public:
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;
    Connection(Connection&&) noexcept = default;
    Connection& operator=(Connection&&) noexcept = default;
};
```

Compile-time rejection is better than an accidental shallow copy.

## 8. Ownership in the final project

For each game object, answer:

- Which subsystem owns it?
- Is its lifetime scoped, unique, or shared?
- Which references are only observers?
- What happens when a scene changes or an entity is removed?
- Can a callback outlive the object it captures?

The demo may ask you to trace one object's creation, registration, use, removal,
and destruction through the multi-file codebase.

## Check yourself

1. Why does memberwise copying fail for an owning raw pointer?
2. What must be true of a moved-from object?
3. Why can `noexcept` affect vector reallocation?
4. When is `unique_ptr` better than `shared_ptr`?
5. Refactor a raw owning member to satisfy the Rule of Zero.

## Summary

- Destruction alone is insufficient when an owning object can be copied.
- Deep copy duplicates a resource; move transfers it and resets the source.
- Special member functions collectively define value and ownership semantics.
- The Rule of Zero delegates resource management to proven member types.
- Smart pointers express ownership; references and raw pointers commonly borrow.
