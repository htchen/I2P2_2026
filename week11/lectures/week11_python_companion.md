# Python Contrast Companion for Week 11

Read this companion beside the [Week 11 lecture
notes](week11_lecture.md). Container selection, sorting/searching, half-open
ranges, frequency maps, ranking, and expected absence transfer well. C++
template instantiation, iterator categories/invalidation, lambda capture, map
insertion semantics, and `optional` truth testing require careful contrast.

## Relationship labels

- **Direct algorithm:** the operation and result transfer closely.
- **Same goal, different static model:** Python type hints describe genericity
  but do not instantiate C++ templates.
- **Closest Python facility:** a built-in or standard-library function uses a
  different interface or guarantee.
- **No faithful equivalent:** the C++ iterator/lifetime rule is absent.

## Coverage map

Every fenced example in the Week 11 note is accounted for below.

| Source example | Relationship | Companion treatment |
|---|---|---|
| Function-template `Maximum` | Same goal, different static model | Use `TypeVar`; Python executes one dynamic function body |
| Integer/string template calls | Direct results | Call the same generic Python function with comparable values |
| Class-template `Box` | Same goal, different static model | Use `Generic[T]` dataclass/property |
| Generic absolute-value comparator | Closest facility | Use `sorted(values, key=abs)` rather than a comparator lambda |
| Lambda value capture of threshold | Different capture rule | Freeze a default argument or closure value explicitly |
| Vector/list layout diagram | Same design question, different object model | Contrast Python's dynamic array list with containers that follow different storage rules |
| `find` iterator and distance | Direct search goal | Return an index or `None` through `enumerate` |
| Vector reallocation and invalidation diagram | No faithful equivalent | Python iterators do not expose relocatable element addresses; preserve only the no-mutation rule |
| Sort/find-if/accumulate pipeline | Direct algorithm | Use `sorted`, `next`, and `sum` |
| `std::sort` versus `list::sort` | No direct container/API match | Python's `sorted` accepts any iterable and returns a new built-in list |
| Lower/upper/equal boundaries | Direct algorithm | Use `bisect_left` and `bisect_right` |
| Erase-remove idiom | Same filtering goal | Rebuild a list or assign a filtered slice |
| Frequency map insertion | Direct counting goal | Use `dict.get` or `Counter`; Python indexing does not default-insert on read |
| Map `find` lookup | Direct lookup goal | Use `get`/membership without mutation |
| Frequency-to-ranking vector | Direct | Sort dictionary items by negative count and word |
| `optional<size_t>` index | Direct absence model | Return `Optional[int]` and test `is not None`, not truthiness |

## Generic functions and classes

```python
from typing import TypeVar


T = TypeVar("T")


def maximum(left: T, right: T) -> T:
    return right if left < right else left
```

The type hint says both inputs and the result share a type, and a static checker
can require ordering. Python does not instantiate and compile a separate
function for every `T`; one function executes dynamically.

```python
largest_int = maximum(3, 8)
largest_word = maximum("ant", "bee")
```

The results match. Python's string ordering and integer behavior still follow
Python definitions, not C++ representation.

```python
from dataclasses import dataclass
from typing import Generic, TypeVar


T = TypeVar("T")


@dataclass(frozen=True)
class Box(Generic[T]):
    value: T
```

`Box[int]` is useful to a type checker, but normal Python execution does not
create the same specialized layout/code as `Box<int>` in C++.

## Callable behavior and capture

Python sorting usually asks for a key rather than a less-than comparator:

```python
values.sort(key=abs)
```

This produces the same absolute-value ordering goal. It computes keys and uses
Python's stable sort, so it is not a direct comparator-call trace. As in C++,
define behavior for values whose magnitude operation is problematic in the
chosen numeric domain; Python integers themselves have no `INT_MIN` case.

Python closures capture names, not C++ `[threshold]` value copies. Freeze the
current value explicitly when that is the desired comparison:

```python
threshold = 80
passed = lambda score, threshold=threshold: score >= threshold
```

Without the default argument, changing `threshold` later changes what the
lambda observes. That differs from the C++ capture-by-value example.

## Search positions and mutation

