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
template <typename T,
          typename A1 = /* see below */, typename A2 = /* see below */,
          typename R1 = /* see below */, typename R2 = /* see below */,
          typename I1 = /* see below */, typename I2 = /* see below */,
          typename S1 = /* see below */, typename S2 = /* see below */>
concept Codec =
  std::Iterator<I1> &&
  std::Sentinel<S1, I1> &&
  std::Iterator<I2> &&
  std::Sentinel<S2, I2> &&
  requires(A1& a1, A2& a2, I1 i1, S1 s1, I2 i2, S2 s2) {
    CodecTraits<typename T::codec_traits, I1, S1, I2, S2>;
    CodecOutput<R1, typename T::template encoder<I1, S1>>;
    CodecOutput<R2, typename T::template decoder<I2, S2>>;

    { T::codec_traits::make_encoder(i1, s1) } -> std::Same<typename T::template encoder<I1, S1>>;
    { T::codec_traits::make_decoder(i2, s2) } -> std::Same<typename T::template decoder<I2, S2>>;

    { T::encode(i1, s1) } -> std::Same<typename T::codec_traits::default_encoded_output>;
    { T::encode(a1) } -> std::Same<typename T::codec_traits::default_encoded_output>;
    { T::encode<R1>(i1, s1) } -> std::Same<R1>;
    { T::encode<R1>(a1) } -> std::Same<R1>;

    { T::decode(i2, s2) } -> std::Same<typename T::codec_traits::default_decoded_output>;
    { T::decode(a2) } -> std::Same<typename T::codec_traits::default_decoded_output>;
    { T::decode<R2>(i2, s2) } -> std::Same<R2>;
    { T::decode<R2>(a2) } -> std::Same<R2>;
  };
```

A `Codec` is a type allowing encoding and decoding arbitrary input.

It is `mgs`' fundamental component.

## Notation

* `a1` - value of type `A1&`
* `i1` - value of type `I1`
* `s1` - value of type `S1`
* `a2` - value of type `A2&`
* `i2` - value of type `I2`
* `s2` - value of type `S2`

## Template arguments

| Template argument | Definition                                                                                | Constraints                           |
|-------------------+-------------------------------------------------------------------------------------------+---------------------------------------|
| `A1`              | Type passed to `encode`. Defaulted to `codec_traits::default_decoded_output`.             |                                       |
| `A2`              | Type passed to `decode`. Defaulted to `codec_traits::default_encoded_output`.             |                                       |
| `R1`              | `encode` return type. Defaulted to `codec_traits::default_encoded_output`.                | [`CodecOutput<T::encoder<I1, S1>>`]() |
| `R2`              | `decode` return type. Defaulted to `codec_traits::default_decoded_output`.                | [`CodecOutput<T::decoder<I2, S2>>`]() |
| `I1`              | Iterator type associated with `A1`. Defaulted to `decltype(begin(a1))`.                   | [`std::InputIterator`]()              |
| `S1`              | Sentinel type associated with `A1`. Defaulted to `decltype(end(a1))`, falls back to `I1`. | [`std::Sentinel<I1>`]()               |
| `I2`              | Iterator type associated with `A2`. Defaulted to `decltype(begin(a2))`.                   | [`std::InputIterator`]()              |
| `S2`              | Sentinel type associated with `A2`. Defaulted to `decltype(end(a2))`, falls back to `I2`  | [`std::Sentinel<I2>`]()               |

## Member types

| Member type       | Definition         | Type                   | Constraints                       |
|-------------------+--------------------+------------------------+-----------------------------------|
| `T::codec_traits` | Codec traits class | Implementation defined | [`CodecTraits<I1, S1, I2, S2>`]() |

## Member alias templates

| Template                                              | Definition                                        |
|-------------------------------------------------------+---------------------------------------------------|
| `template <typename I, typename S = I> using encoder` | return type of `codec_traits::make_encoder(I, S)` |
| `template <typename I, typename S = I> using decoder` | return type of `codec_traits::make_decoder(I, S)` |

## Valid expressions

| Expression              | Return type                               |
|-------------------------+-------------------------------------------|
| `T::encode(i1, s1)`     | `T::codec_traits::default_encoded_output` |
| `T::encode(a1)`         | `T::codec_traits::default_encoded_output` |
| `T::encode<R1>(i1, s1)` | `R1`                                      |
| `T::encode<R1>(a1)`     | `R1`                                      |
| `T::decode(i2, s2)`     | `T::codec_traits::default_decoded_output` |
| `T::decode(a2)`         | `T::codec_traits::default_decoded_output` |
| `T::decode<R2>(i2, s2)` | `R2`                                      |
| `T::decode<R2>(a2)`     | `R2`                                      |

## Expression semantics

| Expression              | Precondition                     | Semantics                                                                      | Postcondition |
|-------------------------+----------------------------------+--------------------------------------------------------------------------------+---------------|
| `T::encode(i1, s1)`     | `[i1, s1)` denotes a valid range | Encodes the input range and returns a value of the default encoded output type |               |
| `T::encode(a1)`         |                                  | Encodes the input and returns a value of the default encoded output type       |               |
| `T::encode<R1>(i1, s1)` | `[i1, s1)` denotes a valid range | Encodes the input range and returns a value of type `R1`                       |               |
| `T::encode<R1>(a1)`     |                                  | Encodes the input and returns a value of type `R1`                             |               |
| `T::decode(i2, s2)`     | `[i2, s2)` denotes a valid range | Decodes the input range and returns a value of the default decoded output type |               |
| `T::decode(a2)`         |                                  | Decodes the input and returns a value of the default decoded output type       |               |
| `T::decode<R2>(i2, s2)` | `[i2, s2)` denotes a valid range | Decodes the input range and returns a value of type `R2`                       |               |
| `T::decode<R2>(a2)`     |                                  | Decodes the input and returns a value of type `R2`                             |               |

## Example

```cpp
#include <mgs/base64.hpp>
#include <mgs/concepts/codec.hpp>

#include <sstream>

using namespace mgs;

int main() {
  static_assert(concepts::is_codec<base64>::value, "");
  static_assert(concepts::is_codec<base64, std::string, std::vector<unsigned char>>::value, "");
  static_assert(concepts::is_codec<base64,
                                   std::string, std::stringstream,
                                   char*, std::istreambuf_iterator<char>>::value, "");

  // std::stringstream is not Iterable, defaulted Iterator type will be invalid
  static_assert(!concepts::is_codec<base64, std::string, std::stringstream, char*>::value, "");
}
```
