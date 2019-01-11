---
layout: default
title: max_decoded_size
parent: base64url
grand_parent: Codecs
nav_order: 6
permalink: /docs/codecs/base64url/max_decoded_size
---

# mgs::base64url::max_decoded_size

```cpp
constexpr static std::size_t max_decoded_size(std::size_t encoded_size);
```

Returns maximum decoded size assuming an input of size `encoded_size`.

## Exceptions

Throws `exceptions::invalid_input_error` if `encoded_size` does not represent a valid base64url-encoded input size.

## Example

```cpp
#include <mgs/base64url.hpp>

using namespace mgs;

int main() {
  static_assert(base64url::max_decoded_size(12) == 9, "");
  // Since base64url encoded output is always padded to be a multiple of 4
  // the following will throw
  // base64url::max_decoded_size(11);
}
```
