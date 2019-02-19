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
template <typename T, typename It>
static T encode(It& it);                                          (1)

template <typename It>
static std::string encode(It& it);                                (2)

template <typename T, typename I, typename S>
static T encode(I begin, S end);                                  (3)

template <typename I, typename S>
static std::string encode(I begin, S end);                        (4)

template <typename T, typename CharT, typename Traits>
static T encode(std::basic_istream<CharT, Traits>& is);           (5)

template <typename CharT, typename Traits>
static std::string encode(std::basic_istream<CharT, Traits>& is); (6)
```

Encodes the given input.

1. Encodes the contents of `it` and returns the result as a `T`.

    This overload only participates in overload resolution if `It` models [`Range`](/docs/meta/concepts/range) and calling overload (3) with its iterators is well-formed.
1. Has the same effect as calling `encode<std::string>(it)`.

1. Encodes the contents of the range `[begin, end)` and returns the result as a `T`.

    This overload only participates in overload resolution if `I` models [`InputIterator`](), `S` models [`Sentinel<I>`](), `T` models [`CodecOutput<decltype(T::make_encoder(begin, end))>`](/docs/concepts/codec_output) and `typename std::iterator_traits<I>::value_type` models [`Byte`](/docs/concepts/byte).
1. Same as calling `encode<std::string>(begin, end)`.
1. Encodes the contents of the input stream `is` and returns the result as a `T`.

    Has the same effect as calling `encode<T>(std::istreambuf_iterator<CharT, Traits>(is), std::istreambuf_iterator<CharT, Traits>())`.
1. Has the same effect as calling `encode<std::string>(is)`.

## Notes

Passing a `char[]` to overloads (1) and (2) will discard the last character if it is the null terminator (`'\0'`).

## Example

```cpp
#include <sstream>

#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  std::string const hello("Hello, World!");
  std::stringstream ss(hello);
  std::stringstream ss2(hello);

  base64::encode<std::vector<char>>(hello);                       // 1.
  base64::encode(hello);                                          // 2.
  base64::encode<std::vector<char>>(hello.begin(), hello.end());  // 3.
  base64::encode(hello.begin(), hello.end());                     // 4.
  base64::encode<std::vector<char>>(ss);                          // 5.
  base64::encode(ss2);                                            // 6.
}
```
