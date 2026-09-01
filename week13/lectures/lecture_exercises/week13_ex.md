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
through `std::unique_ptr<Function>`. The virtual destructor is deliberately
already present in the interface; trace destruction and explain why removing it
would make owning deletion through a base pointer unsafe.

## Hour 3 — Open versus closed alternatives

Complete `EvalValue` to model the same operation with `std::variant` and
`std::visit`. Compare the cost of adding a new type with the cost of adding a
new operation.
