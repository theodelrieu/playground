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
template <typename T, typename Iterable>
static T encode(Iterable& it);                                 (1)

template <typename Iterable>
static std::string encode(Iterable& it);                       (2)

template <typename T,
          typename InputIterator,
          typename Sentinel>
static T encode(InputIterator begin, Sentinel end);            (3)

template <typename InputIterator,
          typename Sentinel>
static std::string encode(InputIterator begin, Sentinel end);  (4)

template <typename T>
static T encode(std::istream& is);                             (5)

static std::string encode(std::istream& is);                   (6)
```

Encodes the given input.

1. Encodes the contents of `it` and returns the result as a `T`.

    This overload only participates in overload resolution if `Iterable` models [`Iterable`]() and calling overload (3) with its iterators is well-formed.
2. Same as calling `encode<std::string>(it)`.

3. Encodes the contents of the range `[begin, end)` and returns the result as a `T`.

    This overload only participates in overload resolution if `begin` models [`InputIterator`](), `end` models [`Sentinel<InputIterator>`](), `T` models [`CodecOutput`]() and `typename std::iterator_traits<InputIterator>::value_type` models [`ByteIntegral`]().
4. Same as calling `encode<std::string>(begin, end)`.
5. Encodes the contents of the input stream `is` and returns the result as a `T`.

    This overload only participates in overload resolution if `T` models [`CodecOutput`]().
6. Same as calling `encode<std::string>(is)`.

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
