# Lecture Source Map

This document records how the Fall 2026 lecture notes reuse and reorganize the
two previous course archives and additional instructor materials. It is intended
to make future revision easier and to prevent useful material from disappearing
during the C/C++ course merger.

## Source archives

1. [`htchen/i2p-nthu`](https://github.com/htchen/i2p-nthu) — the earlier,
   separate C and C++ course notes.
2. [`I2P2_2025`](https://drive.google.com/drive/u/0/folders/1kIu5d8L7o3Dj5aY5_76_M_2SXgPSm6w9)
   — the 2025 Colab notebooks (`Week01.ipynb` through `Week15.ipynb`, with some
   week numbers intentionally absent or split).
3. Instructor-provided [*From C to Assembly* handout](assets/references/from_c_to_assembly.pdf)
   (2020) — a compact x86-oriented source on the compilation pipeline, program
   sections, addresses and memory access, control flow, calls, stack frames,
   recursion, and optimization. The architecture-specific examples are adapted
   as observation exercises rather than presented as portable C rules.
4. Instructor-provided [*Assembly* slides](assets/references/lee_assembly.pptx)
   — historical x86 examples used only for optional address, control-flow, and
   call-stack observations after the portable C model is established.
5. Instructor-provided [*Josephus Problem* slides](assets/references/josephus_lee.pptx)
   — the problem statement, small-circle traces, algorithm comparisons, and
   circular-list diagrams adapted into the Week 5 student explanation.

The new notes are a condensed adaptation, not a week-for-week copy. Explanations
were rewritten around students who already know Python, examples were updated to
make ownership contracts explicit, and overlapping legacy lessons were merged.

## Code-example reuse policy

When a 2025 notebook already contains a useful teaching example, the new notes
retain its recognizable problem, data, or execution trace instead of inventing
an unrelated replacement. Reuse is selective rather than verbatim:

- keep the original example when its prerequisites have already been taught;
- split a long notebook program when one smaller example teaches the idea more
  clearly;
- repair undefined behavior, incomplete definitions, unchecked allocation, and
  ambiguous contracts before presenting the code to students;
- translate Weeks 1–7 examples to strict C17 and use C++17 only after the
  C-to-C++ transition in Week 8; and
- apply the repository's Google-derived style without changing the algorithm
  or observation that made the example useful.

The table below names the notebook examples actually retained. “Adapted” means
that the instructional core is reused while the interface, safety checks,
decomposition, or naming has been revised for this course.

## Python contrast companions

All 13 lecture weeks use a student-facing Python companion format. Together the
companions cover direct algorithmic translation, C pointers and manual lifetime
with no faithful equivalent, the C-to-C++ transition, C++ copy/move ownership
semantics, generic programming, polymorphism, and graph search.

Each companion inventories every fenced example in its source lecture and uses
one of three treatments:

1. preserve inputs, outputs, and algorithm in a direct Python translation;
2. show the closest ordinary Python model and name the changed contract; or
3. explain why translating the mechanism would be misleading.

The runnable `python_examples.py` files test the meaningful translations, while
the companion prose accounts for shell commands, diagrams, declarations, and
non-equivalent lifetime mechanisms. Coverage maps must be reviewed whenever a
fenced example changes in its source lecture.

## Mapping to Lecture Weeks

| Lecture week | Main legacy GitHub material | 2025 notebook material | Reorganization |
|--------------|-----------------------------|------------------------|----------------|
| Week 1 — From Python to C | `程式設計一/Introduction to programming`, `Printf and Scanf`, `Operators, Expressions, and Statements`, `Looping` | — | Compresses the former introductory sequence into one contrastive lecture and adds the complete build pipeline, assembly observation, common binary sections, and undefined behavior. |
| Week 2 — Functions, Arrays, and Strings | `程式設計一/function`, `array`, `Input and output`, `Printf and Scanf/String type` | — | Treats arrays and strings as explicit representations of Python-familiar sequence ideas, adds prefix and sorted-boundary query patterns, and gives a minimal address-passing bridge; multidimensional arrays, in-place editing, and custom sorting are retained as optional extensions. |
| Week 3 — Structures, Modules, Builds, and Debugging | `Supplementary Material 2`, `如何compile多個檔案`, `Programming related Topic/Debug.md`, `程式撰寫風格.md` | — | Combines record design, invariants, interfaces, linking, and evidence-driven debugging. |
| Week 4 — Pointers, Lifetime, and Dynamic Memory | `程式設計一/pointer`, `Supplementary Material 1` | Linked-node allocation examples in `Week01.ipynb` | Adapts the node-allocation examples to introduce ownership and allocation failure before lists; machine-code details are a labeled optional observation after the portable pointer model. |
| Week 5 — Linked Lists | `程式設計二/mid1/2-linked_list.md`, `2-linked_list_sup.md`, `3-josephus_problem.md` | `Week01.ipynb`, `Week02.ipynb`, linked-list part of `Week03.ipynb` | Retains the pointer-to-pointer insertion/deletion shape, circular-list Josephus simulation, and iterative Josephus recurrence. Interfaces now state ownership and failure behavior, and all allocations are checked. |
| Week 6 — Recursion and Binary Trees | `程式設計一/Recursive`, `程式設計二/mid1/4-binary_tree.md` | tree material in `Week03.ipynb` and `Week04_1.ipynb` | Retains recursive size and traversal examples and the preorder/inorder reconstruction pattern, then adds stack-overflow risk, Hanoi, N queens, destruction, ambiguity, and complexity. Expression-tree evaluation moves to Week 7. |
| Week 7 — Expression Parsing and Syntax Trees | `程式設計二/mid1/5-syntax_tree.md`, `6-Computer.md`, `7-Assembly.md`, `8-calculator_tree.pdf.md` | `Week04_2.ipynb`, `Week05.ipynb` | Adapts the notebook calculator's lexer, recursive-descent parser, syntax tree, evaluator, and code generator into one consistent, fully defined C17 pipeline. |
| Week 8 — From C to C++ | `程式設計二/Intro` | introductory sections of `Week07.ipynb` | Retains the notebook's string/vector demonstrations and three-way value/pointer/reference swap trace. Lambdas and `decltype` are intentionally omitted; lambdas begin with algorithms in Week 11. |
| Week 9 — Classes, Invariants, and Operators | `程式設計二/Classes I` | Rational/class material in `Week07.ipynb` | Rebuilds the notebook `Rational` example around a normalized representation, constructor validation, `const` observation, and conventional arithmetic/output operators. |
| Week 11 — Templates and the Standard Library | template section of `Classes III`, `week 12 Standard library` | standard-library portion of `Week11.ipynb`; `std::optional` in `Week14.ipynb` | Retains container/algorithm examples while adding explicit range contracts, lower/upper boundaries, expected-absence results, complexity, and iterator invalidation early enough for Midterm 2. |
| Week 12 — Ownership, Copying, Moving, and Rule of Zero | `程式設計二/Classes II` | `Week08.ipynb`, `Week10.ipynb` | Retains the educational `IntVec` representation and copy/move lifetime trace, repairs incomplete operations, and contrasts the result with a Rule-of-Zero `vector` owner. |
| Week 13 — Inheritance and Runtime Polymorphism | derived-class section of `程式設計二/Classes III` | polymorphism section of `Week07.ipynb`; `std::variant` in `Week14.ipynb` | Retains the notebook `Shape` hierarchy and heterogeneous traversal, adding `override` diagnostics, virtual destruction, explicit ownership, Composite, and composition/strategy. |
| Week 14 — Graphs and State-Space Search | `week 13 找路徑`, `week 14 Water Jugs Problem`, `week 16 Missionary and Torch` | graph/search material in `Week11.ipynb`, `Week12.ipynb`, `Week13.ipynb` | Retains geometric connected components, queue-based shortest paths, and Water Jugs successor generation while separating graph model, search policy, and path explanation. |

## Material retained for later use

The following source topics are useful but do not fit the 13-note core:

- advanced C library exercises, file processing, bit operations, unions, stacks,
  queues, and doubly linked lists;
- architecture-specific x86 syntax, historical 32-bit calling-convention
  details, and assembly implementation details beyond the conceptual bridges
  needed for the midterm compiler's simulated target;
- C++20 ranges, concepts, modules, parallel algorithms, and advanced `optional`
  or `variant` details from `Week14.ipynb` beyond the optional comparison in the
  Week 13 lecture notes;
- Python/C++ interoperability, CUDA, and later-language previews from
  `Week15.ipynb`;
- extended Josephus variants and the Ninety-Nine Problems collection.

These are candidates for labs, project extensions, optional readings, and
post-course enrichment rather than required Tuesday lectures.

## Adaptation principles

- Assume Python fluency; spend time on representation, lifetime, ownership, and
  compiled behavior.
- Preserve the legacy project spine: linked data → trees → syntax tree/compiler,
  followed by C++ abstractions and a multi-file game.
- Organize every Tuesday lecture into three instructional hours, each ending in
  a trace, design checkpoint, live implementation, or verification activity.
- Prefer one coherent example per idea over several near-duplicate versions.
- State nullability, bounds, ownership, invariants, and failure behavior.
- Use warnings, sanitizers, tests, and small trace questions throughout.
- Teach raw resource management to explain the model, then guide students to
  RAII, standard containers, smart pointers, and the Rule of Zero.
- Keep source-stage boundaries testable so students can audit AI-generated code.

## Attribution

The `htchen/i2p-nthu` repository is distributed under the MIT License:

> Copyright (c) 2017 htchen

> Permission is hereby granted, free of charge, to any person obtaining a copy
> of this software and associated documentation files (the "Software"), to deal
> in the Software without restriction, including without limitation the rights
> to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
> of the Software, and to permit persons to whom the Software is furnished to do
> so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all
> copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
> IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
> FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
> AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
> LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
> OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
> SOFTWARE.

The 2025 notebooks are course materials supplied by the course owner for this
revision. Their notebook names are retained above for internal provenance.
