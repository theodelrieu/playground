---
layout: default
title: max_decoded_size
parent: base32
grand_parent: Codecs
nav_order: 6
permalink: /docs/codecs/base32/max_decoded_size
---

# mgs::base32::max_decoded_size

```cpp
constexpr static std::size_t max_decoded_size(std::size_t encoded_size);
```

Returns maximum decoded size assuming an input of size `encoded_size`.

## Exceptions

Throws `exceptions::invalid_input_error` if `encoded_size` does not represent a valid base32-encoded input size.

## Example

```cpp
#include <mgs/base32.hpp>

using namespace mgs;

int main() {
  static_assert(base32::max_decoded_size(16) == 10, "");
  // Since base32 encoded output is always padded to be a multiple of 8
  // the following will throw
  // base32::max_decoded_size(15);
}
```
