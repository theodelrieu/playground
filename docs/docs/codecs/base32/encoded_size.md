---
layout: default
title: encoded_size
parent: base32
grand_parent: Codecs
nav_order: 5
permalink: /docs/codecs/base32/encoded_size
---

# mgs::base32::encoded_size

```cpp
constexpr static std::size_t encoded_size(std::size_t decoded_size) noexcept;
```

Returns exact encoded size assuming an input of size `decoded_size`.

## Example

```cpp
#include <mgs/base32.hpp>

using namespace mgs;

int main() {
  static_assert(base32::encoded_size(8) == 16, "");
  static_assert(base32::encoded_size(10) == 16, "");
  static_assert(base32::encoded_size(11) == 24, "");
}
```
