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
template <typename T, typename Iterable>
static T decode(Iterable& it);                                 (1)

template <typename Iterable>
static std::string decode(Iterable& it);                       (2)

template <typename T,
          typename InputIterator,
          typename Sentinel>
static T decode(InputIterator begin, Sentinel end);            (3)

template <typename InputIterator,
          typename Sentinel>
static std::string decode(InputIterator begin, Sentinel end);  (4)

template <typename T>
static T decode(std::istream& is);                             (5)

static std::string decode(std::istream& is);                   (6)
```

Decodes the given input.

1. Decodes the contents of `it` and returns the result as a `T`.

    This overload only participates in overload resolution if `Iterable` models [`Iterable`]() and calling overload (3) with its iterators is well-formed.
2. Same as calling `decode<std::string>(it)`.

3. Decodes the contents of the range `[begin, end)` and returns the result as a `T`.

    This overload only participates in overload resolution if `begin` models [`InputIterator`](), `end` models [`Sentinel<InputIterator>`](), `T` models [`CodecOutput`]() and `typename std::iterator_traits<InputIterator>::value_type` models [`ByteIntegral`]().
4. Same as calling `decode<std::string>(begin, end)`.
5. Decodes the contents of the input stream `is` and returns the result as a `T`.

    This overload only participates in overload resolution if `T` models [`CodecOutput`]().
6. Same as calling `decode<std::string>(is)`.

## Notes

Passing a `char[]` to overloads (1) and (2) will discard the last character if it is the null terminator (`'\0'`).

## Example

```cpp
#include <sstream>

#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  std::string const encoded("SGVsbG8sIFdvcmxkIQ==");
  std::stringstream ss(encoded);
  std::stringstream ss2(encoded);

  base64::decode<std::vector<char>>(encoded);                         // 1.
  base64::decode(encoded);                                            // 2.
  base64::decode<std::vector<char>>(encoded.begin(), encoded.end());  // 3.
  base64::decode(encoded.begin(), encoded.end());                     // 4.
  base64::decode<std::vector<char>>(ss);                              // 5.
  base64::decode(ss2);                                                // 6.
}
```
