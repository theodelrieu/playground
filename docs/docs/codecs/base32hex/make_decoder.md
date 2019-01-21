---
layout: default
title: make_decoder
parent: base32hex
grand_parent: Codecs
nav_order: 4
permalink: /docs/codecs/base32hex/make_decoder
---

# mgs::base32hex::make_decoder

```cpp
template <typename I, typename S>
static decoder<I, S> make_decoder(I begin, S end);
```

Constructs an `Decoder` from an iterator range.

Calling `make_decoder` is well-formed if:

* `I` models [`InputIterator`]()
* `S` models [`Sentinel<I>`]()
* `decoder<I, S>` models [`TransformedInputRange`]()

## Example

```cpp
#include <mgs/base32hex.hpp>

using namespace mgs;

int main() {
  std::string const hello("SGVsbG8sIFdvcmxkIQ==");

  auto decoder = base32hex::make_decoder(hello.begin(), hello.end());
}
```
