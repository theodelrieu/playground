---
layout: default
title: CodecTraits
nav_order: 9
parent: Concepts
permalink: /docs/concepts/codec_traits
---

# mgs::CodecTraits

Defined in header `<mgs/concepts/codec_traits.hpp>`

```cpp
template <typename T, typename I1, typename S1 = I1, typename I2 = I1, typename S2 = S1>
concept CodecTraits =
  std::InputIterator<I1> &&
  std::Sentinel<S1, I1> &&
  std::InputIterator<I2> &&
  std::Sentinel<S2, I2> &&
  requires(I1 i1, S1 s1, I2 i2, S2 s2) {
    { T::make_encoder(i1, s1) } -> TransformedInputView;
    { T::make_decoder(i2, s2) } -> TransformedInputView;
    CodecOutput<typename T::default_encoded_output, decltype(T::make_encoder(i1, s1))>;
    CodecOutput<typename T::default_decoded_output, decltype(T::make_decoder(i2, s2))>;
  };
```

`CodecTraits` are helper types used in most `mgs`' codecs.

They create `Encoder`s and `Decoder`s, and define default return types for `encode` and `decode` static member functions.

## Notation

* `i1` - value of type `I1`
* `s1` - value of type `S1`
* `i2` - value of type `I2`
* `s2` - value of type `S2`
* `Encoder` - type returned by `T::make_encoder(i1, s1)`
* `Decoder` - type returned by `T::make_decoder(i2, s2)`

## Associated types

| Member type                 | Definition                         | Constraints                |
|-----------------------------+------------------------------------+----------------------------|
| `T::default_encoded_output` | Default return type of `T::encode` | [`CodecOutput<Encoder>`]() |
| `T::default_decoded_output` | Default return type of `T::decode` | [`CodecOutput<Decoder>`]() |

## Template arguments

| Template argument | Definition                                                | Constraints              |
|-------------------+-----------------------------------------------------------+--------------------------|
| `I1`              | Iterator type passed to `make_encoder`                    | [`std::InputIterator`]() |
| `S1`              | Sentinel type passed to `make_encoder`, defaulted to `I1` | [`std::Sentinel<I1>`]()  |
| `I2`              | Iterator type passed to `make_decoder`                    | [`std::InputIterator`]() |
| `S2`              | Sentinel type passed to `make_decoder`, defaulted to `I2` | [`std::Sentinel<I2>`]()  |

## Valid expressions

| Expression                | Return type                |
|---------------------------+----------------------------|
| `T::make_encoder(i1, s1)` | [`TransformedInputView`]() |
| `T::make_decoder(i2, s2)` | [`TransformedInputView`]() |

## Expression semantics

| Expression                | Precondition                           | Semantics                       | Postcondition |
|---------------------------+----------------------------------------+---------------------------------+---------------|
| `T::make_encoder(i1, s1)` | `[i1, s1)` denotes a valid input range | Creates and return an `Encoder` |               |
| `T::make_decoder(i2, s2)` | `[i2, s2)` denotes a valid input range | Creates and return an `Decoder` |               |

## Example

```cpp
#include <mgs/concepts/codec_traits.hpp>
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  static_assert(concepts::is_codec_traits<base64::codec_traits>::value, "");

  base64::codec_traits::default_encoded_output encoded = base64::encode("Hello, World!");
  base64::codec_traits::default_decoded_output decoded = base64::decode(encoded);
}
```
