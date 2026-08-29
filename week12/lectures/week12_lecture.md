# Week 12 Lecture Notes — Ownership, Copying, Moving, and the Rule of Zero

> November 24, 2026 · Source lineage: the legacy Classes II notes and IntVec
> implementation from the 2025 Week 8/10 notebooks

## Learning objectives

By the end of this lecture, you should be able to:

1. Explain destruction, copying, moving, and assignment for an owning class.
2. Diagnose shallow-copy and double-free defects.
3. Implement or deliberately disable special member functions.
4. Apply the Rule of Zero and use smart pointers appropriately.
5. Explain exception-safe resource transfer at a conceptual level.
6. Choose between a uniquely owned tree and a deliberately shared immutable
   object graph when composite operations reuse substructure.

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | Why does an owning class need coordinated lifetime operations? | Trace the legacy IntVec representation and destructor |
| 2 | What are the exact semantics of copy, assignment, and move? | Implement/test special members including failure paths |
| 3 | How should production code express ownership with Rule of Zero and smart pointers? | Refactor IntVec and map recursive/final-project lifetimes |

## Hour 1 — Owning representations and deterministic destruction

### 1. Ownership must survive value operations

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

### Size, capacity, and growth invariant

The 2025 IntVec notebook stored begin, logical end, and allocation end pointers.
This version stores a pointer plus counts; both need the same invariant:

```text
0 <= size_ <= capacity_
data_ == nullptr exactly when capacity_ == 0
data_[0 .. size_) contains live values owned by this object
```

```cpp
void IntVec::reserve(std::size_t requested)
{
    if (requested <= capacity_) return;

    int* replacement = new int[requested];
    if (size_ != 0) std::copy_n(data_, size_, replacement);
    delete[] data_;
    data_ = replacement;
    capacity_ = requested;
}
```

For a generic element type, use an RAII temporary so an exception during copying
cannot leak. The `int` specialization is a teaching step toward `std::vector<T>`.

### Hour 1 lifetime trace

Instrument every constructor and destructor with object address, data address,
size, and capacity. Trace default construction, growth, nested scope exit, and
return-by-value. Separate guaranteed language behavior from optional copy
elision; log count is not the abstraction's contract.

## Hour 2 — Copy, move, assignment, and exception guarantees

### 2. The special member functions

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

### 3. Deep copying

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

### Construction versus assignment

| Operation | Destination already owns a resource? | Required behavior |
|-----------|---------------------------------------|-------------------|
| Copy construction | No | allocate and copy from source |
| Copy assignment | Yes | preserve source and replace destination safely |
| Move construction | No | acquire source resource and leave source valid |
| Move assignment | Yes | release destination, acquire source, handle self-move |

Construction creates lifetime; assignment operates within an existing lifetime.
This distinction is why one implementation cannot blindly serve every case.

### 4. Moving transfers ownership

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

### Exception-safety levels

- **No-throw guarantee:** operation cannot fail by exception.
- **Strong guarantee:** failure leaves the original observable state unchanged.
- **Basic guarantee:** failure preserves invariants and leaks nothing, but the
  value may change.
- **No guarantee:** even invariants may be lost.

Copy-and-swap commonly provides the strong guarantee. A direct assignment that
deletes old storage before allocating new storage loses the original value when
allocation throws.

### Aliasing and self-assignment lab

Test `value = value`, `value = std::move(value)`, assignment between empty and
nonempty vectors, and copying when capacity exceeds size. Use distinct values to
detect copying uninitialized capacity instead of logical elements. Force an
allocation failure through a teaching hook and verify the promised guarantee.

## Hour 3 — Rule of Zero, smart pointers, and project ownership

### 5. Rule of Three, Five, and Zero

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

### 6. Smart pointers encode ownership

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

### Unique ownership in a tree

```cpp
struct Node {
    int value;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

auto root = std::make_unique<Node>();
root->value = 10;
root->left = std::make_unique<Node>();
```

Recursive destruction is automatic. The type is movable but not copyable unless
deep copy is explicitly implemented. Search functions can return `Node*` or
`const Node*` as borrowers while the tree retains ownership.

### Recursive composites: tree or shared graph?

A composite model may contain leaf objects and branch objects whose children
share the same abstract interface. Evaluation merely borrows the children, but
destruction, copying, and transformations force an ownership decision.

Choose one coherent model:

