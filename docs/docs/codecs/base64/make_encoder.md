---
layout: default
title: make_encoder
parent: base64
grand_parent: Codecs
nav_order: 3
permalink: /docs/codecs/base64/make_encoder
---

# mgs::base64::make_encoder

```cpp
template <typename InputIterator, typename Sentinel>
static encoder<InputIterator, Sentinel> make_encoder(InputIterator begin, Sentinel end);
```

Constructs an `Encoder` from an iterator range.

Calling `make_encoder` is well-formed if:

* `InputIterator` models [`InputIterator`]()
* `Sentinel` models [`Sentinel<InputIterator>`]()
* `encoder<InputIterator, Sentinel>` models [`IterableTransformedInputAdapter`]()

## Example

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  std::string const hello("Hello, World!");

  auto encoder = base64::make_encoder(hello.begin(), hello.end());
}
```
