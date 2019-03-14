---
layout: default
title: encode
parent: base16
grand_parent: Codecs
nav_order: 1
permalink: /docs/codecs/base16/encode
---

# mgs::base16::encode

```cpp
template <typename T = base16::default_encoded_output>
static T encode(char const* s);                         (1)

template <typename T = base16::default_encoded_output,
          typename It>
static T encode(It& it);                                (2)

template <typename T = base16::default_encoded_output,
          typename It>
static T encode(It const& it);                          (3)

template <typename T = base16::default_encoded_output,
          typename I,
          typename S>
static T encode(I begin, S end);                        (4)

```

Encodes the given input.

1. Encodes the content of the null-terminated character string pointed to by `s`.

    The length of the string is determined by the first null character.
    The behavior is undefined if `[s, s + std::strlen(s))` is not a valid range (for example, if `s` is a null pointer).

1. Encodes the content of `it` and returns the result as a `T`.

    This overload only participates in overload resolution if `It` models [`std::Range`](https://en.cppreference.com/w/cpp/ranges/Range) and calling overload (3) with its iterators is well-formed.
    If `It` is a `char[]`, it has the same effect as calling `encode<T>(static_cast<char const*>(it))`.

1. Same as (2), but allows const lvalues and rvalues to be passed.

1. Encodes the content of the range `[begin, end)` and returns the result as a `T`.

    This overload only participates in overload resolution if `I` models [`std::InputIterator`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/InputIterator), `S` models [`std::Sentinel<I>`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/Sentinel), `T` models [`CodecOutput<base16::encoder<I, S>>`](/docs/concepts/codec_output) and [`iter_value_t<I>`](https://en.cppreference.com/w/cpp/iterator/iter_t) models [`Byte`](/docs/concepts/byte).

## Example

```cpp
#include <mgs/base16.hpp>

using namespace mgs;

int main() {
  std::string decoded("Hello, World!");

  base16::encode(decoded.c_str());                // 1.
  base16::encode(decoded);                        // 2.
  base16::encode(std::move(decoded));             // 3.
  base16::encode(decoded.begin(), decoded.end()); // 4.
}
```
