---
layout: default
title: make_encoder
parent: base32
grand_parent: Codecs
nav_order: 3
permalink: /docs/codecs/base32/make_encoder
---

# mgs::base32::make_encoder

```cpp
template <typename I, typename S>
static base32::encoder<I, S> make_encoder(I begin, S end);
```

Constructs an `Encoder` from an iterator-sentinel pair.

Calling `make_encoder` is well-formed if:

* `I` models [`std::InputIterator`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/InputIterator)
* `S` models [`std::Sentinel<I>`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/Sentinel)
* `base32::encoder<I, S>` models [`TransformedInputRange`](/docs/concepts/transformed_input_range)

## Example

```cpp
#include <mgs/base32.hpp>

using namespace mgs;

int main() {
  std::string const hello("Hello, World!");

  auto encoder = base32::make_encoder(hello.begin(), hello.end());
}
```
