---
layout: default
title: encoded_size
parent: base64
grand_parent: Codecs
nav_order: 5
permalink: /docs/codecs/base64/encoded_size
---

# mgs::base64::encoded_size

```cpp
constexpr static std::size_t encoded_size(std::size_t decoded_size) noexcept;
```

Returns exact encoded size assuming an input of size `decoded_size`.

## Example

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  static_assert(base64::encoded_size(8) == 12, "");
}
```
