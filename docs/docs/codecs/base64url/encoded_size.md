---
layout: default
title: encoded_size
parent: base64url
grand_parent: Codecs
nav_order: 5
permalink: /docs/codecs/base64url/encoded_size
---

# mgs::base64url::encoded_size

```cpp
constexpr static std::size_t encoded_size(std::size_t decoded_size) noexcept;
```

Returns exact encoded size assuming an input of size `decoded_size`.

## Example

```cpp
#include <mgs/base64url.hpp>

using namespace mgs;

int main() {
  static_assert(base64url::encoded_size(8) == 12, "");
}
```
