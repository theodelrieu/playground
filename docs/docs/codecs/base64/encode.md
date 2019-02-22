---
layout: default
title: encode
parent: base64
grand_parent: Codecs
nav_order: 1
permalink: /docs/codecs/base64/encode
---

# mgs::base64::encode

```cpp
template <typename T = base64::default_encoded_output>
static T encode(char const* s);                         (1)

template <typename T = base64::default_encoded_output,
          typename It>
static T encode(It& it);                                (2)

template <typename T = base64::default_encoded_output,
          typename It>
static T encode(It const& it);                          (3)

template <typename T = base64::default_encoded_output,
          typename I,
          typename S>
static T encode(I begin, S end);                        (4)

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

    This overload only participates in overload resolution if `I` models [`std::InputIterator`](), `S` models [`std::Sentinel<I>`](), `T` models [`CodecOutput<base64::encoder<I, S>>`](/docs/concepts/codec_output) and `typename std::iterator_traits<I>::value_type` models [`Byte`](/docs/concepts/byte).

## Example

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  std::string decoded("Hello, World!");

  base64::encode(decoded.c_str());                // 1.
  base64::encode(decoded);                        // 2.
  base64::encode(std::move(decoded));             // 3.
  base64::encode(decoded.begin(), decoded.end()); // 4.
}
```
