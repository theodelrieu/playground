---
layout: default
title: Codec
nav_order: 10
parent: Concepts
permalink: /docs/concepts/codec
---

# mgs::Codec

Defined in header `<mgs/concepts/codec.hpp>`

```cpp
template <typename T, typename A,
          typename R1 = /* see below */, typename R2 = /* see below */,
          typename I = /* see below */, typename S = I>
concept Codec =
  std::Iterator<I> &&
  std::Sentinel<S, I> &&
  requires(A& a, I i, S s) {
    CodecTraits<typename T::codec_traits, I, S>;
    CodecOutput<R, typename T::template encoder<I, S>>;

    { T::codec_traits::make_encoder(i, s) } -> std::Same<typename T::template encoder<I, S>>;
    { T::codec_traits::make_decoder(i, s) } -> std::Same<typename T::template decoder<I, S>>;

    { T::encode(i, s) } -> std::Same<typename T::codec_traits::default_encoded_output>;
    { T::encode(a) } -> std::Same<typename T::codec_traits::default_encoded_output>;
    { T::encode<R1>(i, s) } -> std::Same<R1>;
    { T::encode<R1>(a) } -> std::Same<R1>;

    { T::decode(i, s) } -> std::Same<typename T::codec_traits::default_decoded_output>;
    { T::decode(a) } -> std::Same<typename T::codec_traits::default_decoded_output>;
    { T::decode<R2>(i, s) } -> std::Same<R2>;
    { T::decode<R2>(a) } -> std::Same<R2>;
  };
```

A `Codec` is a type allowing encoding and decoding arbitrary input.

It is `mgs`' fundamental component.

## Template arguments

| Template argument | Definition | Constraints |
|-------------------+------------+-------------|
| TODO use I1 I2 etc

## Notation

* `i` - value of type `I`
* `s` - value of type `S`
* `a` - value of type `A&`

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
  static_assert(binary_to_base::concepts::is_encoding_traits<binary_traits>::value, "");

  using binary_codec = binary_to_base::basic_codec<binary_traits>;
  // encodes to 01100001
  binary_codec::encode("a");
}
```
