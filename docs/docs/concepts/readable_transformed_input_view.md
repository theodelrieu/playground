---
layout: default
title: ReadableTransformedInputView
nav_order: 4
parent: Concepts
permalink: /docs/concepts/readable_transformed_input_view
---

# mgs::ReadableTransformedInputView

Defined in header `<mgs/concepts/readable_transformed_input_view.hpp>`

```cpp
template <typename T, typename O>
concept ReadableTransformedInputView =
  TransformedInputView<T> &&
  std::OutputIterator<O, typename T::value_type> &&
  requires(T& v, O o, std::size_t n) {
    { v.read(o, n) } -> std::Same<std::size_t>;
  };
```

The concept `ReadableTransformedInputView<T, O>` refines [`TransformedInputView`]() by adding support for block reads, which usually perform better than reading elements one by one.

## Refinements

* [`TransformedInputView`]()

## Associated types

| Member type     | Definition             |
|-----------------+------------------------|
| `T::value_type` | Transformed input type |

## Template arguments

| Template argument | Definition                   | Constraints                              |
|-------------------+------------------------------+------------------------------------------|
| `O`               | Iterator type used in `read` | [`std::OutputIterator<T::value_type>`]() |

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
struct is_readable_transformed_input_view { /* ... */ };

template <typename T>
constexpr auto is_readable_transformed_input_view_v = is_readable_transformed_input_view<T>::value;
}

template <typename T,
          typename = std::enable_if_t<concepts::is_readable_transformed_input_view_v<T>>>
using ReadableTransformedInputView = T;
}
```

## Example

```cpp
#include <iostream>

#include <mgs/base64.hpp>
#include <mgs/concepts/readable_transformed_input_view.hpp>

using namespace mgs;

int main() {
  std::string s("Hello, World!");

  auto encoder = base64::make_encoder(s.begin(), s.end());

  static_assert(concepts::is_readable_transformed_input_view_v<decltype(encoder)>, "");

  auto const nb_read = encoder.read(std::ostreambuf_iterator<char>(std::cout), std::size_t(-1));
  std::cout << "Read " << nb_read << " characters" << std::endl;
}
```
