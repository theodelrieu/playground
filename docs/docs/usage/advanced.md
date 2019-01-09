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

* `std::array<T, std::size_t>`
* Sequence containers that are [`std::CopyConstructible`]() or [`std::MoveConstructible`]()
  * And either
    * [`std::Constructible`]() from an [`std::Iterator`]() range
  * Or
    * [`std::DefaultConstructible`]()
    * [`Iterable`]()
    * Have a [`std::RandomAccessIterator`]() associated iterator type 
    * Have a [`std::SizedSentinel`]() associated sentinel type 
    * Have a `size_type` member type
    * Have a `resize(size_type)` member function

Caveat
{: .label .label-yellow }

For `std::array`s, `mgs` will throw an exception if the transformed data does not fit **EXACTLY** in the array.

## Iterator related optimizations

While codecs accepts every [`Iterable`]() (assuming it fulfills the constraints properly), some of them will perform better when the underlying iterators are [`std::RandomAccessIterator`]()s.

This is the case for [`base64`]()'s encoder and decoder. Both will define the `max_transformed_size` member function, which can be used to perform a single memory allocation:

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

Codecs defining `max_transformed_size` also model the [`SizedTransformedInputAdapter`]() concept.

## Emulated concepts

As mentioned previously, `mgs` emulates C++20 concepts to properly constrain its APIs.

This subsection will explain how to use `mgs` emulated concepts, if you are looking for a quick introduction first, you can take a look [here](/docs/concepts).

### Using type traits

`mgs` exposes a type trait and a variable template for every concept.

Here is how to constraint a function on [`Iterable`](/docs/concepts/iterable):

```cpp
#include <mgs/concepts/iterable.hpp>

using namespace mgs;

template <typename T, typename = std::enable_if_t<concepts::is_iterable_v<T>>>
void f(T const&)
{
}
```

### Using type aliases

`mgs` also exposes an alias which bears the concept name.

```cpp
#include <mgs/concepts/iterable.hpp>

using namespace mgs;

template <typename T, typename = Iterable<T>>
void f(T const&) {}

// alternative
template <typename T>
void g(Iterable<T> const&) {}
```

This is very handy, however there is a caveat. The latter version cannot be used reliably with overloads.

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
void f(Integral<T>) {}

template <typename T>
void f(SignedIntegral<T>) {} // this fails to compile
```

Since those are just aliases to their first template parameter, the compiler complains that we redefined `f`, even with the `std::enable_if`s in the aliases!

Long story short, do not use those aliases to constrain overload sets.

Note
{: .label .label-blue }
This idea of template aliases comes from Arthur O'Dwyer's [blog](https://quuxplusone.github.io/blog/2018/08/23/stop-cascading-errors/).
