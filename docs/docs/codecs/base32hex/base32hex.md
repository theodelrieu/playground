---
layout: default
title: base32hex
nav_order: 4
parent: Codecs
has_children: true
permalink: /docs/codecs/base32hex
---

# mgs::base32hex

Defined in header `<mgs/base32hex.hpp>`

```cpp
class base32hex;
```

---

`mgs::base32hex` is a codec implementing the base32hex encoding scheme, as defined in [RFC4648](https://tools.ietf.org/html/rfc4648).

## Characteristics

|-------------------+------------------------------------|
| Alphabet          | `0123456789ABCDEFGHIJKLMNOPQRSTUV` |
| Padding           | Required, 8 byte boundary          |
| Padding character | `=`                                |

## Member types

| Member type              | Definition                   |
|--------------------------+------------------------------|
| `default_encoded_output` | `std::string`                |
| `default_decoded_output` | `std::vector<unsigned char>` |

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

#include <mgs/base32hex.hpp>

using namespace mgs;

int main() {
  // Create a std::string filled with encoded input
  auto const encoded = base32hex::encode("Hello, World!");

  std::cout << encoded << std::endl;

  // Create a Decoder to lazily decode input
  auto decoder = base32hex::make_decoder(encoded.begin(), encoded.end());
  for (auto c : decoder) {
    std::cout << c;
  }
  std::cout << std::endl;
}
```
