---
layout: default
title: decode
parent: base32hex
grand_parent: Codecs
nav_order: 2
permalink: /docs/codecs/base32hex/decode
---

# mgs::base32hex::decode

```cpp
template <typename T, typename It>
static T decode(It& it);                                          (1)

template <typename It>
static std::string decode(It& it);                                (2)

template <typename T, typename I, typename S>
static T decode(I begin, S end);                                  (3)

template <typename I, typename S>
static std::string decode(I begin, S end);                        (4)

template <typename T, typename CharT, typename Traits>
static T decode(std::basic_istream<CharT, Traits>& is);           (5)

template <typename CharT, typename Traits>
static std::string decode(std::basic_istream<CharT, Traits>& is); (6)
```

Decodes the given input.

1. Decodes the contents of `it` and returns the result as a `T`.

    This overload only participates in overload resolution if `It` models [`Iterable`](/docs/concepts/iterable) and calling overload (3) with its iterators is well-formed.
1. Has the same effect as calling `decode<std::string>(it)`.

1. Decodes the contents of the range `[begin, end)` and returns the result as a `T`.

    This overload only participates in overload resolution if `I` models [`InputIterator`](), `S` models [`Sentinel<I>`](), `T` models [`CodecOutput<decltype(T::make_decoder(begin, end))>`](/docs/concepts/codec_output) and `typename std::iterator_traits<I>::value_type` models [`Byte`](/docs/concepts/byte).
1. Same as calling `decode<std::string>(begin, end)`.
1. Decodes the contents of the input stream `is` and returns the result as a `T`.

    Has the same effect as calling `decode<T>(std::istreambuf_iterator<CharT, Traits>(is), std::istreambuf_iterator<CharT, Traits>())`.
1. Has the same effect as calling `decode<std::string>(is)`.

## Exceptions

All overloads may throw an exception derived from `exceptions::decode_error`.

## Notes

Passing a `char[]` to overloads (1) and (2) will discard the last character if it is the null terminator (`'\0'`).

## Example

```cpp
#include <sstream>

#include <mgs/base32hex.hpp>

using namespace mgs;

int main() {
  std::string const encoded("SGVsbG8sIFdvcmxkIQ==");
  std::stringstream ss(encoded);
  std::stringstream ss2(encoded);

  base32hex::decode<std::vector<char>>(encoded);                         // 1.
  base32hex::decode(encoded);                                            // 2.
  base32hex::decode<std::vector<char>>(encoded.begin(), encoded.end());  // 3.
  base32hex::decode(encoded.begin(), encoded.end());                     // 4.
  base32hex::decode<std::vector<char>>(ss);                              // 5.
  base32hex::decode(ss2);                                                // 6.
}
```
