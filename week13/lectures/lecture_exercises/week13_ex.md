# Week 13 Lecture Exercises: Runtime Polymorphism

[Starter code](week13_starter.cpp)

## Hour 1 — Substitution contract

Complete the abstract `Function` interface and two overrides. Call them through
base references and mark every override explicitly.

## Hour 2 — Polymorphic ownership

Store heterogeneous functions in `std::unique_ptr<Function>`. Add a virtual
destructor, trace destruction, and explain why owning raw base pointers are
unsafe here.

## Hour 3 — Open versus closed alternatives

Model one operation once with virtual dispatch and once with `std::variant` plus
`std::visit`. Compare the cost of adding a new type with the cost of adding a
new operation.
