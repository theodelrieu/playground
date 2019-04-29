---
layout: default
title: Binary-to-text codec variant
parent: Write your own codec
nav_order: 1
permalink: /docs/wyoc/variant
---

# Binary-to-text codec variant

This section explains how to create binary-to-text codec variants (e.g. [`base64`](/docs/codecs/base64) with a different alphabet).

## Define the encoding traits

To create your codec variant, you first need to define an encoding traits type, which must model [`binary_to_base::EncodingTraits`](/docs/concepts/encoding_traits).

Here are the [`base64`](/docs/codecs/base64) traits (simplified):

```cpp
#include <mgs/binary_to_base/padding_policy.hpp>

using namespace mgs;

struct base64_encoding_traits
{
  using alphabet_t = char const[64];

  // using C++17 inline variables for simplicity
  inline static constexpr alphabet_t alphabet = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
      'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

  static constexpr char padding_character = '=';
  static constexpr auto padding_policy = binary_to_base::padding_policy::required;

  static constexpr int index_of(char c)
  {
    /* ... */
  }
};
```

## Create the codec variant

`mgs` uses a generic implementation for every binary-to-text codec, which can be parameterized with encoding traits:

```cpp
// Header <mgs/codecs/binary_to_base/basic_codec.hpp>

namespace mgs {
namespace binary_to_base {

template <typename EncodingTraits, typename DecodingTraits = EncodingTraits>
class basic_codec { /* ... */ };
}
}
```

You just have to pass your encoding traits:

```cpp
struct custom_traits { /* ... */ };

int main() {
  using my_custom_base64 = mgs::binary_to_base::basic_codec<custom_traits>;

  my_custom_base64::encode("Hello, World!");
}
```

Note
{: .label .label-blue }
`binary_to_base::basic_codec`s second template parameter is useful if you want to have different behaviors when encoding and decoding (e.g. [`base64url_nopad`](/docs/codecs/base64url) has different padding policies).
