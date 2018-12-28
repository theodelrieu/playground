---
layout: default
title: Advanced
nav_order: 4
parent: Usage
---

# Advanced usage
{: .no_toc }

This section introduces some advanced topics and revisit in a bit more detail previously discussed ones.

## Table of contents
{: .no_toc .text_delta }

1. TOC
{: toc }

---

## Customize an existing codec

Some codecs can be eazily customized, this is the case for all the `baseX` codecs.

Look at `base64`'s [`customization`]() section to learn more about it.

## Write your own codec

`mgs` provides some building blocks to help you write your codec.

### basic_codec

```cpp
// Located in <mgs/codecs/basic_codec.hpp>

namespace mgs {
namespace codecs {
template <typename CodecTraits>
class basic_codec { /* ... */ };
}
}
```

This class will take care of defining, and properly constraining the following functions:

* `make_encoder`
* `make_decoder`
* `encode`
* `decode`

It expects a single template parameter, which must model the [`CodecTraits`]() concept.

### basic_transformed_input_adapter

```cpp
// Located in <mgs/adapters/basic_transformed_input_adapter.hpp>

namespace mgs {
namespace adapters {
template <typename InputTransformer>
class basic_transformed_input_adapter { /* ... */ };
}
}
```

This class will help you create encoders and decoders.

It expects a single template parameter, which must model the [`InputTransformer`]() concept.

Note
{: .label .label-blue }
If the template parameter also models [`SizedInputTransformer`](), `basic_transformed_input_adapter` will model [`SizedTransformedInputAdapter`]() as well.

### Example

A working example can be found in the test suite, in `codecs_base/test/test_codecs_base.cpp`.

It is a no-op codec, but it uses the previously mentioned building blocks and can be copy-pasted to get started easily.

## Quirks and details

### Iterator related optimizations

While codecs accepts every [`Iterable`]() (assuming it fulfills the constraints properly), some of them will perform better when the underlying iterators are [`RandomAccessIterator`]()s.

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

### Supported return types

Here is the list of supported `encode`/`decode` return types:

* `std::array<CharT, std::size_t>`
* [`SequenceContainer`]()s that are [`CopyConstructible`]() or [`MoveConstructible`]()
  * And either
    * [`Constructible`]() from an [`Iterator`]() range
  * Or
    * [`DefaultConstructible`]()
    * [`Iterable`](), with [`RandomAccessIterator`]()
    * Have a `size_type` member type alias
    * Have a `resize(size_type)` member function

