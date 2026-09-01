# Week 13 Lecture Exercises: Runtime Polymorphism

[Starter code](week13_starter.cpp)

The lecture's scoring `Rule` hierarchy and this starter's numeric `Function`
hierarchy use the same leaf/interface pattern. The second domain lets you check
that the dispatch and ownership reasoning transfers instead of depending on
shape-specific names.

## Hour 1 — Substitution contract

The starter already supplies the abstract `Function` contract. Complete the
`Constant::Eval` and `Variable::Eval` overrides, call them through base
references, and explain what each `override` asks the compiler to verify.

## Hour 2 — Polymorphic ownership

Complete `MakeSampleFunctions` so a `FunctionList` owns heterogeneous objects
through `std::unique_ptr<Function>`, in the test driver's stated order:
`Constant{0.5}` followed by `Variable`. The virtual destructor is deliberately
already present in the interface; trace destruction and explain why removing it
would make owning deletion through a base pointer unsafe.

## Hour 3 — Composition through the same interface

Complete `Sum::Eval`. A `Sum` owns two `Function` children and is itself a
`Function`, so callers can evaluate a leaf or an entire expression through the
same interface. Draw the ownership tree, evaluate it through a base reference,
and explain why `unique_ptr` plus the virtual destructor releases the complete
tree.

The lecture's `std::variant` section is optional enrichment. If it is assigned,
translate the two leaf cases to a closed `variant` as a separate extension; it
is not a hidden requirement of this core starter.

## Compile and verify

From the repository root:

```sh
c++ -std=c++17 -Wall -Wextra -Wpedantic \
  week13/lectures/lecture_exercises/week13_starter.cpp -o /tmp/week13_starter
/tmp/week13_starter
```

After every TODO is correct, the final line is
`all Week 13 starter checks passed`. Then inspect
[`../examples.cpp`](../examples.cpp) for the completed hierarchy and the clearly
separated optional `variant` comparison.
