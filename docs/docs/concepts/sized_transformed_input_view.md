---
layout: default
title: SizedTransformedInputView
nav_order: 7
parent: Concepts
permalink: /docs/concepts/sized_transformed_input_view
---

# mgs::SizedTransformedInputView

Defined in header `<mgs/concepts/sized_transformed_input_view.hpp>`

```cpp
template <typename T>
concept SizedTransformedInputView =
  TransformedInputView<T> &&
  requires(T const& cv) {
    { cv.max_transformed_size() } -> std::Same<std::size_t>;
  };
```

The concept `SizedTransformedInputView<T>` refines [`TransformedInputView`]() by adding maximum transformed size calculation.

## Refinements

* [`TransformedInputView`]()

## Notation

* `cv` - value of type `T const&`

## Valid expressions

In addition to the expressions defined by the refined concepts, the following expressions must be valid.

| Expression                  | Return type   |
|-----------------------------+---------------|
| `cv.max_transformed_size()` | `std::size_t` |

## Expression semantics

| Expression                  | Precondition | Semantics                                                       | Postcondition |
|-----------------------------+--------------+-----------------------------------------------------------------+---------------|
| `cv.max_transformed_size()` |              | Returns the maximum transformed size of the current input range |               |

## Example

```cpp
#include <mgs/concepts/sized_transformed_input_view.hpp>
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  std::string s("Hello, World!");

  auto encoder = base64::make_encoder(s.begin(), s.end());
  static_assert(concepts::is_sized_transformed_input_view<decltype(encoder)>::value, "");

  auto const max_size = encoder.max_transformed_size();
}
```
