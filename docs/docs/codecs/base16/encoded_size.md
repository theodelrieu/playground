---
layout: default
title: encoded_size
parent: base16
grand_parent: Codecs
nav_order: 5
permalink: /docs/codecs/base16/encoded_size
---

# mgs::base16::encoded_size

```cpp
constexpr static std::size_t encoded_size(std::size_t decoded_size) noexcept;
```

Returns exact encoded size assuming an input of size `decoded_size`.

## Example

```cpp
#include <mgs/base16.hpp>

using namespace mgs;

int main() {
  static_assert(base16::encoded_size(8) == 16, "");
}
```
