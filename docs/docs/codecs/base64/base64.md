---
layout: default
title: Base64
nav_order: 1
parent: Codecs
has_children: true
permalink: /docs/codecs/base64
---

# mgs::base64

Defined in header `<mgs/base64.hpp>`

```cpp
class base64;
```

---

`mgs::base64` is a codec implementing the Base64 encoding scheme, as defined in [RFC4648](https://tools.ietf.org/html/rfc4648).

## Static member functions

|--------------------+-------------------------|
| [`encode`](encode) | encodes input           |
| [`decode`]()       | decodes input           |
| [`make_encoder`]() | constructs an encoder   |
| [`make_decoder`]() | constructs a decoder    |

## Example

```cpp
#include <iostream>

#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  // Create a std::string filled with encoded input
  auto const encoded = base64::encode("Hello, World!");

  std::cout << encoded << std::endl;

  // Create a Decoder to lazily decode input
  auto decoder = base64::make_decoder(encoded.begin(), encoded.end());
  for (auto c : decoder) {
    std::cout << c;
  }
  std::cout << std::endl;
}
```
