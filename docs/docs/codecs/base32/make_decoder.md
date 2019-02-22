---
layout: default
title: make_decoder
parent: base32
grand_parent: Codecs
nav_order: 4
permalink: /docs/codecs/base32/make_decoder
---

# mgs::base32::make_decoder

```cpp
template <typename I, typename S>
static base32::decoder<I, S> make_decoder(I begin, S end);
```

Constructs a `Decoder` from an iterator-sentinel pair.

Calling `make_decoder` is well-formed if:

* `I` models [`std::InputIterator`]()
* `S` models [`std::Sentinel<I>`]()
* `base32::decoder<I, S>` models [`TransformedInputRange`]()

## Example

```cpp
#include <mgs/base32.hpp>

using namespace mgs;

int main() {
  std::string const hello("JBSWY3DPFQQFO33SNRSCC===");

  auto decoder = base32::make_decoder(hello.begin(), hello.end());
}
```
