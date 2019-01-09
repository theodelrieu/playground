---
layout: default
title: Byte
nav_order: 1
parent: Concepts
permalink: /docs/concepts/byte
---

# mgs::Byte

Defined in header `<mgs/concepts/byte.hpp>`

```cpp
template <typename T>
concept Byte = std::Same<T, std::byte> ||
  (std::Integral<T> &&
  std::numeric_limits<T>::digits + std::numeric_limits<T>::is_signed ==
  std::numeric_limits<unsigned char>::digits);
```

The concept `Byte<T>` is satisfied if `T` either:

* is [`std::byte`](https://en.cppreference.com/w/cpp/types/byte)
* models [`std::Integral`](https://en.cppreference.com/w/cpp/concepts/Integral) and can represent the same number of bits as `unsigned char`.

## Concept emulation

```cpp
namespace mgs {
namespace concepts {

template <typename T>
struct is_byte { /* ... */ };

template <typename T>
constexpr auto is_byte_v = is_byte<T>::value;
}

template <typename T,
          typename = std::enable_if_t<concepts::is_byte_v<T>>>
using Byte = T;
}
```

## Example

```cpp
#include <cstddef> 
#include <cstdint>

#include <mgs/concepts/byte.hpp>

using namespace mgs;

static_assert(concepts::is_byte_v<std::byte>, "");
static_assert(concepts::is_byte_v<unsigned char>, "");
static_assert(concepts::is_byte_v<char>, "");

static_assert(!concepts::is_byte_v<int>, "");
static_assert(!concepts::is_byte_v<bool>, "");
```
