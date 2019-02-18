---
layout: default
title: TransformedInputRange
nav_order: 3
parent: Concepts
permalink: /docs/concepts/transformed_input_range
---

# mgs::TransformedInputRange

Defined in header `<mgs/concepts/transformed_input_range.hpp>`

```cpp
template <typename T>
concept TransformedInputRange =
  std::Semiregular<T> &&
  Range<T> &&
  requires {
    std::InputIterator<typename T::underlying_iterator>;
    std::Sentinel<typename T::underlying_sentinel, typename T::underlying_iterator>;
    std::Constructible<T, typename T::underlying_iterator, typename T::underlying_sentinel>;
  };
```

A `TransformedInputRange` generates a sequence of elements by transforming its underlying iterator-sentinel range.

Transformed input can be retrieved by iterating over the range.

## Model of

* [`std::Semiregular`]()
* [`Range`]()

## Associated types

| Member type              | Definition                                  |
|--------------------------+---------------------------------------------|
| `T::underlying_iterator` | [`std::InputIterator`]()                    |
| `T::underlying_sentinel` | [`std::Sentinel<T::underlying_iterator>`]() |

## Notation

* `i` - value of type `T::underlying_iterator`
* `s` - value of type `T::underlying_sentinel`

## Valid expressions

In addition to the expressions defined by the modeled concepts, the following expressions must be valid.

| Expression     | Return type   |
|----------------+---------------|
| `T(i, s)`      | `T`           |

## Expression semantics

| Expression | Precondition                          | Semantics                               | Postcondition |
|------------+---------------------------------------+-----------------------------------------+---------------|
| `T(i, s)`  | `[i, s)` denotes a valid input range. | Constructs a range from the input range. |               |

## Concept emulation

```cpp
namespace mgs {
namespace concepts {

template <typename T>
struct is_transformed_input_range { /* ... */ };

template <typename T>
constexpr auto is_transformed_input_range_v = is_transformed_input_range<T>::value;
}

template <typename T,
          typename = std::enable_if_t<concepts::is_transformed_input_range_v<T>>>
using TransformedInputRange = T;
}
```

## Example

```cpp
#include <iostream>

#include <mgs/base64.hpp>
#include <mgs/concepts/transformed_input_range.hpp>

using namespace mgs;

int main() {
  std::string s("Hello, World!");

  auto encoder = base64::make_encoder(s.begin(), s.end());

  static_assert(concepts::is_transformed_input_range_v<decltype(encoder)>, "");

  for (auto c : encoder) {
    std::cout << c;
  }
  std::cout << std::endl;
}
```
