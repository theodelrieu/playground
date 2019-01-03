---
layout: default
title: SizedInputTransformer
nav_order: 6
parent: Concepts
permalink: /docs/concepts/sized_input_transformer
---

# mgs::SizedInputTransformer

Defined in header `<mgs/concepts/sized_input_transformer.hpp>`

```cpp
template <typename T>
concept SizedInputTransformer =
  InputTransformer<T> &&
  requires(T const& cv) {
    { cv.max_transformed_size() } -> std::size_t;
  };
```

The concept `SizedInputTransformer<T>` refines [`InputTransformer`]() by adding maximum transformed size calculation.

## Refinements

* [`InputTransformer`]()

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
#include <mgs/concepts/sized_input_transformer.hpp>

using namespace mgs;

class noop_transformer {
// omitting the implementation shown in the InputTransformer example 

public:
  std::size_t max_transformed_size() const {
    return std::distance(_current, _end);
  }
};

static_assert(concepts::is_sized_input_transformer<noop_transformer>::value, "");
```