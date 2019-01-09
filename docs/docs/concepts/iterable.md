---
layout: default
title: Iterable
nav_order: 2
parent: Concepts
permalink: /docs/concepts/iterable
---

# mgs::Iterable

Defined in header `<mgs/concepts/iterable.hpp>`

```cpp
template <typename T>
concept Iterable = requires(T& a) {
 { begin(a) } -> std::Iterator;
 { end(a) } -> std::Sentinel<decltype(begin(a))>;
};
```

The `Iterable<T>` concept is satisfied when `T` can be iterated over.

## Notation

* `it` - value of type `T&`
* `I` - value type of `begin(it)`

## Valid expressions

| Expression                    | Return type       |
|-------------------------------+-------------------|
| `using std::begin; begin(it)` | [`Iterator`]()    |
| `using std::end; end(it)`     | [`Sentinel<I>`]() |

## Concept emulation

```cpp
namespace mgs {
namespace concepts {

template <typename T>
struct is_iterable { /* ... */ };
}

template <typename T,
          typename = std::enable_if_t<concepts::is_iterable<T>::value>>
using Iterable = T;
}
```

## Example

```cpp
#include <vector>
#include <sstream>

#include <mgs/concepts/iterable.hpp>

using namespace mgs;

struct stringstream_wrapper {
  std::stringstream ss;
};

auto begin(stringstream_wrapper& sw) {
  return std::istreambuf_iterator<char>(sw);
}

auto end(stringstream_wrapper&) {
  return std::istreambuf_iterator<char>();
}

static_assert(concepts::is_iterable<std::vector<int>>::value, "");
static_assert(concepts::is_iterable<char[10]>::value, "");
static_assert(concepts::is_iterable<stringstream_wrapper>::value, "");

static_assert(!concepts::is_iterable<std::stringstream>::value, "");
```
