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
template <typename I, typename S>
static base64::encoder<I, S> make_encoder(I begin, S end);
```

Constructs an `Encoder` from an iterator-sentinel pair.

Calling `make_encoder` is well-formed if:

* `I` models [`std::InputIterator`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/InputIterator)
* `S` models [`std::Sentinel<I>`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/Sentinel)
* `base64::encoder<I, S>` models [`TransformedInputRange`](/docs/concepts/transformed_input_range)

## Example

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  std::string const hello("Hello, World!");

  auto encoder = base64::make_encoder(hello.begin(), hello.end());
}
```
