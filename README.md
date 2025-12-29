# cpp-result

A small, header-only **`Result<T, E>` implementation for C++17 and C++20**, inspired by Rust’s `Result`, designed for **explicit error handling**, **non-throwing control flow**, and **fail-fast debugging**.

This library is intentionally opinionated. It favors correctness, explicitness, and debuggability over convenience.

---

## Motivation

C++ error handling options are unsatisfying in many low-level contexts:

* Exceptions are implicit, hard to reason about, and often disabled in embedded or infrastructure code.
* Error codes are explicit but verbose and non-composable.
* `std::expected` exists, but is limited and ergonomically weak for functional composition.

This library exists because the author wanted:

* Rust-style `Result<T, E>` semantics
* Explicit success/failure in the type system
* Composable APIs (`map`, `and_then`, `or_else`, `match`)
* Early-return propagation (`TRY_OK`)
* Loud, debuggable failures instead of silent bugs

This is not a beginner-friendly abstraction. That is a conscious choice.

---

## Core Requirements (Read This First)

### `T` and `E` **must be debuggable**

Both template parameters **must** support stream output:

```cpp
std::ostream& operator<<(std::ostream&, const T&);
std::ostream& operator<<(std::ostream&, const E&);
```

This is **non-negotiable**.

Why?

* `unwrap()` and `unwrap_err()` intentionally **panic** on misuse
* Panic messages print the contained value or error
* Failures are immediate, loud, and informative

If your types cannot be streamed, this library is not suitable.

### Supported Compilers

This library relies on `GNU statement expressions` to implement the `TRY_OK` macro.

As a result, it is intended to be used with:

- `GCC`

- `Clang` in GNU-compatible mode

Other compilers are not supported.

---

## What This Library Is *Not*

* Not a replacement for exceptions
* Not a wrapper that hides control flow
* Not exception-safe if *you* call throwing APIs

If you want implicit behavior, this is the wrong tool.

---

## Integration

There is **nothing to build and nothing to link**.

Simply copy the `result/` directory into your project and include:

```cpp
#include "result/result.hpp"
```

That’s it.

---

## Directory Layout

```
result/
├── result.hpp                    // main include
├── result-type-definition.hpp    // Result<T, E>
├── result-type-constructor.hpp   // Ok / Err
├── result-type-observers.hpp     // unwrap, is_ok, etc.
├── result-type-monadics.hpp      // map, and_then, or_else, match
├── result-helper.hpp
├── panic.hpp                     // panic + diagnostics
```

---

## Example: Parsing a Versioned Header

This example demonstrates:

* explicit error handling
* non-throwing control flow
* error propagation using `TRY_OK`
* debuggability requirements
* safe and unsafe result consumption

### Setup

```cpp
#include <array>
#include <cstdint>
#include <iostream>
#include <string_view>

#include "result/result.hpp"

using std::array;
using std::string_view;
using namespace std::literals;

using result_type::Err;
using result_type::Ok;
using result_type::Result;

enum class Version : uint8_t
{
    V1 = 1,
    V2 = 2
};

// Required: Result<T, E> assumes both T and E are debuggable
std::ostream& operator<<(std::ostream& os, Version v)
{
    return os << static_cast<int>(v);
}
```

---

### Parsing a Version Field

```cpp
auto parse_version(array<uint8_t, 6> const& header)
    -> Result<Version, string_view>
{
    switch (header.at(0))
    {
        case 1: return Ok(Version::V1);
        case 2: return Ok(Version::V2);
        default: return Err("Unknown version"sv);
    }
}
```

* No exceptions
* No sentinel values
* Failure is explicit in the return type

---

### Propagating Errors with `TRY_OK`

```cpp
auto parse_data(array<uint8_t, 6> const& header)
    -> Result<uint8_t, string_view>
{
    const Version version = TRY_OK(parse_version(header));

    // Arithmetic performs integer promotion, so the result
    // is explicitly cast back to uint8_t.
    return Ok<uint8_t>(
        static_cast<uint8_t>(version) + header[1] + header[2]
    );
}
```

`TRY_OK`:

* unwraps the value on success
* early-returns the error on failure
* works only with r-values
* behaves like Rust’s `?`, not like a control-flow macro

---

### Consuming the Result

```cpp
int main()
{
    parse_version({2, 3, 4, 5, 6, 7})
        .match(
            [](auto version)
            {
                std::cout << "Version: "
                          << static_cast<int>(version) << '\n';
            },
            [](auto error)
            {
                std::cout << error << '\n';
            }
        );

    // Safe here because failure is logically impossible
    const int parsed = parse_data({2, 3, 4, 5, 6, 7}).unwrap();
    std::cout << "parsed: " << parsed << '\n';
}
```

Notes:

* `match()` is explicit and exhaustive
* `unwrap()` is intentionally unsafe if misused
* Misuse results in a panic with diagnostic output

---

## Error Handling Philosophy

This library enforces the following mindset:

* Errors are values
* Control flow is explicit
* Failure paths are visible in the type system
* Incorrect assumptions should crash early and loudly

If you dislike that philosophy, do not use this library.

---

## License

There is no License. use at your own risk.
