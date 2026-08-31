# Course Programming Style

The course adopts the [Google C++ Style
Guide](https://google.github.io/styleguide/cppguide.html) for C++ examples and
the [Google Python Style
Guide](https://google.github.io/styleguide/pyguide.html) for Python companion
examples. C17 uses a small, explicitly documented adaptation because Google
does not publish an equivalent complete C language guide. The C adaptation
keeps the same visual layout and safety principles while using common C naming
conventions.

Style supports readability; it does not replace correctness. A program must
first obey the relevant language standard, preserve its invariants, and express
ownership clearly.

## Shared C and C++ layout

- Indent C and C++ by two spaces and never use tabs in those languages. Keep
  syntax-required tabs in formats such as Make recipes.
- Keep ordinary lines within 80 columns when practical.
- Put an opening brace at the end of the declaration or control statement.
- Prefer braces for control-statement bodies. A short single statement may omit
  them only when it remains on one line; if one branch of a conditional uses
  braces, all branches use braces.
- Declare one variable per declaration when pointer or reference syntax could
  otherwise make the types difficult to read.
- Keep headers self-contained and include the matching project header first in
  an implementation file.
- Use comments to explain contracts, invariants, ownership, or non-obvious
  decisions. Do not merely translate each statement into prose.

The repository's `.clang-format` file encodes the mechanical subset of these
rules. Formatting is necessary but not sufficient: a formatter cannot choose a
clear name or establish a correct ownership policy.

## Python companion conventions

- Use four spaces for indentation and follow standard `snake_case` naming for
  functions, variables, and modules; use `CapWords` for classes.
- Write type hints when they clarify the intended comparison, but explicitly
  state that hints do not reproduce C/C++ storage, `const`, or ownership rules.
- Prefer standard Python values and context managers. Do not use `ctypes` or
  implementation-specific object-address tricks to imitate C pointers.
- Preserve the C/C++ example's inputs, outputs, and algorithm when a direct
  translation exists. When it does not, label the Python code as a closest
  model and explain what semantic property is missing.
- Keep runnable companion examples deterministic and free of third-party
  dependencies.

Python companions target Python 3.9 or newer and use modern built-in collection
annotations. Files that use `X | None` include
`from __future__ import annotations`, so the annotations are also accepted by
the Python 3.9 baseline.

## C17 conventions for Weeks 1–7

- Compile `.c` files as C17, never as C++.
- Name functions and variables with `snake_case`.
- Name structure and enumeration tags with `PascalCase`.
- Name macros with `UPPER_SNAKE_CASE`; prefer typed constants and functions when
  textual substitution is unnecessary.
- Use C declarations and C library facilities. Do not use references, classes,
  constructors, `new`, `delete`, namespaces, templates, or C++ standard-library
  types in C examples.
- Make array length, buffer capacity, pointer validity, allocation ownership,
  and release responsibility explicit in interfaces.

The C naming exception is deliberate. Applying C++ `MixedCase` function names
to the C half would obscure the language transition and conflict with common C
library practice without teaching a core programming concept.

## C++17 conventions for Weeks 8–14

- Name types with `PascalCase` and ordinary functions with `MixedCase`.
- Name variables and data members with `snake_case`; append `_` to private data
  members.
- Name namespace/class-scope and compile-time constants with `kPascalCase`, and
  macros with `UPPER_SNAKE_CASE`. A local read-only observation computed at run
  time may retain a descriptive `snake_case` variable name.
- Accessors may match the property name, such as `size()` or `value()`.
- Prefer `std::` qualification over a global `using namespace std;` directive.
- Prefer values, RAII, standard-library containers, and the Rule of Zero.
- Use raw pointers and direct `new`/`delete` only when the lesson specifically
  examines low-level representation or ownership.

Course files retain the familiar `.cpp` suffix even though Google commonly uses
`.cc`; the filename extension does not affect the language concepts being
taught. The course also remains on C++17 even if a newer revision of the Google
guide targets a later language version.

## Example

```cpp
class ScoreBook {
 public:
  explicit ScoreBook(const std::vector<int>& scores) : scores_(scores) {
  }

  double Mean() const;
  std::size_t size() const {
    return scores_.size();
  }

 private:
  std::vector<int> scores_;
};
```

The public operation uses `MixedCase`; the property accessor follows the
property spelling; layout and ownership are visible without dominating the
algorithm being taught.
