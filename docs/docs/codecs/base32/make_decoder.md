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

* `I` models [`std::InputIterator`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/InputIterator)
* `S` models [`std::Sentinel<I>`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/Sentinel)
* `base32::decoder<I, S>` models [`TransformedInputRange`](/docs/concepts/transformed_input_range)

## Example

```cpp
#include <mgs/base32.hpp>

using namespace mgs;

int main() {
  std::string const hello("JBSWY3DPFQQFO33SNRSCC===");

  auto decoder = base32::make_decoder(hello.begin(), hello.end());
}
```
