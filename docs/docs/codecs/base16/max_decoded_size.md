---
layout: default
title: max_decoded_size
parent: base16
grand_parent: Codecs
nav_order: 6
permalink: /docs/codecs/base16/max_decoded_size
---

# mgs::base16::max_decoded_size

```cpp
constexpr static std::size_t max_decoded_size(std::size_t encoded_size);
```

Returns maximum decoded size assuming an input of size `encoded_size`.

## Exceptions

Throws `exceptions::invalid_input_error` if `encoded_size` does not represent a valid base16-encoded input size.

## Example

```cpp
#include <mgs/base16.hpp>

using namespace mgs;

int main() {
  static_assert(base16::max_decoded_size(16) == 8, "");
  // Since base16 encoded output is always a multiple of 2,
  // the following will throw
  // base16::max_decoded_size(15);
}
```
