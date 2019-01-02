---
layout: default
title: TransformedInputView
nav_order: 2
parent: Concepts
permalink: /docs/concepts/transformed_input_view
---

# mgs::TransformedInputView

Defined in header `<mgs/concepts/transformed_input_view.hpp>`

```cpp
template <typename T>
concept TransformedInputView =
  std::Semiregular<T> &&
  Iterable<T> &&
  requires(T& v) {
    requires std::InputIterator<typename T::underlying_iterator>;
    requires std::Sentinel<typename T::underlying_sentinel, typename T::underlying_iterator>;
    requires std::Constructible<T, typename T::underlying_iterator, typename T::underlying_sentinel>;
  };
```

A `TransformedInputView` generates a sequence of elements by transforming its underlying iterator-sentinel range.

Transformed input can be iterated over.

## Refinements

* [`std::Semiregular`]()
* [`Iterable`]()

## Associated types

| Member type              | Definition                             |
|--------------------------+----------------------------------------|
| `T::underlying_iterator` | [`InputIterator`]()                    |
| `T::underlying_sentinel` | [`Sentinel<T::underlying_iterator>`]() |

## Notation

* `i` - value of type `T::underlying_iterator`
* `s` - value of type `T::underlying_sentinel`

## Valid expressions

In addition to the expressions defined by the refined concepts, the following expressions must be valid.

| Expression     | Return type   |
|----------------+---------------|
| `T(i, s)`      | `T`           |

## Expression semantics

| Expression | Precondition                    | Semantics                               | Postcondition |
|------------+---------------------------------+-----------------------------------------+---------------|
| `T(i, s)`  | `[i, s)` denotes a valid range. | Constructs a view from the input range. |               |

## Example

```cpp
#include <iostream>

#include <mgs/base64.hpp>
#include <mgs/concepts/transformed_input_view.hpp>

using namespace mgs;

int main() {
  std::string s("Hello, World!");

  auto encoder = base64::make_encoder(s.begin(), s.end());

  static_assert(concepts::is_transformed_input_view<decltype(encoder)>::value, "");

  for (auto c : encoder) {
    std::cout << c;
  }
  std::cout << std::endl;
}
```
