---
layout: default
title: base64url
nav_order: 2
parent: Codecs
has_children: true
permalink: /docs/codecs/base64url
---

# mgs::base64url, mgs::base64url_nopad

Defined in header `<mgs/base64url.hpp>`

```cpp
using base64url = codecs::basic_codec</* implementation-defined */>;
using base64url_nopad = codecs::basic_codec</* implementation-defined */>;
```

---

`mgs::base64url` is a codec implementing the base64url encoding scheme, as defined in [RFC4648](https://tools.ietf.org/html/rfc4648).

`mgs::base64url_nopad` only differs in padding policy, and thus shares the same APIs and constraints.

## Characteristics

|-------------------+-----------------------------------------------------------------------------------------|
| Alphabet          | `ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_`                      |
| Padding           | Required for `base64url`, optional when decoding for `base64url_nopad`, 4 byte boundary |
| Padding character | `=`                                                                                     |

## Member types

| Member type              | Definition                   |
|--------------------------+------------------------------|
| `default_encoded_output` | `std::string`                |
| `default_decoded_output` | `std::vector<unsigned char>` |

## Member alias templates

| Template                                              | Definition                                     |
|-------------------------------------------------------|------------------------------------------------|
| `template <typename I, typename S = I> using encoder` | Return type of `base64url::make_encoder(I, S)` |
| `template <typename I, typename S = I> using decoder` | Return type of `base64url::make_decoder(I, S)` |

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

#include <mgs/base64url.hpp>

using namespace mgs;

int main() {
  // Create a std::string filled with encoded input
  auto const encoded = base64url::encode("Hello, World!");
  auto const encoded_nopad = base64url_nopad::encode("Hello, World!");

  std::cout << encoded << std::endl;
  std::cout << encoded_nopad << std::endl;

  // Create a Decoder to lazily decode input
  auto decoder = base64url::make_decoder(encoded.begin(), encoded.end());
  for (auto c : decoder) {
    std::cout << c;
  }
  std::cout << std::endl;
}
```
