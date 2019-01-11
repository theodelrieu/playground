---
layout: default
title: base16
nav_order: 5
parent: Codecs
has_children: true
permalink: /docs/codecs/base16
---

# mgs::base16

Defined in header `<mgs/base16.hpp>`

```cpp
class base16;
```

---

`mgs::base16` is a codec implementing the base16 encoding scheme, as defined in [RFC4648](https://tools.ietf.org/html/rfc4648).

## Characteristics

|----------+---------------------------------------------------------|
| Alphabet | `0123456789ABCDEF`                                      |
| Padding  | None                                                    |
| Case     | Accepts both lower and upper case letters when decoding |

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

#include <mgs/base16.hpp>

using namespace mgs;

int main() {
  auto const decoded = base16::decode("666F6F626172");
  auto const decoded_lower = base16::decode("666f6f626172");
}
```
