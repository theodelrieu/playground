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

template <typename T, typename View>
concept CodecOutput =
  TransformedInputView<View> &&
  requires (V& view) {
    { codecs::output_traits<T>::create(view) } -> std::Same<T>;
  };
```

The `CodecOutput<T, View>` concept is satisfied when `T` can be created from `View`'s transformed input.

It relies on the customization point `mgs::codecs::output_traits<T>::create(View&)`, declared in header `<mgs/codecs/output_traits_fwd.hpp>`.

## Template arguments

| Template argument | Definition                                                        | Constraints                |
|-------------------+-------------------------------------------------------------------+----------------------------|
| `View`            | View passed to the `output_traits<T>::create` customization point | [`TransformedInputView`]() |

## Notation

* `v` - a value of type `View&`

## Valid expressions

| Expression                            | Return type |
|---------------------------------------+-------------|
| `codecs::output_traits<T>::create(v)` | `T`         |

## Expression semantics

| Expression                            | Precondition | Semantics                                                                                                                         | Postcondition |
|---------------------------------------+--------------+-----------------------------------------------------------------------------------------------------------------------------------+---------------|
| `codecs::output_traits<T>::create(v)` |              | Creates a `T` from `v`'s transformed input (if `codecs::output_traits<T>` is not specialized, the default implementation is used) |               |

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
  template <typename View>
  static QLinkedList<CharT> create(View& view) {
    QLinkedList<CharT> list;
    std::copy(view.begin(), view.end(), std::back_inserter(list));
    return list;
  }
};

using namespace mgs;

int main() {
  using encoder = base64::encoder<char const*>;
  
  static_assert(concepts::is_codec_output<std::string, encoder>::value, "");
  static_assert(concepts::is_codec_output<QLinkedList<char>, encoder>::value, "");

  static_assert(!concepts::is_codec_output<QList<char>, encoder>::value, "");
}
```
