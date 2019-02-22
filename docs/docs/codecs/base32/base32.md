---
layout: default
title: base32
nav_order: 3
parent: Codecs
has_children: true
permalink: /docs/codecs/base32
---

# mgs::base32

Defined in header `<mgs/base32.hpp>`

```cpp
using base32 = codecs::basic_codec</* implementation-defined */>;
```

---

`mgs::base32` is a codec implementing the base32 encoding scheme, as defined in [RFC4648](https://tools.ietf.org/html/rfc4648).

## Characteristics

|-------------------+------------------------------------|
| Alphabet          | `ABCDEFGHIJKLMNOPQRSTUVWXYZ234567` |
| Padding           | Required, 8 byte boundary          |
| Padding character | `=`                                |

## Member types

| Member type              | Definition                   |
|--------------------------+------------------------------|
| `default_encoded_output` | `std::string`                |
| `default_decoded_output` | `std::vector<unsigned char>` |

## Member alias templates

| Template                                              | Definition                                  |
|-------------------------------------------------------|---------------------------------------------|
| `template <typename I, typename S = I> using encoder` | Return type of `base32::make_encoder(I, S)` |
| `template <typename I, typename S = I> using decoder` | Return type of `base32::make_decoder(I, S)` |

## Static member functions

|----------------------------------------+---------------------------------|
| [`encode`](encode)                     | encodes input                   |
| [`decode`](decode)                     | decodes input                   |
| [`make_encoder`](make_encoder)         | constructs an encoder           |
| [`make_decoder`](make_decoder)         | constructs a decoder            |
| [`encoded_size`](encoded_size)         | calculates encoded size         |
| [`max_decoded_size`](max_decoded_size) | calculates maximum decoded size |

## Example

```cpp
#include <iostream>

#include <mgs/base32.hpp>

using namespace mgs;

int main() {
  // Create a std::string filled with encoded input
  auto const encoded = base32::encode("Hello, World!");

  std::cout << encoded << std::endl;

  // Create a Decoder to lazily decode input
  auto decoder = base32::make_decoder(encoded.begin(), encoded.end());
  for (auto c : decoder) {
    std::cout << c;
  }
  std::cout << std::endl;
}
```
