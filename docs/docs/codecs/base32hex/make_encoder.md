---
layout: default
title: make_encoder
parent: base32hex
grand_parent: Codecs
nav_order: 3
permalink: /docs/codecs/base32hex/make_encoder
---

# mgs::base32hex::make_encoder

```cpp
template <typename I, typename S>
static encoder<I, S> make_encoder(I begin, S end);
```

Constructs an `Encoder` from an iterator range.

Calling `make_encoder` is well-formed if:

* `I` models [`InputIterator`]()
* `S` models [`Sentinel<I>`]()
* `encoder<I, S>` models [`TransformedInputRange`]()

## Example

```cpp
#include <mgs/base32hex.hpp>

using namespace mgs;

int main() {
  std::string const hello("Hello, World!");

  auto encoder = base32hex::make_encoder(hello.begin(), hello.end());
}
```
