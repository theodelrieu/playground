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
    { cv.max_transformed_size() } -> std::Same<std::size_t>;
  };
```

The concept `SizedInputTransformer<T>` refines [`InputTransformer`](/docs/concepts/input_transformer) by adding maximum transformed size calculation.

## Refinements

* [`InputTransformer`](/docs/concepts/input_transformer)

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

## Concept emulation

```cpp
namespace mgs {
namespace concepts {

template <typename T>
struct is_sized_input_transformer { /* ... */ };

template <typename T>
constexpr auto is_sized_input_transformer_v = is_sized_input_transformer<T>::value;
}

template <typename T,
          typename = std::enable_if_t<concepts::is_sized_input_transformer_v<T>>>
using SizedInputTransformer = T;
}
}
```

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

static_assert(concepts::is_sized_input_transformer_v<noop_transformer>, "");
```