1. **Unique tree:** each parent stores uniquely owned children. Destruction is
   automatic and local. Reusing an unchanged subtree in a new result requires a
   deep clone or a move that removes it from the original.
2. **Shared immutable graph:** nodes are immutable and held through shared
   ownership. A transformation may safely reuse unchanged subtrees, but cycles
   must be impossible or broken with weak references, and sharing must be a
   deliberate semantic property.
3. **Borrowed graph:** an external arena or document owns all nodes and edges
   borrow them. This can be valid, but the arena lifetime must exceed every
   evaluation and transformation.

Do not combine owning raw child pointers with ad hoc subtree reuse. Adding a
destructor later can turn leaks into double deletion, while omitting one leaves
the entire graph leaked. A raw pointer may still be a clear non-owning observer
after owners are established elsewhere.

### Exception-safe composite construction

Build owned children into RAII objects before publishing the parent. If later
allocation or validation throws, already constructed smart pointers release
their subtrees automatically. Dense expressions containing several raw `new`
operations hide which allocations survive when one operation throws.

Private constructors introduce another design question: `std::make_unique`
normally performs construction inside the library template, where private
access is unavailable. Options include a public constructor with a named free
factory, a controlled construction token, returning a concrete value, or a
carefully implemented class factory that returns a smart pointer. Choose the
smallest mechanism that preserves the invariant; do not fall back to an owning
raw pointer merely to bypass access control.

### Ownership design exercise

For a generic scoring-rule tree with literal, input, unary, and binary nodes,
draw both the unique-tree and shared-immutable representations. Then consider a
transformation that returns a new rule while preserving the original. Mark
which subtrees must be cloned, may be shared, or may only be borrowed. Stop at
the ownership map and exception paths; no transformation implementation is
provided.

### Shared ownership cycles

If two `shared_ptr` objects own each other, neither reference count reaches zero.
Model a parent observer as `weak_ptr` when children own descendants. Draw strong
and weak edges; “shared because many places use it” is not a lifetime design.

### 7. Explicitly disable unsupported operations

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

### 8. Ownership in the final project

For each game object, answer:

- Which subsystem owns it?
- Is its lifetime scoped, unique, or shared?
- Which references are only observers?
- What happens when a scene changes or an entity is removed?
- Can a callback outlive the object it captures?

The demo may ask you to trace one object's creation, registration, use, removal,
and destruction through the multi-file codebase.

### Hour 3 final-project audit

Choose one tower, monster, scene, and asset. For each, record owner type,
creation site, transfers, borrowers/callbacks, removal event, and destruction
site. Flag raw owning pointers, reference captures that may outlive objects, and
`shared_ptr` without genuine shared lifetime. Propose the smallest Rule-of-Zero
or `unique_ptr` improvement and describe its integration risk.

In Thursday's lab, turn the ownership map into one bounded improvement. Carry it
through construction, observation, removal, cleanup, tests, and sanitizer
verification without attempting a repository-wide pointer rewrite. This Week
12 material is included in the Midterm 2 scope; Week 13 material is
excluded because it is first presented two days before the exam.

## Check yourself

1. Why does memberwise copying fail for an owning raw pointer?
2. What must be true of a moved-from object?
3. Why can `noexcept` affect vector reallocation?
4. When is `unique_ptr` better than `shared_ptr`?
5. Why can reusing a raw child pointer make ownership ambiguous?
6. When is immutable subtree sharing preferable to deep cloning?
7. Refactor a raw owning member to satisfy the Rule of Zero.

## Summary

- Destruction alone is insufficient when an owning object can be copied.
- Deep copy duplicates a resource; move transfers it and resets the source.
- Special member functions collectively define value and ownership semantics.
- The Rule of Zero delegates resource management to proven member types.
- Smart pointers express ownership; references and raw pointers commonly borrow.
- Recursive composites require one coherent tree, shared-graph, or arena
  lifetime model before transformations are implemented.

## References and legacy sources

- [Classes II](<https://github.com/htchen/i2p-nthu/blob/master/程式設計二/Classes%20II/README.md>)
- [2025 Week 8 notebook (Colab)](https://colab.research.google.com/drive/1qkDyeCDzzislM1BN8XSoUJxiomuT3cv8)
- [2025 Week 10 notebook (Colab)](https://colab.research.google.com/drive/1WzsdyfHgyjojg6DsJm_NH5pANk2GB18t)
