---
layout: default
title: decode
parent: base64
grand_parent: Codecs
nav_order: 2
permalink: /docs/codecs/base64/decode
---

# mgs::base64::decode

```cpp
template <typename T = base64::default_decoded_output>
static T decode(char const* s);                         (1)

template <typename T = base64::default_decoded_output,
          typename It>
static T decode(It& it);                                (2)

template <typename T = base64::default_decoded_output,
          typename It>
static T decode(It const& it);                          (3)

template <typename T = base64::default_decoded_output,
          typename I,
          typename S>
static T decode(I begin, S end);                        (4)

```

Decodes the given input.

1. Decodes the content of the null-terminated character string pointed to by `s`.

    The length of the string is determined by the first null character.
    The behavior is undefined if `[s, s + std::strlen(s))` is not a valid range (for example, if `s` is a null pointer).

1. Decodes the content of `it` and returns the result as a `T`.

    This overload only participates in overload resolution if `It` models [`std::Range`]() and calling overload (3) with its iterators is well-formed.
    If `It` is a `char[]`, it has the same effect as calling `decode<T>(static_cast<char const*>(it))`.

1. Same as (2), but allows const lvalues and rvalues to be passed.

1. Decodes the content of the range `[begin, end)` and returns the result as a `T`.

    This overload only participates in overload resolution if `I` models [`std::InputIterator`](), `S` models [`std::Sentinel<I>`](), `T` models [`CodecOutput<base64::decoder<I, S>>`](/docs/concepts/codec_output) and `typename std::iterator_traits<I>::value_type` models [`Byte`](/docs/concepts/byte).

## Exceptions

All overloads may throw an exception derived from `exceptions::decode_error`.

## Example

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  std::string encoded("SGVsbG8sIFdvcmxkIQ==");

  base64::decode(encoded.c_str());                // 1.
  base64::decode(encoded);                        // 2.
  base64::decode(std::move(encoded));             // 3.
  base64::decode(encoded.begin(), encoded.end()); // 4.
}
```
