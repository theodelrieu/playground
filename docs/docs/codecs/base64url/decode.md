---
layout: default
title: decode
parent: base64url
grand_parent: Codecs
nav_order: 2
permalink: /docs/codecs/base64url/decode
---

# mgs::base64url::decode

```cpp
template <typename T = base64url::default_decoded_output>
static T decode(char const* s);                           (1)

template <typename T = base64url::default_decoded_output,
          typename It>
static T decode(It& it);                                  (2)

template <typename T = base64url::default_decoded_output,
          typename It>
static T decode(It const& it);                            (3)

template <typename T = base64url::default_decoded_output,
          typename I,
          typename S>
static T decode(I begin, S end);                          (4)

```

Decodes the given input.

1. Decodes the content of the null-terminated character string pointed to by `s`.

    The length of the string is determined by the first null character.
    The behavior is undefined if `[s, s + std::strlen(s))` is not a valid range (for example, if `s` is a null pointer).

1. Decodes the content of `it` and returns the result as a `T`.

    This overload only participates in overload resolution if `It` models [`std::Range`](https://en.cppreference.com/w/cpp/ranges/Range) and calling overload (3) with its iterators is well-formed.
    If `It` is a `char[]`, it has the same effect as calling `decode<T>(static_cast<char const*>(it))`.

1. Same as (2), but allows const lvalues and rvalues to be passed.

1. Decodes the content of the range `[begin, end)` and returns the result as a `T`.

    This overload only participates in overload resolution if `I` models [`std::InputIterator`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/InputIterator), `S` models [`std::Sentinel<I>`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/Sentinel), `T` models [`CodecOutput<base64url::decoder<I, S>>`](/docs/concepts/codec_output) and [`iter_value_t<I>`](https://en.cppreference.com/w/cpp/iterator/iter_t) models [`Byte`](/docs/concepts/byte).

## Example

```cpp
#include <mgs/base64url.hpp>

using namespace mgs;

int main() {
  std::string encoded("SGVsbG8sIFdvcmxkIQ==");

  base64url::decode(encoded.c_str());                // 1.
  base64url::decode(encoded);                        // 2.
  base64url::decode(std::move(encoded));             // 3.
  base64url::decode(encoded.begin(), encoded.end()); // 4.
}
```
