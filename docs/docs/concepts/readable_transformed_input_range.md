---
layout: default
title: ReadableTransformedInputRange
nav_order: 4
parent: Concepts
permalink: /docs/concepts/readable_transformed_input_range
---

# mgs::ReadableTransformedInputRange

Defined in header `<mgs/concepts/readable_transformed_input_range.hpp>`

```cpp
template <typename T, typename O>
concept ReadableTransformedInputRange =
  TransformedInputRange<T> &&
  std::OutputIterator<O, std::iter_value_t<T>> &&
  requires(T& v, O o, std::size_t n) {
    { v.read(o, n) } -> std::Same<std::size_t>;
  };
```

The concept `ReadableTransformedInputRange<T, O>` refines [`TransformedInputRange`](/docs/concepts/transformed_input_range) by adding support for block reads, which usually perform better than reading elements one by one.

## Refinements

* [`TransformedInputRange`](/docs/concepts/transformed_input_range)

## Template arguments

| Template argument | Definition                   | Constraints                              |
|-------------------+------------------------------+------------------------------------------|
| `O`               | Iterator type used in `read` | [`std::OutputIterator<std::iter_value_t<T>>`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/OutputIterator) |

## Notation

* `v` - value of type `T&`
* `o` - value of type `O`
* `n` - value of type `std::size_t`

## Valid expressions

In addition to the expressions defined by the refined concepts, the following expressions must be valid.

| Expression     | Return type   |
|----------------+---------------|
| `v.read(o, n)` | `std::size_t` |

## Expression semantics

| Expression     | Precondition                                                          | Semantics                                                                 | Postcondition |
|----------------+-----------------------------------------------------------------------+---------------------------------------------------------------------------+---------------|
| `v.read(o, n)` | Copying `n` elements of type `T::value_type` into `o` is well-defined | Copies up-to `n` elements into `o` and return the number of read elements |               |

## Concept emulation

```cpp
namespace mgs {
namespace concepts {

template <typename T>
struct is_readable_transformed_input_range { /* ... */ };

template <typename T>
constexpr auto is_readable_transformed_input_range_v = is_readable_transformed_input_range<T>::value;
}

template <typename T,
          typename = std::enable_if_t<is_readable_transformed_input_range_v<T>>>
using ReadableTransformedInputRange = T;
}
```

## Example

```cpp
#include <iostream>

#include <mgs/base64.hpp>
#include <mgs/concepts/readable_transformed_input_range.hpp>

using namespace mgs;

int main() {
  std::string s("Hello, World!");

  auto encoder = base64::make_encoder(s.begin(), s.end());

  static_assert(is_readable_transformed_input_range_v<decltype(encoder)>, "");

  auto const nb_read = encoder.read(std::ostreambuf_iterator<char>(std::cout), std::size_t(-1));
  std::cout << "Read " << nb_read << " characters" << std::endl;
}
```
