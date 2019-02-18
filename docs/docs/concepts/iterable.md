---
layout: default
title: Range
nav_order: 2
parent: Concepts
permalink: /docs/meta/concepts/iterator/range
---

# mgs::Range

Defined in header `<mgs/meta/concepts/iterator/range.hpp>`

```cpp
template <typename T>
concept Range = requires(T& a) {
 { begin(a) } -> std::Iterator;
 { end(a) } -> std::Sentinel<decltype(begin(a))>;
};
```

The `Range<T>` concept is satisfied when `T` can be iterated over.

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
struct is_range { /* ... */ };

template <typename T>
constexpr auto is_range_v = is_range<T>::value;
}

template <typename T,
          typename = std::enable_if_t<concepts::is_range_v<T>>>
using Range = T;
}
```

## Example

```cpp
#include <vector>
#include <sstream>

#include <mgs/meta/concepts/iterator/range.hpp>

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

static_assert(concepts::is_range_v<std::vector<int>>, "");
static_assert(concepts::is_range_v<char[10]>, "");
static_assert(concepts::is_range_v<stringstream_wrapper>, "");

static_assert(!concepts::is_range_v<std::stringstream>, "");
```
