---
layout: default
title: make_decoder
parent: base64
grand_parent: Codecs
nav_order: 4
permalink: /docs/codecs/base64/make_decoder
---

# mgs::base64::make_decoder

```cpp
template <typename I, typename S>
static base64::decoder<I, S> make_decoder(I begin, S end);
```

Constructs a `Decoder` from an iterator-sentinel pair.

Calling `make_decoder` is well-formed if:

* `I` models [`std::InputIterator`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/InputIterator)
* `S` models [`std::Sentinel<I>`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/Sentinel)
* `base64::decoder<I, S>` models [`TransformedInputRange`](/docs/concepts/transformed_input_range)

## Example

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  std::string const hello("SGVsbG8sIFdvcmxkIQ==");

  auto decoder = base64::make_decoder(hello.begin(), hello.end());
}
```
