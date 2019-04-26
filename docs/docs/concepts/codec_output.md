---
layout: default
title: CodecOutput
nav_order: 8
parent: Concepts
permalink: /docs/concepts/codec_output
---

# mgs::CodecOutput

Defined in header `<mgs/concepts/codec_output.hpp>`

```cpp
#include <mgs/codecs/output_traits_fwd.hpp>

template <typename T, typename Range>
concept CodecOutput =
  TransformedInputRange<Range> &&
  requires (Range& range) {
    { codecs::output_traits<T>::create(range) } -> std::Same<T>;
  };
```

The `CodecOutput<T, Range>` concept is satisfied when `T` can be created from `Range`'s transformed input.

It relies on the customization point `mgs::codecs::output_traits<T>::create(Range&)`, declared in header `<mgs/codecs/output_traits_fwd.hpp>`.

## Template arguments

| Template argument | Definition                                                        | Constraints                |
|-------------------+-------------------------------------------------------------------+----------------------------|
| `Range`            | Range passed to the `output_traits<T>::create` customization point | [`TransformedInputRange`](/docs/concepts/transformed_input_range) |

## Notation

* `v` - a value of type `Range&`

## Valid expressions

| Expression                            | Return type |
|---------------------------------------+-------------|
| `codecs::output_traits<T>::create(v)` | `T`         |

## Expression semantics

| Expression                            | Precondition | Semantics                                                                                                                         | Postcondition |
|---------------------------------------+--------------+-----------------------------------------------------------------------------------------------------------------------------------+---------------|
| `codecs::output_traits<T>::create(v)` |              | Creates a `T` from `v`'s transformed input (if `codecs::output_traits<T>` is not specialized, the default implementation is used) |               |

## Concept emulation

```cpp
namespace mgs {
namespace concepts {

template <typename T, typename Range>
struct is_codec_output { /* ... */ };

template <typename T, typename Range>
constexpr auto is_codec_output_v = is_codec_output<T, Range>::value;
}

template <typename T, typename Range,
          typename = std::enable_if_t<is_codec_output_v<T, Range>>>
using CodecOutput = T;
}
```

## Example

```cpp
#include <mgs/concepts/codec_output.hpp>
#include <mgs/concepts/output_traits_fwd.hpp>

#include <mgs/base64.hpp>

#include <QLinkedList>
#include <QList>

// add support for QLinkedList
namespace mgs {
namespace codecs {
template <typename CharT>
struct output_traits<QLinkedList<CharT>> {
  template <typename Range>
  static QLinkedList<CharT> create(Range& range) {
    QLinkedList<CharT> list;
    std::copy(range.begin(), range.end(), std::back_inserter(list));
    return list;
  }
};

using namespace mgs;

int main() {
  using encoder = base64::encoder<char const*>;
  
  static_assert(is_codec_output_v<std::string, encoder>, "");
  static_assert(is_codec_output_v<QLinkedList<char>, encoder>, "");

  static_assert(!is_codec_output_v<QList<char>, encoder>, "");
}
```
