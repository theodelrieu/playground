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
static base32hex::decoder<I, S> make_decoder(I begin, S end);
```

Constructs a `Decoder` from an iterator-sentinel pair.

Calling `make_decoder` is well-formed if:

* `I` models [`std::InputIterator`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/InputIterator)
* `S` models [`std::Sentinel<I>`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/Sentinel)
* `base32hex::decoder<I, S>` models [`TransformedInputRange`](/docs/concepts/transformed_input_range)

## Example

```cpp
#include <mgs/base32hex.hpp>

using namespace mgs;

int main() {
  std::string const hello("91IMOR3F5GG5ERRIDHI22===");

  auto decoder = base32hex::make_decoder(hello.begin(), hello.end());
}
```
