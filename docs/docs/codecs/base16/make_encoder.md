---
layout: default
title: make_encoder
parent: base16
grand_parent: Codecs
nav_order: 3
permalink: /docs/codecs/base16/make_encoder
---

# mgs::base16::make_encoder

```cpp
template <typename I, typename S>
static base16::encoder<I, S> make_encoder(I begin, S end);
```

Constructs an `Encoder` from an iterator-sentinel pair.

Calling `make_encoder` is well-formed if:

* `I` models [`std::InputIterator`]()
* `S` models [`std::Sentinel<I>`]()
* `base16::encoder<I, S>` models [`TransformedInputRange`]()

## Example

```cpp
#include <mgs/base16.hpp>

using namespace mgs;

int main() {
  std::string const hello("Hello, World!");

  auto encoder = base16::make_encoder(hello.begin(), hello.end());
}
```
