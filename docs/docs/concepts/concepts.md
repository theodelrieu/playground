---
layout: default
title: Concepts
nav_order: 7
has_children: true
permalink: /docs/concepts
---

# Concepts

This section briefly introduces C++ concepts and documents every concept defined in `mgs`.

## Usage

Concepts help constraining generic code, let's imagine we have the following code:

```cpp
template <typename T>
auto add_numbers(T lhs, T rhs) {
  return lhs + rhs;
}
```

It will work with numbers but nothing prevents passing `std::string`s, which does not seem right for an `add_numbers` function. 

Let's use concepts to constrain `T` to be an integral type:

```cpp
template <std::Integral T>
auto add_numbers(T lhs, T rhs) {
  return lhs + rhs;
}
```

Calling `add_numbers` with `std::string`s will now fail to compile.

## Concept definition syntax

Here is a toy concept definition, which will be useful to read and understand more advanced concepts:

```cpp
template <typename T>
concept Foo =
  std::DefaultConstructible<T> &&                     // 1.
  requires(T const& value) {                          // 2.
    std::SignedIntegral<typename T::difference_type>; // 3.
    { value.get() } -> typename T::value_type;        // 4.
    { value.size() } -> std::Same<std::size_t>;       // 5.
  };
```

1. `T` must model [`std::DefaultConstructible`](https://en.cppreference.com/w/cpp/concepts/DefaultConstructible)
1. `requires` clause allows using values of any type (here `T const&`) to ensure the validity of expressions composing it
1. `T::difference_type` must model [`std::SignedIntegral`](https://en.cppreference.com/w/cpp/concepts/SignedIntegral)
1. `value.get()` must be a valid expression, and return a type **convertible to** `T::value_type`, which must be a valid type
1. `value.size()` must be a valid expression, and return **exactly** `std::size_t`.

When a concept is specified after the `->`, its first template parameter has to be omitted. It will be set automatically by the compiler.

e.g. the equivalent writing of `5.` is `std::Same<decltype((value.size())), U>`.

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

To know more about concept emulation and how to use it, take a look at the [advanced section](/docs/usage/advanced).
