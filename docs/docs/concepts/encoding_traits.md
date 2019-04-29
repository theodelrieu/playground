---
layout: default
title: EncodingTraits
nav_order: 9
parent: Concepts
permalink: /docs/concepts/encoding_traits
---

# mgs::binary_to_base::EncodingTraits

Defined in header `<mgs/binary_to_base/concepts/encoding_traits.hpp>`

```cpp
#include <mgs/binary_to_base/padding_policy.hpp>

template <std::size_t N>
constexpr std::size_t is_power_of_2()
{
  return N != 0 && ((N & (N - 1)) == 0);
}

template <typename T>
concept EncodingTraits = 
  std::is_array_v<decltype(T::alphabet)> &&
  is_power_of_2<sizeof(T::alphabet)>() &&
  std::Same<char, std::remove_const_t<std::remove_extent_t<decltype(T::alphabet)>>> &&
  std::Same<padding_policy, std::remove_const_t<decltype(T::padding_policy)>> &&
  (std::Same<char, std::remove_const_t<decltype(T::padding_character)>> ||
  T::padding_policy == padding_policy::none) &&
  requires(char c) {
    { T::index_of(c) } -> std::Same<int>;
  };
```

`EncodingTraits` are helper types used in the implementation of `binary_to_base`'s `Encoder` and `Decoder`.

They define the codec's alphabet, the padding policy and character (if padding is desired).

## Notation

* `c` - value of type `char`

## Static data members

| Static data member     | Definition               | Type                             | Constraints                                     |
|------------------------+--------------------------+----------------------------------+-------------------------------------------------|
| `T::alphabet`          | Base's alphabet          | `char[]`                         | `sizeof(T::alphabet)` must be a power of 2      |
| `T::padding_policy`    | Base's padding policy    | `binary_to_base::padding_policy` | Must be either `none`, `optional` or `required` |
| `T::padding_character` | Base's padding character | `char`                           | Not needed when `padding_policy` is `none`      |

## Valid expressions

| Expression       | Return type |
|------------------+-------------|
| `T::index_of(c)` | `int`       |

## Expression semantics

| Expression       | Precondition | Semantics                                           | Postcondition |
|------------------+--------------+-----------------------------------------------------+---------------|
| `T::index_of(c)` |              | Returns the index of `c` in `T::alphabet`, or `-1`. |               |

## Concept emulation

```cpp
namespace mgs {
namespace binary_to_base {
namespace concepts {

template <typename T>
struct is_encoding_traits { /* ... */ };

template <typename T>
constexpr auto is_encoding_traits_v = is_encoding_traits<T>::value;
}

template <typename T,
          typename = std::enable_if_t<is_encoding_traits_v<T>>>
using EncodingTraits = T;
}
}
```

## Example

```cpp
#include <mgs/binary_to_base/concepts/encoding_traits.hpp>
#include <mgs/binary_to_base/padding_policy.hpp>
#include <mgs/binary_to_base/basic_codec.hpp>

using namespace mgs;
// using C++17 inline variables for simplicity
struct binary_traits {
  inline static char const[] alphabet = {'0', '1'};
  inline static auto const padding_policy = binary_to_base::padding_policy::none;

  static int index_of(char c) {
    if (c == '0')
      return 0;
    if (c == '1')
      return 1;
    return -1;
  }
};

int main() {
  static_assert(binary_to_base::is_encoding_traits_v<binary_traits>, "");

  using binary_codec = binary_to_base::basic_codec<binary_traits>;
  // encodes to 01100001
  binary_codec::encode("a");
}
```
