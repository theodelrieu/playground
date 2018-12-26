---
layout: default
title: Intermediate
nav_order: 3
parent: Usage
---

# Intermediate usage
{:.no_toc}

This section is for users having particular needs, e.g. low memory consumption.

It is assumed the reader has seen the [basic section](basic).


## Table of contents
{:.no_toc .text_delta}

1. TOC
{:toc}

---

## Eager vs. lazy

The examples shown in the [previous section](basic) are eager, which means encoding a huge input will require allocating an even bigger chunk of memory.

This can have undesirable effects (e.g. `std::bad_alloc` being thrown).

However, this does not mean codecs are eager. Some of them are lazy internally, they are made eager because of the high level API.

### Lazy constructs

Currently, every `baseX` codec is implemented on top of lazy constructs, namely one `encoder` and one `decoder`.

It is possible to use one of the following APIs instead of the high level one:

1. `encoder`/`decoder` iterators.
2. `write` member function.

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  std::vector<std::uint8_t> const huge_buffer = /* ... */

  // requires an Iterator/Sentinel pair (which must outlives the encoder!)
  auto encoder = base64::make_encoder(huge_buffer.begin(), huge_buffer.end());

  auto output_iterator = std::ostream_iterator<char>(std::cout);

  // 1. Less boilerplate, can be less efficient than 2.
  std::copy(encoder.begin(), encoder.end(), output_iterator);

  // 2. More boilerplate, but can be more efficient than 1.
  auto nb_written = encoder.write(output_iterator, 4096);
  while (nb_written)
    nb_written = encoder.write(output_iterator, 4096);
}
```

TODO explain what the code does, and what happens under the hood

Caveat
{: .label .label-yellow }

Only one of the following ways must be used!
The `encoder`/`decoder` are stateful, therefore it will be empty after having encoded the underlying data.

Note
{: .label .label-blue }

Both `encoder` and `decoder` model the [`IterableTransformedInputAdapter`]() concept.



