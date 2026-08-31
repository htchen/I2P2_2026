# Development Environment Setup

## Table of Contents

1. [Prerequisites](#1-prerequisites)
2. [Compiler Setup](#2-compiler-setup)
3. [Build Setup](#3-build-setup)
4. [Verification](#4-verification)
5. [Troubleshooting](#5-troubleshooting)

---

## 1. Prerequisites

Install a command-line C/C++ toolchain that provides one of these pairs:

- Clang: `clang` and `clang++`
- GCC: `gcc` and `g++`

The course baseline is **C17** for C and **C++17** for C++. Python contrast
companions require **Python 3.9 or newer**. A terminal, Git, a text editor, and a
debugger are also required. The final-project template has additional Allegro 5
dependencies; follow its pinned release instructions after the baseline checks
below succeed.

Do not commit generated executables, object files, IDE metadata, or build
directories.

## 2. Compiler Setup

Check that both drivers are available:

```sh
cc --version
c++ --version
python3 --version
```

On systems where `cc`/`c++` are unavailable, substitute `clang`/`clang++` or
`gcc`/`g++` in the commands below. Course C programs use:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic -g program.c -o program
```

Course C++ programs use:

```sh
c++ -std=c++17 -Wall -Wextra -Wpedantic -g program.cpp -o program
```

Warnings are part of the development feedback loop. Do not silence a warning
without explaining why the code is nevertheless correct.

## 3. Build Setup

Single-file lecture examples can be compiled with the commands above. For a
multi-file C program, compile and link every implementation file:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic -g main.c module.c -o program
```

For a multi-file C++ program:

```sh
c++ -std=c++17 -Wall -Wextra -Wpedantic -g main.cpp component.cpp -o program
```

If a supplied project uses CMake, keep generated files outside the source tree:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Use the build instructions shipped with an assigned project when they differ
from these generic commands.

## 4. Verification

Compile the repository's standalone examples from its root:

```sh
for source in week0[1-7]/lectures/examples.c; do
    cc -std=c17 -Wall -Wextra -Wpedantic "$source" -o /tmp/i2p2-example
done

for source in week0[89]/lectures/examples.cpp week1[1-4]/lectures/examples.cpp; do
    c++ -std=c++17 -Wall -Wextra -Wpedantic "$source" -o /tmp/i2p2-example
done

for source in week*/lectures/python_examples.py; do
    python3 "$source"
done
```

For memory-sensitive exercises on a compiler that supports sanitizers, add:

```sh
-fsanitize=address,undefined -fno-omit-frame-pointer
```

Run a normal case, a boundary case, an invalid case when the interface permits
one, and a cleanup/failure case before treating a build as verified.

## 5. Troubleshooting

- **Compiler command not found:** install a C/C++ command-line toolchain, open a
  new terminal, and repeat the version checks.
- **A source file is compiled as the wrong language:** use `cc` with `.c` files
  and `c++` with `.cpp` files.
- **Header not found:** confirm that the dependency is installed and add only
  the documented include path; do not copy a system header into the project.
- **Linker reports an undefined symbol:** ensure the implementation file or
  required library appears in the final link command.
- **Program works locally but fails in the judge:** reproduce the published
  compiler standard, remove reliance on undefined behavior, and test exact
  boundary and input-format cases.
- **Sanitizer is unavailable:** record the compiler/platform limitation and use
  the staff-approved equivalent tool. Sanitizer availability does not excuse
  missing ownership and cleanup reasoning.

When requesting help, include the exact command, full diagnostic, compiler
version, operating system, and the smallest reproducible source example.
