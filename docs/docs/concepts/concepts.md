---
layout: default
title: Concepts
nav_order: 6
has_children: true
permalink: /docs/concepts
---

# Concepts

This section briefly introduces C++ concepts and lists every concept defined in `mgs`.

## Concept definition syntax

Here is a toy concept definition, which will be useful to read and understand more advanced concepts:

```cpp
template <typename T, typename U>
concept Foo =
  std::DefaultConstructible<T> &&                     // 1.
  std::Integral<U> &&                                 // 2.
  requires(T const& value) {                          // 3.
    typename T::difference_type;                      // 4.
    std::SignedIntegral<typename T::difference_type>; // 5.
    { value.capacity() };                             // 6.
    { value.get() } -> typename T::value_type;        // 7.
    { value.begin() } -> std::Iterator;               // 8.
    { value.size() } -> std::Same<U>;                 // 9.
  };
```

1. `T` must model [`std::DefaultConstructible`](https://en.cppreference.com/w/cpp/concepts/DefaultConstructible)
1. `U` must model [`std::Integral`](https://en.cppreference.com/w/cpp/concepts/Integral)
1. `requires` clause allows using values of any type (here `T const&`) to ensure the validity of expressions composing it
1. `T::difference_type` must be a valid type
1. `T::difference_type` must model [`std::SignedIntegral`](https://en.cppreference.com/w/cpp/concepts/SignedIntegral)
1. `value.capacity()` must be a valid expression
1. `value.get()` must be a valid expression, and return a type **convertible to** `T::value_type`, which must also be a valid type
1. `value.begin()` must be a valid expression, and return a type that models the [`std::Iterator`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/Iterator) concept
1. `value.size()` must be a valid expression, and return **exactly** `U`.

When a concept is specified after the `->`, its first template parameter has to be omitted. It will be set automatically by the compiler.

e.g. the equivalent writing of `9.` is `std::Same<decltype((value.size())), U>`.

Note
{: .label .label-blue }
It is strongly recommended to read the [cppreference concepts page](https://en.cppreference.com/w/cpp/language/constraints) to learn more about this feature.

## Concept emulation

Concepts are a C++20 feature, and thus are not present in the code.

To emulate them, `mgs` defines for each concept:

* a `is_XXX` type trait
* a `is_XXX_v` variable template
* a `XXX` type alias

Taking back the `Foo` toy concept from above:

```cpp
namespace mgs {
namespace concepts {

template <typename T, typename U>
struct is_foo { /* ... */ };

template <typename T, typename U>
constexpr auto is_foo_v = is_foo<T, U>::value;
}

template <typename T, typename U,
          typename = std::enable_if_t<concepts::is_foo_v<T, U>>>
using Foo = T;
}
```

To know more about concept emulation and how to use it well, take a look at the [advanced section](/docs/usage/advanced).