```python
from collections.abc import Sequence
from typing import Optional, TypeVar


T = TypeVar("T")


def index_of(values: Sequence[T], target: T) -> Optional[int]:
    for index, value in enumerate(values):
        if value == target:
            return index
    return None
```

The result corresponds to `distance(begin, position)` or absence at `end`.
Python's `list.index` is another option but raises `ValueError` on absence.

There is no faithful translation of a dangling C++ vector iterator:

```python
iterator = iter(values)
values.append(42)
```

Python does not expose an iterator as a pointer into relocatable element
storage. Its iterator may observe mutation in implementation- and
container-defined ways, leading to skipped, repeated, or newly visited logical
items rather than C++ undefined behavior from an invalid iterator. The shared
rule is simple: do not structurally mutate a collection while iterating unless
the API explicitly defines it.

The C++ layout diagram should therefore not be used to predict Python object
addresses. Python's built-in `list` is an implementation-managed dynamic array
of object references, while `collections.deque` and third-party containers have
different layouts. Python's `sorted(iterable)` accepts any iterable and returns
a new built-in list; it does not require the input itself to expose random-access
iterators. This differs from C++ `std::sort`, which rearranges an existing range
and rejects `std::list` iterators at compile time. The closest Python analogue
to the C++ map-to-vector ranking pipeline is `sorted(frequency.items(), ...)`,
which likewise materializes a sortable list of records.

## Algorithm pipeline

```python
ordered = sorted(values)
first_large = next((value for value in ordered if value >= 10), None)
total = sum(ordered)
```

The intent matches C++ sort/find-if/accumulate. Python returns a new list here;
`values.sort()` would mutate the original. `next(..., None)` requires a domain
where `None` cannot be a real element or a different sentinel object.

## Boundary algorithms

```python
from bisect import bisect_left, bisect_right


lower = bisect_left(values, target)
upper = bisect_right(values, target)
equal = (lower, upper)
```

The sorted/partitioned precondition and half-open equal range are the same.
Python returns integer indices, so there is no iterator-category distinction or
linear iterator-increment cost. List indexing provides random access.

## Filtering and mutation visibility

An independent filtered value is concise:

```python
values = [value for value in values if value >= 0]
```

This rebinds one name. Other aliases still refer to the old list. To preserve
the identity of the list object, use slice assignment:

```python
values[:] = [value for value in values if value >= 0]
```

Both differ from the C++ erase-remove mechanics and iterator invalidation, but
the filtering predicate and final sequence match.

## Frequency maps and ranking

```python
frequency: dict[str, int] = {}
for word in words:
    frequency[word] = frequency.get(word, 0) + 1
```

This explicitly supplies the default. Reading `frequency[word]` when the word
is absent raises `KeyError`; unlike C++ `map::operator[]`, it does not insert a
zero value merely by lookup. `collections.Counter(words)` is the higher-level
production alternative.

Non-mutating lookup is direct:

```python
if query in frequency:
    print(frequency[query])
```

Ranking by descending frequency and then ascending word uses a tuple key:

```python
ranking = sorted(frequency.items(), key=lambda item: (-item[1], item[0]))
```

The result corresponds to the vector of pairs and comparator in the C++
example. Python's stable sort and tuple comparison define the mechanics.

## Expected absence

```python
from collections.abc import Sequence
from typing import Optional


def index_of_integer(values: Sequence[int], target: int) -> Optional[int]:
    for index, value in enumerate(values):
        if value == target:
            return index
    return None


def report_index(values: Sequence[int], target: int) -> None:
    index = index_of_integer(values, target)
    if index is not None:
        print(f"found at {index}")
```

The explicit `is not None` check is essential because index zero is false in a
Python truth test. A C++ `optional<size_t>` converts to true when engaged even
if the stored index is zero.

## What the Python versions must not hide

- Type hints and `Generic` do not instantiate C++ templates.
- Python key functions are not comparator objects with C++ strict-weak-ordering
  call patterns.
- Closure capture defaults differ from C++ explicit value/reference captures.
- Python iteration mutation behavior does not explain C++ iterator invalidation.
- `dict` lookup does not share `map::operator[]`'s default-insertion behavior.
- `Optional[int]` must be checked against `None` so index zero remains valid.

Runnable versions are collected in [`python_examples.py`](python_examples.py).
