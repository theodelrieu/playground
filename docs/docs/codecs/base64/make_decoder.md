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
template <typename InputIterator, typename Sentinel>
static decoder<InputIterator, Sentinel> make_decoder(InputIterator begin, Sentinel end);
```

Constructs an `Decoder` from an iterator range.

Calling `make_decoder` is well-formed if:

* `InputIterator` models [`InputIterator`]()
* `Sentinel` models [`Sentinel<InputIterator>`]()
* `decoder<InputIterator, Sentinel>` models [`IterableTransformedInputAdapter`]()

## Example

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  std::string const hello("SGVsbG8sIFdvcmxkIQ==");

  auto decoder = base64::make_decoder(hello.begin(), hello.end());
}
```
