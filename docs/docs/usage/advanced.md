---
layout: default
title: Advanced
nav_order: 4
parent: Usage
---

# Advanced usage
{: .no_toc }

## Table of contents
{: .no_toc .text_delta }

1. TOC
{: toc }

---

## Supported return types

Here is the list of supported `encode`/`decode` return types:

* `std::array`
* Sequence containers that are [`std::Constructible`](https://en.cppreference.com/w/cpp/concepts/Constructible) from an [`std::InputIterator`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/InputIterator)/[`std::Sentinel`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/Sentinel) pair.

There is an optimization for containers having:

* [`std::RandomAccessIterator`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/RandomAccessIterator)s
* a `size_type` member type
* a `resize(size_type)` member function

Caveat
{: .label .label-yellow }

For `std::array`s, `mgs` will throw an exception if the transformed data does not fit **EXACTLY** in the array.

## Iterator related optimizations

While codecs accepts every [`std::Range`](https://en.cppreference.com/w/cpp/ranges/Range) (assuming it fulfills the codec's constraints), some of them will perform better when the underlying iterators are [`std::RandomAccessIterator`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/RandomAccessIterator)s.

This is the case for [`base64`](/docs/codecs/base64)'s encoder and decoder. Both will define the `max_transformed_size` member function, which can be used to perform a single memory allocation:

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  std::string huge_string = /* ... */;

  // std::string::iterator is a std::RandomAccessIterator, encoder will define max_transformed_size
  auto encoder = base64::make_encoder(huge_string.begin(), huge_string.end());
  auto const max_size = encoder.max_transformed_size();

  // allocate max_size bytes in one go
  std::string encoded_str(max_size, '\0');
  auto const total_written = encoder.read(encoded_str.begin(), max_size);

  // do not forget to resize at the end!
  encoded_str.resize(total_written);
}
```

Note
{: .label .label-blue }

Codecs defining `max_transformed_size` also model the [`SizedTransformedInputRange`](/docs/concepts/sized_transformed_input_range) concept.

## Emulated concepts

As mentioned previously, `mgs` emulates C++20 concepts to properly constrain its APIs.

This subsection will explain how to use `mgs` emulated concepts, if you are looking for a quick introduction first, you can take a look [here](/docs/concepts).

### Using type traits

`mgs` exposes a type trait and a variable template for every concept.

Here is how to constraint a function on [`std::Range`](https://en.cppreference.com/w/cpp/ranges/Range):

```cpp
#include <mgs/meta/concepts/range.hpp>

using namespace mgs::meta;

template <typename T, typename = std::enable_if_t<is_range_v<T>>>
void f(T const&)
{
}
```

### Using type aliases

`mgs` also exposes an alias which bears the concept name.

```cpp
#include <mgs/meta/concepts/range.hpp>

using namespace mgs::meta;

template <typename T, typename = Range<T>>
void f(T const&) {}

// alternative
template <typename T>
void g(Range<T> const&) {}
```

This is quite handy, however there is a caveat with overloads.

Imagine we have two aliases `Integral` and `SignedIntegral`:

```cpp
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
using Integral = T;

template <typename T, typename = std::enable_if_t<std::is_signed<T>::value &&
                                                  std::is_integral<T>::value>>
using SignedIntegral = T;
```

Let's try to use them:

```cpp
template <typename T>
void f(SignedIntegral<T>) {}

template <typename T>
void f(Integral<T>) {} // this fails to compile
```

Since those are just aliases to their first template parameter, the compiler complains that we redefined `f`, even with the `std::enable_if`s in the aliases' template parameters!

Hopefully, there is a way to force overload resolution by setting a priority to each of them:

```cpp
template <std::size_t N> struct priority_tag : priority_tag<N - 1>{};
template <> struct priority_tag<0>{};

// SignedIntegral is more specialized than Integral, hence priority_tag<1>
template <typename T>
void f(SignedIntegral<T>, priority_tag<1>) {}

// Fallback overload
template <typename T>
void f(Integral<T>, priority_tag<0>) {}

int main() {
  priority_tag<1> tag;
  f(2, tag); // calls first overload
  f(2u, tag); // calls second overload
  f("", tag); // fails to compile
}
```

Usually, you would rather have `priority_tag`s in `f_impl` methods, and have a top-level `f` forwarding the call to `f_impl`.

Note
{: .label .label-blue }
This idea of template aliases comes from Arthur O'Dwyer's [blog](https://quuxplusone.github.io/blog/2018/08/23/stop-cascading-errors/).
