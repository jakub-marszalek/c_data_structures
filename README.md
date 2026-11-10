# c_data_structures

[![CI](https://github.com/jakub-marszalek/c_data_structures/actions/workflows/ci.yml/badge.svg)](https://github.com/jakub-marszalek/c_data_structures/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
![Language: C](https://img.shields.io/badge/language-C-informational)

A collection of small, dependency-free data structures written in pure C, 
built with embedded and automotive software in mind: no hidden global state, 
explicit error handling, and every structure tested under AddressSanitizer 
and UBSan on each push.

## Data structures

| Structure | Status |
|---|---|
| [Dynamic vector (`int`)](#vector) | Done |
| Ring buffer | Planned |
| Linked list | Planned |

The vector is the first one on purpose: it is the simplest case to get
the fundamentals right - ownership rules, allocation failures, overflow checks,
API design and the test and CI setup. The same conventions will be applied to the next 
structures in future.

## Vector

A dynamic array of `int` with an opaque type and status-code based error handling.
It is two files (`include/vector.h`, `src/vector.c`) you can use in any project.

## Features

- Opaque `vector_t` - the layout is hidden in `vector.c`, callers only use the API
- Amortized O(1) `vector_push_back` with capacity doubling (initial capacity: 4)
- Random access, `set`, `insert` and `remove` at any index (check vector.h for details)
- Explicit error handling: every function (except create and destroy) returns a `vector_status_t`, nothing is silently ignored
- Allocation failures are survivable: if `realloc` fails, the vector is left unchanged
- Capacity-doubling overflow is checked before the size is enlarged
- No global state, no dependencies beyond the C standard library
- Builds cleanly with `-Wall -Wextra -Wpedantic -Wshadow -Wconversion -Werror`

## Quick start

```c
#include <stdio.h>
#include "vector.h"

int main(void)
{
    vector_t *vec = vector_create();
    if (NULL == vec) {
        return 1;
    }

    for (int i = 1; i <= 5; i++) {
        if (vector_push_back(vec, i * 10) != VEC_OK) {
            vector_destroy(vec);
            return 1;
        }
    }

    int value;
    if (vector_get(vec, 2, &value) == VEC_OK) {
        printf("vec[2] = %d\n", value);   /* vec[2] = 30 */
    }

    vector_destroy(vec);
    vec = NULL;
    return 0;
}
```

A longer example is in [`examples/vector_demo.c`](examples/vector_demo.c).

## Build and test

Requirements: a C compiler (gcc) and `make`.

```sh
make test    # build and run the test suite
make clean   # remove build/ directory
```

To use the library in your own project, copy `include/vector.h` and
`src/vector.c`, add `include/` to your include path and compile `vector.c`
together with your sources.

## API

All functions except `vector_create()` and `vector_destroy()` return a
`vector_status_t`.

| Function | Description |
|---|---|
| `vector_create()` | Allocate a new empty vector. Returns `NULL` on failure. |
| `vector_destroy(vec)` | Release the vector. Passing `NULL` is safe. |
| `vector_size(vec, &out)` | Number of stored elements. |
| `vector_capacity(vec, &out)` | Number of elements that fit without reallocation. |
| `vector_push_back(vec, value)` | Append an element, growing the buffer if needed. |
| `vector_pop_back(vec, &out)` | Remove and return the last element. |
| `vector_get(vec, i, &out)` | Read the element at index `i`. |
| `vector_set(vec, i, value)` | Overwrite an existing element at index `i`. |
| `vector_insert(vec, i, value)` | Insert at `i`, shifting the tail right. |
| `vector_remove(vec, i, &out)` | Remove the element at `i`, shifting the tail left. |
| `vector_clear(vec)` | Set size to 0. Capacity is kept. |

Status codes:

| Code | Meaning |
|---|---|
| `VEC_OK` | Success |
| `VEC_ERR_NULL` | A `NULL` vector or output pointer was passed |
| `VEC_ERR_EMPTY` | The operation needs a non-empty vector |
| `VEC_ERR_RANGE` | Index outside the valid range |
| `VEC_ERR_NOMEM` | Memory allocation failed |

Full documentation of every function is in [`include/vector.h`](include/vector.h).

### Complexity

| Operation | Time |
|---|---|
| `push_back` | amortized O(1) |
| `pop_back`, `get`, `set`, `size`, `capacity`, `clear` | O(1) |
| `insert`, `remove` | O(n) |

## Design notes

- **Opaque type.** `struct vector_t` is defined only in `vector.c`. Callers cannot touch `size` or `data` directly, so the invariants (`size <= capacity`, `data` always valid) cannot be broken from outside.
- **Status codes instead of `assert`/`abort`.** A library should not decide to kill the program. Errors are returned and the caller chooses what to do.
- **Strong guarantee on failure.** Growth goes through a temporary pointer (`realloc` result is checked before it replaces `data`), so a failed allocation leaves the vector exactly as it was. `vector_insert` and `vector_remove` also leave the vector and output value untouched when they fail.
- **Overflow-safe growth.** Before doubling the capacity, the code checks that `capacity * sizeof(int) * 2` cannot overflow `size_t`.
- **Clear ownership.** The caller owns whatever `vector_create()` returns and releases it with `vector_destroy()`. `clear` and `remove` never shrink the buffer.

## Testing

The test suite (`tests/test_vector.c`) contains 38 test cases built on plain
`assert()` - no framework required. It covers:

- creation and destruction, repeated many times to catch leaks
- `NULL` vector and `NULL` output pointers for every function
- empty-vector behavior for `pop_back`, `get`, `set`, `remove`
- capacity growth exactly at the boundary and after 100 000 pushes
- insert and remove at the beginning, middle and end, with and without reallocation
- index range checks on both sides of every valid range
- independence of two vectors alive at the same time

Tests are compiled with `-fsanitize=address,undefined`, so out-of-bounds access,
use-after-free, leaks, signed overflow and other undefined behavior fail the build.

## Continuous integration

GitHub Actions ([`.github/workflows/ci.yml`](.github/workflows/ci.yml)) builds the
library and runs the full test suite on every push to `main` and `develop` and on every pull request.

## Project structure

```
c_data_structures/
├── include/vector.h        public API
├── src/vector.c            implementation
├── tests/test_vector.c     test suite
├── examples/vector_demo.c  usage example
├── .github/workflows/      CI
├── makefile
└── LICENSE
```

## Limitations (vector)

- Stores `int` only. Making it generic would mean either a `void *` plus element size or macros/code generation.
- Not thread-safe. Synchronize access externally.
- The buffer never shrinks automatically.

## License

[MIT](LICENSE) © 2026 Jakub Marszałek