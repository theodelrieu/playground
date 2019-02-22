---
layout: default
title: encode
parent: base32hex
grand_parent: Codecs
nav_order: 1
permalink: /docs/codecs/base32hex/encode
---

# mgs::base32hex::encode

```cpp
template <typename T = base32hex::default_encoded_output>
static T encode(char const* s);                           (1)

template <typename T = base32hex::default_encoded_output,
          typename It>
static T encode(It& it);                                  (2)

template <typename T = base32hex::default_encoded_output,
          typename It>
static T encode(It const& it);                            (3)

template <typename T = base32hex::default_encoded_output,
          typename I,
          typename S>
static T encode(I begin, S end);                          (4)

```

Encodes the given input.

1. Encodes the content of the null-terminated character string pointed to by `s`.

    The length of the string is determined by the first null character.
    The behavior is undefined if `[s, s + std::strlen(s))` is not a valid range (for example, if `s` is a null pointer).

1. Encodes the content of `it` and returns the result as a `T`.

    This overload only participates in overload resolution if `It` models [`std::Range`]() and calling overload (3) with its iterators is well-formed.
    If `It` is a `char[]`, it has the same effect as calling `encode<T>(static_cast<char const*>(it))`.

1. Same as (2), but allows const lvalues and rvalues to be passed.

1. Encodes the content of the range `[begin, end)` and returns the result as a `T`.

    This overload only participates in overload resolution if `I` models [`std::InputIterator`](), `S` models [`std::Sentinel<I>`](), `T` models [`CodecOutput<base32hex::encoder<I, S>>`](/docs/concepts/codec_output) and `typename std::iterator_traits<I>::value_type` models [`Byte`](/docs/concepts/byte).


## Example

```cpp
#include <mgs/base32hex.hpp>

using namespace mgs;

int main() {
  std::string decoded("Hello, World!");

  base32hex::encode(decoded.c_str());                // 1.
  base32hex::encode(decoded);                        // 2.
  base32hex::encode(std::move(decoded));             // 3.
  base32hex::encode(decoded.begin(), decoded.end()); // 4.
}
```
