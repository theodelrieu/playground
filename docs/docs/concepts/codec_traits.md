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
template <typename T,
          typename A1 = typename T::default_encoded_output,
          typename A2 = typename T::default_decoded_output,
          typename I1 = /* see below */, typename I2 = /* see below */,
          typename S1 = /* see below */, typename S2 = /* see below */>
concept CodecTraits =
  std::InputIterator<I1> &&
  std::Sentinel<S1, I1> &&
  std::InputIterator<I2> &&
  std::Sentinel<S2, I2> &&
  requires(I1 i1, S1 s1, I2 i2, S2 s2) {
    { T::make_encoder(i1, s1) } -> TransformedInputRange;
    { T::make_decoder(i2, s2) } -> TransformedInputRange;
    CodecOutput<A1, decltype(T::make_encoder(i1, s1))>;
    CodecOutput<A2, decltype(T::make_decoder(i2, s2))>;
  };
```

`CodecTraits` are helper types used in most `mgs`' codecs.

They create `Encoder`s and `Decoder`s, and define default return types for `encode` and `decode` static member functions.

## Notation

* `a1` - value of type `A1&`
* `a2` - value of type `A2&`
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

| Template argument | Definition                                                                  | Constraints              |
|-------------------|-----------------------------------------------------------------------------|--------------------------|
| `I1`              | Iterator type passed to `make_encoder`, defaulted to `decltype(begin(a1))`. | [`std::InputIterator`]() |
| `S1`              | Sentinel type passed to `make_encoder`, defaulted to `I1`.                  | [`std::Sentinel<I1>`]()  |
| `I2`              | Iterator type passed to `make_decoder`, defaulted to `decltype(begin(a2))`. | [`std::InputIterator`]() |
| `S2`              | Sentinel type passed to `make_decoder`, defaulted to `I2`.                  | [`std::Sentinel<I2>`]()  |

## Valid expressions

| Expression                | Return type                |
|---------------------------+----------------------------|
| `T::make_encoder(i1, s1)` | [`TransformedInputRange`]() |
| `T::make_decoder(i2, s2)` | [`TransformedInputRange`]() |

## Expression semantics

| Expression                | Precondition                           | Semantics                       | Postcondition |
|---------------------------+----------------------------------------+---------------------------------+---------------|
| `T::make_encoder(i1, s1)` | `[i1, s1)` denotes a valid input range | Creates and return an `Encoder` |               |
| `T::make_decoder(i2, s2)` | `[i2, s2)` denotes a valid input range | Creates and return an `Decoder` |               |

## Concept emulation

```cpp
namespace mgs {
namespace concepts {

template <typename T,
          typename A1 = /* ... */, typename A2 = /* ... */,
          typename I1 = /* ... */, typename I2 = /* ... */,
          typename S1 = /* ... */, typename S2 = /* ... */>
struct is_codec_traits { /* ... */ };

template <typename T,
          typename A1 = /* ... */, typename A2 = /* ... */,
          typename I1 = /* ... */, typename I2 = /* ... */,
          typename S1 = /* ... */, typename S2 = /* ... */>
constexpr auto is_codec_traits_v = is_codec_traits<T, A1, A2, I1, I2, S1, S2>::value;
}

template <typename T,
          typename A1 = /* ... */, typename A2 = /* ... */,
          typename I1 = /* ... */, typename I2 = /* ... */,
          typename S1 = /* ... */, typename S2 = /* ... */,
          typename = std::enable_if_t<concepts::is_codec_traits_v<T, A1, A2, I1, I2, S1, S2>>>
using CodecTraits = T;
}
```
