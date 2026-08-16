# Lecture 4 — Pointers, Lifetime, and Dynamic Memory

> September 29, 2026 · Source lineage: the legacy pointer, dynamic allocation,
> double-pointer, and linked-data notes

## Learning objectives

By the end of this lecture, you should be able to:

1. Read declarations involving objects, addresses, and pointers.
2. Distinguish stack duration, dynamic allocation, and object lifetime.
3. Allocate, resize, and free dynamic arrays safely.
4. Identify leaks, dangling pointers, null dereferences, and invalid access.
5. Express ownership and mutation through a function contract.

## 1. A pointer stores an address

```c
int value = 7;
int *pointer = &value;

printf("value=%d\n", value);
printf("address=%p\n", (void *)&value);
printf("through pointer=%d\n", *pointer);
```

- `&value` produces the address of `value`.
- `pointer` stores that address.
- `*pointer` designates the object at that address.
- The pointer type describes the pointed-to object and controls pointer arithmetic.

Read `int *pointer` as “pointer is a pointer to int.” In a multi-declaration, the
star belongs to each declarator:

```c
int *first;
int *second;
```

This is clearer than `int *first, second`, where `second` is not a pointer.

## 2. Pass an address to modify a caller's object

```c
void swap(int *left, int *right)
{
    int temporary = *left;
    *left = *right;
    *right = temporary;
}

int main(void)
{
    int a = 10;
    int b = 20;
    swap(&a, &b);
}
```

C still passes arguments by value: `left` receives a copy of `&a`. Both the
original address and its copy designate the same integer, so dereferencing the
copy modifies `a`.

Use `const` to separate observation from mutation:

```c
int sum(const int *values, size_t count); /* pointed-to ints are read-only */
```

## 3. Arrays and pointers are related, not identical

In most expressions, an array is converted to a pointer to its first element:

```c
values[i] == *(values + i)
```

But an array object and a pointer object differ:

- `sizeof array` is the storage for all elements in its declaration scope.
- `sizeof pointer` is the storage for one address.
- An array name cannot be assigned a new address.
- A pointer can be advanced or redirected if it is not `const`.

Pointer arithmetic is defined only within one array object (plus its one-past
position). You may form the one-past pointer for loop comparison, but not
dereference it.

## 4. Lifetime is different from scope

```c
int *bad_address(void)
{
    int local = 42;
    return &local; /* wrong: local's lifetime ends on return */
}
```

The returned pointer dangles. The variable name is out of scope, and more
importantly the object no longer exists. A valid pointer must designate a live
object (or be a permitted one-past pointer that is never dereferenced).

## 5. Dynamic allocation

Dynamic storage remains allocated until `free` releases it.

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int *read_values(size_t count)
{
    if (count > SIZE_MAX / sizeof(int)) return NULL;

    int *values = malloc(count * sizeof *values);
    if (values == NULL && count != 0) return NULL;

    for (size_t i = 0; i < count; ++i) {
        if (scanf("%d", &values[i]) != 1) {
            free(values);
            return NULL;
        }
    }
    return values;
}
```

At the call site:

```c
int *values = read_values(count);
if (values == NULL && count != 0) {
    fprintf(stderr, "could not read values\n");
    return 1;
}
/* use values */
free(values);
values = NULL;
```

Writing `sizeof *values` keeps the allocation correct if the pointed-to type is
changed. Check multiplication before allocation when sizes may be untrusted.

### `calloc` and `realloc`

- `calloc(count, size)` allocates and zeroes the bytes.
- `realloc(old, new_size)` may resize in place or move the allocation.

Never overwrite the only pointer before confirming `realloc` succeeded:

```c
int *resized = realloc(values, new_count * sizeof *values);
if (resized == NULL && new_count != 0) {
    /* values is still valid */
    handle_failure();
} else {
    values = resized;
}
```

## 6. Ownership contracts

For every pointer, ask:

1. May it be null?
2. How many elements are valid?
3. May the callee modify the pointed-to objects?
4. Who owns the allocation?
5. Who must free it, and when?
6. Can another pointer outlive the owner?

Examples:

```c
void print_values(const int *borrowed, size_t count);
int *values_clone(const int *source, size_t count); /* caller owns result */
void values_destroy(int **owned);                   /* releases and nulls */
```

The double pointer in `values_destroy` lets the function change the caller's
pointer as well as free the allocation:

```c
void values_destroy(int **owned)
{
    if (owned == NULL) return;
    free(*owned);
    *owned = NULL;
}
```

## 7. Failure patterns

| Failure | Meaning |
|---------|---------|
| Leak | The last usable pointer is lost before `free` |
| Dangling pointer | The pointer remains after the object's lifetime ends |
| Double free | The same allocation is released more than once |
| Invalid free | `free` receives an address not returned by allocation |
| Null dereference | `*pointer` is evaluated when `pointer == NULL` |
| Buffer overflow | Access goes before or beyond an allocation |

Compile memory-sensitive work with sanitizers:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic -g \
  -fsanitize=address,undefined program.c -o program
```

Sanitizers do not prove correctness, but they turn many silent errors into a
report close to the failing operation.

## Check yourself

1. Draw the objects and arrows after `int x = 3; int *p = &x;`.
2. Why is returning `&local` invalid but returning a `malloc` result possible?
3. What is the difference between `const int *p` and `int *const p`?
4. Write the ownership contract for `read_values`.
5. Explain why `values = realloc(values, bytes)` can leak memory.

## Summary

- A pointer is a typed address; dereferencing designates the pointed-to object.
- Valid access requires correct bounds, alignment, type, and lifetime.
- Dynamic allocation makes lifetime explicit and therefore makes ownership vital.
- Every successful allocation needs one eventual release on every path.
- Pointer contracts should state nullability, size, mutability, and ownership.
