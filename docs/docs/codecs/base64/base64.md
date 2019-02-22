---
layout: default
title: base64
nav_order: 1
parent: Codecs
has_children: true
permalink: /docs/codecs/base64
---

# mgs::base64

Defined in header `<mgs/base64.hpp>`

```cpp
using base64 = codecs::basic_codec</* implementation-defined */>;
```

---

`mgs::base64` is a codec implementing the base64 encoding scheme, as defined in [RFC4648](https://tools.ietf.org/html/rfc4648).

## Characteristics

|-------------------+--------------------------------------------------------------------|
| Alphabet          | `ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/` |
| Padding           | Required, 4 byte boundary                                          |
| Padding character | `=`                                                                |

## Member types

| Member type              | Definition                   |
|--------------------------+------------------------------|
| `default_encoded_output` | `std::string`                |
| `default_decoded_output` | `std::vector<unsigned char>` |

## Member alias templates

| Template                                              | Definition                                  |
|-------------------------------------------------------|---------------------------------------------|
| `template <typename I, typename S = I> using encoder` | Return type of `base64::make_encoder(I, S)` |
| `template <typename I, typename S = I> using decoder` | Return type of `base64::make_decoder(I, S)` |

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
