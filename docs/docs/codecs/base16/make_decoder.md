---
layout: default
title: make_decoder
parent: base16
grand_parent: Codecs
nav_order: 4
permalink: /docs/codecs/base16/make_decoder
---

# mgs::base16::make_decoder

```cpp
template <typename I, typename S>
static base16::decoder<I, S> make_decoder(I begin, S end);
```

Constructs a `Decoder` from an iterator-sentinel pair.

Calling `make_decoder` is well-formed if:

* `I` models [`std::InputIterator`]()
* `S` models [`std::Sentinel<I>`]()
* `base16::decoder<I, S>` models [`TransformedInputRange`]()

## Example

```cpp
#include <mgs/base16.hpp>

using namespace mgs;

int main() {
  std::string const hello("666F6F626172");

  auto decoder = base16::make_decoder(hello.begin(), hello.end());
}
```
