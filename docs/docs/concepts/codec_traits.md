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
template <typename T, typename I, typename S = I>
concept CodecTraits =
  std::InputIterator<I> &&
  std::Sentinel<S, I> &&
  requires(I i, S s) {
    { T::make_encoder(i, s) } -> TransformedInputView;
    { T::make_decoder(i, s) } -> TransformedInputView;
    CodecOutput<typename T::default_encoded_output, decltype(T::make_encoder(i, s))>;
    CodecOutput<typename T::default_decoded_output, decltype(T::make_decoder(i, s))>;
  };
```

`CodecTraits` are helper types used in most `mgs`' codecs.

They create `Encoder`s and `Decoder`s, and define default return types for `encode` and `decode` static member functions.

## Notation

* `i` - value of type `I`
* `s` - value of type `S`
* `Encoder` - type returned by `T::make_encoder(i, s)`
* `Decoder` - type returned by `T::make_decoder(i, s)`

## Associated types

| Member type                 | Definition                         | Constraints                |
|-----------------------------+------------------------------------+----------------------------|
| `T::default_encoded_output` | Default return type of `T::encode` | [`CodecOutput<Encoder>`]() |
| `T::default_decoded_output` | Default return type of `T::decode` | [`CodecOutput<Decoder>`]() |

## Template arguments

| Template argument | Definition                                               | Constraints              |
|-------------------+----------------------------------------------------------+--------------------------|
| `I`               | Iterator type passed to `make_encoder`                   | [`std::InputIterator`]() |
| `S`               | Sentinel type passed to `make_encoder`, defaulted to `I` | [`std::Sentinel<I>`]()   |

## Valid expressions

| Expression                | Return type                |
|---------------------------+----------------------------|
| `T::make_encoder(i, s)`   | [`TransformedInputView`]() |
| `T::make_decoder(i, s)` | [`TransformedInputView`]() |

## Expression semantics

| Expression              | Precondition                         | Semantics                       | Postcondition |
|-------------------------+--------------------------------------+---------------------------------+---------------|
| `T::make_encoder(i, s)` | `[i, s)` denotes a valid input range | Creates and return an `Encoder` |               |
| `T::make_decoder(i, s)` | `[i, s)` denotes a valid input range | Creates and return an `Decoder` |               |

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
