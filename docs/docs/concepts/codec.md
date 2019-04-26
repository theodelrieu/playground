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
  std::InputIterator<I1> &&
  std::Sentinel<S1, I1> &&
  std::InputIterator<I2> &&
  std::Sentinel<S2, I2> &&
  requires(A1& a1, A2& a2, I1 i1, S1 s1, I2 i2, S2 s2) {
    { T::make_encoder(i1, s1) } -> TransformedInputRange;
    { T::make_decoder(i2, s2) } -> TransformedInputRange;

    CodecOutput<typename T::default_encoded_output, decltype(T::make_encoder(i1, s1))>;
    CodecOutput<typename T::default_decoded_output, decltype(T::make_decoder(i2, s2))>;
    CodecOutput<R1, decltype(T::make_encoder(i1, s1))>;
    CodecOutput<R2, decltype(T::make_decoder(i2, s2))>;

    { T::encode(i1, s1) } -> std::Same<typename T::default_encoded_output>;
    { T::encode(a1) } -> std::Same<typename T::default_encoded_output>;
    { T::encode<R1>(i1, s1) } -> std::Same<R1>;
    { T::encode<R1>(a1) } -> std::Same<R1>;

    { T::decode(i2, s2) } -> std::Same<typename T::default_decoded_output>;
    { T::decode(a2) } -> std::Same<typename T::default_decoded_output>;
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

| Template argument | Definition                                                                               | Constraints                                                                     |
|-------------------|------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------|
| `A1`              | Type passed to `encode`. Defaulted to `T::default_decoded_output`.                       |                                                                                 |
| `A2`              | Type passed to `decode`. Defaulted to `T::default_encoded_output`.                       |                                                                                 |
| `R1`              | `encode` return type. Defaulted to `T::default_encoded_output`.                          | [`CodecOutput<decltype(T::make_encoder(i1, s1))>`](/docs/concepts/codec_output) |
| `R2`              | `decode` return type. Defaulted to `T::default_decoded_output`.                          | [`CodecOutput<decltype(T::make_decoder(i2, s2))>`](/docs/concepts/codec_output) |
| `I1`              | Iterator type passed to `encode` and `make_encoder`, defaulted to `decltype(begin(a1))`. | [`std::InputIterator`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/InputIterator)                                                        |
| `S1`              | Sentinel type passed to `encode` and `make_encoder`, defaulted to `I1`.                  | [`std::Sentinel<I1>`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/Sentinel)                                                         |
| `I2`              | Iterator type passed to `decode` and `make_decoder`, defaulted to `decltype(begin(a2))`. | [`std::InputIterator`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/InputIterator)                                                        |
| `S2`              | Sentinel type passed to `decode` and `make_decoder`, defaulted to `I2`.                  | [`std::Sentinel<I2>`](https://en.cppreference.com/w/cpp/experimental/ranges/iterator/Sentinel)                                                         |

## Member types

| Member type              | Definition                                                        | Type                   | Constraints                                                                     |
|--------------------------+-------------------------------------------------------------------+------------------------+---------------------------------------------------------------------------------|
| `default_encoded_output` | Type returned by `encode` when no template parameter is specified | Implementation defined | [`CodecOutput<decltype(T::make_encoder(i1, s1))>`](/docs/concepts/codec_output) |
| `default_decoded_output` | Type returned by `decode` when no template parameter is specified | Implementation defined | [`CodecOutput<decltype(T::make_decoder(i2, s2))>`](/docs/concepts/codec_output) |

## Valid expressions

| Expression                | Return type                                                       |
|---------------------------+-------------------------------------------------------------------|
| `T::make_encoder(i1, s1)` | [`TransformedInputRange`](/docs/concepts/transformed_input_range) |
| `T::make_decoder(i2, s2)` | [`TransformedInputRange`](/docs/concepts/transformed_input_range) |
| `T::encode(i1, s1)`       | `T::default_encoded_output`                                       |
| `T::encode(a1)`           | `T::default_encoded_output`                                       |
| `T::encode<R1>(i1, s1)`   | `R1`                                                              |
| `T::encode<R1>(a1)`       | `R1`                                                              |
| `T::decode(i2, s2)`       | `T::default_decoded_output`                                       |
| `T::decode(a2)`           | `T::default_decoded_output`                                       |
| `T::decode<R2>(i2, s2)`   | `R2`                                                              |
| `T::decode<R2>(a2)`       | `R2`                                                              |

## Expression semantics

| Expression                | Precondition                           | Semantics                                                                      |
|---------------------------+----------------------------------------+--------------------------------------------------------------------------------|
| `T::make_encoder(i1, s1)` | `[i1, s1)` denotes a valid input range | Creates an encoder from the input range                                        |
| `T::make_decoder(i2, s2)` | `[i2, s2)` denotes a valid input range | Creates a decoder from the input range                                         |
| `T::encode(i1, s1)`       | `[i1, s1)` denotes a valid input range | Encodes the input range and returns a value of the default encoded output type |
| `T::encode(a1)`           |                                        | Encodes the input and returns a value of the default encoded output type       |
| `T::encode<R1>(i1, s1)`   | `[i1, s1)` denotes a valid input range | Encodes the input range and returns a value of type `R1`                       |
| `T::encode<R1>(a1)`       |                                        | Encodes the input and returns a value of type `R1`                             |
| `T::decode(i2, s2)`       | `[i2, s2)` denotes a valid input range | Decodes the input range and returns a value of the default decoded output type |
| `T::decode(a2)`           |                                        | Decodes the input and returns a value of the default decoded output type       |
| `T::decode<R2>(i2, s2)`   | `[i2, s2)` denotes a valid input range | Decodes the input range and returns a value of type `R2`                       |
| `T::decode<R2>(a2)`       |                                        | Decodes the input and returns a value of type `R2`                             |

## Concept emulation

```cpp
namespace mgs {
namespace concepts {

template <typename T,
          typename A1 = /* ... */, typename A2 = /* ... */,
          typename R1 = /* ... */, typename R2 = /* ... */,
          typename I1 = /* ... */, typename I2 = /* ... */,
          typename S1 = /* ... */, typename S2 = /* ... */>
struct is_codec { /* ... */ };

template <typename T,
          typename A1 = /* ... */, typename A2 = /* ... */,
          typename R1 = /* ... */, typename R2 = /* ... */,
          typename I1 = /* ... */, typename I2 = /* ... */,
          typename S1 = /* ... */, typename S2 = /* ... */>
constexpr auto is_codec_v = is_codec<T, A1, A2, R1, R2, I1, I2, S1, S2>::value;
}

template <typename T,
          typename A1 = /* ... */, typename A2 = /* ... */,
          typename R1 = /* ... */, typename R2 = /* ... */,
          typename I1 = /* ... */, typename I2 = /* ... */,
          typename S1 = /* ... */, typename S2 = /* ... */,
          typename = std::enable_if_t<is_codec_v<T, A1, A2, R1, R2, I1, I2, S1, S2>>>
using Codec = T;
}
```

## Example

```cpp
#include <mgs/base64.hpp>
#include <mgs/concepts/codec.hpp>

using namespace mgs;

int main() {
  static_assert(is_codec_v<base64>, "");
  static_assert(is_codec_v<base64, std::string, std::vector<unsigned char>>, "");
  static_assert(is_codec_v<base64,
                                    std::string, std::string,
                                    char*, std::istreambuf_iterator<char>>, "");
}
```
