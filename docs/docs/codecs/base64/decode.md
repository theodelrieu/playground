---
layout: default
title: decode
parent: Base64
grand_parent: Codecs
nav_order: 2
permalink: /docs/codecs/base64/decode
---

# mgs::base64::decode

```cpp
template <typename T, typename Iterable>
T decode(Iterable& it);                                 (1)

template <typename Iterable>
std::string decode(Iterable& it);                       (2)

template <typename T,
          typename InputIterator,
          typename Sentinel>
T decode(InputIterator begin, Sentinel end);            (3)

template <typename InputIterator,
          typename Sentinel>
std::string decode(InputIterator begin, Sentinel end);  (4)

template <typename T>
T decode(std::istream& is);                             (5)

std::string decode(std::istream& is);                   (6)
```

decodes the given input.

1. decodes the contents of `it` and returns the result as a `T`.

    This overload only participates in overload resolution if `Iterable` models [`Iterable`]() and calling overload (3) with its iterators is well-formed.
2. Same effect as calling `decode<std::string>(it)`.

3. decodes the contents of the range `[begin, end)` and returns the result as a `T`.

    This overload only participates in overload resolution if `begin` models [`InputIterator`](), `end` models [`Sentinel<InputIterator>`](), `T` models [`CodecOutput`]() and `typename std::iterator_traits<InputIterator>::value_type` models [`ByteIntegral`]().
4. Same effect as calling `decode<std::string>(begin, end)`.
5. decodes the contents of the input stream `is` and returns the result as a `T`.

    This overload only participates in overload resolution if `T` models [`CodecOutput`]().
6. Same effect as calling `decode<std::string>(is)`.

## Notes

Passing a `char[]` to overloads (1) and (2) will discard any input from the first decountered null terminator (`'\0'`).

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
