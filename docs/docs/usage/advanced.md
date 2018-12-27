---
layout: default
title: Advanced
nav_order: 4
parent: Usage
---

# Advanced usage
{: .no_toc }

This section explains in detail new and previously discussed topics, and how to write a custom codec.

## Table of contents
{: .no_toc .text_delta }

1. TOC
{: toc }

---

## Quirks and details

### Constraints on encode/decode input parameters

Codecs share a common set of contraints on `encode`/`decode` input parameters.

As discussed in the [basic section](basic), the input parameters must either be:

1. An [`Iterable`]()
1. An [`InputIterator`]() and a [`Sentinel<InputIterator>`]()
1. `std::istream&`

We will focus on `2.` since the other two are built on top of it.

Depending on the called function, an Encoder or a Decoder will be constructed with the iterator-sentinel pair. 

This type must model the [`IterableTransformedInputAdapter`]() concept. Otherwise, a compiler error will be triggered.

Note
{: .label .label-blue }
Codecs can have additional constraints, which are described in each codec's section (e.g. [`base64`]()).

### Constraints on encode/decode return types

Just like with their input parameters, `encode`/`decode` impose contraints on which types they return.

As explained [previously](), an Encoder or a Decoder is created when `encode`/`decode` is called, which must model the [`IterableTransformedInputAdapter`]() concept.

The return type itself must also model a concept: [`CodecOutput`]().

Otherwise, a compiler error will be triggered.

### Iterator related optimizations

While codecs accepts every Iterable (assuming it fulfills the constraints properly), some of them will perform better when the underlying iterators are [`RandomAccessIterator`]()s.

This is the case for [`base64`]()'s encoder and decoder. Both will define the `max_transformed_size` member function, which can be used to perform a single memory allocation:

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  // std::string::iterator is a RandomAccessIterator
  std::string huge_string = /* ... */;

  auto encoder = base64::make_encoder(huge_string.begin(), huge_string.end());
  auto const max_size = encoder.max_transformed_size();

  // allocate max_size bytes in one go
  std::string encoded_str(max_size, '\0');
  auto const total_written = encoder.write(encoded_str.begin(), max_size);

  // do not forget to resize at the end!
  encoded_str.resize(total_written);
}
```

Note
{: .label .label-blue }

Codecs defining `max_transformed_size` also model the [`SizedTransformedInputAdapter`]() concept.

### Default return-type-creation mechanism

It can be helpful to understand what happens during the default return-type-creation mechanism.

Given the following requirements:

* For a type `T`, assuming `mgs::codecs::output_traits<T>` has **NOT** been specialized by the user
* For a type `U`, modeling the [`IterableTransformedInputAdapter`]() concept

The following steps will be executed:

* `mgs::codecs::output_traits<T>::create` delegates the call to an internal converter
* If `T` is a `std::array<CharT, std::size_t>`
  * `U` must model [`IterableTransformedInputAdapter<CharT const*>`]()
* Else if `T` models [`DefaultConstructible`]() and either [`CopyConstructible`]() or [`MoveConstructible`]()
  * If `T`:
    * Has [`RandomAccessIterator`]()s
    * Has a `size_type` member type alias
    * Has a `resize(size_type)` member function
      * The fastest path is taken, especially if `U` models [`SizedTransformedInputAdapter`]()
  * Else if `T`:
      * Does not model [`AssociativeContainer`]()
      * Models [`Constructible<U::iterator, U::sentinel>`]()
          * `T`'s iterator range constructor is called
* Otherwise, a compiler error will be triggered

## Advanced customization

### User-defined types (advanced)

Partial specialization + SFINAE, how to use SizedTransformedInputAdapter

### Write a codec variant

show how to change base64 alphabet

### Write a new codec

use basic_codec, mention InputTransformer/SizedInputTransformer
