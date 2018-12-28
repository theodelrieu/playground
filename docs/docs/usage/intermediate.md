---
layout: default
title: Intermediate
nav_order: 3
parent: Usage
---

# Intermediate usage
{:.no_toc}

This section introduces Encoders and Decoders, and demonstrates how they are used in more advanced features than those shown in the [basic section](basic).

If you wish to write your own codec, or want to know more about the library details, check out the [advanced section](advanced) as well.

## Table of contents
{:.no_toc .text_delta}

1. TOC
{:toc}

---

## Encoders and Decoders

Every codec is built on top of an Encoder and a Decoder.

Both model the [`IterableTransformedInputAdapter`]() concept and thus share a common API:

1. `begin`/`end` member functions, each returning an [`InputIterator`]().
2. `write` member function, returning the number of characters written.

To create them, use the codecs' `make_encoder`/`make_decoder` static member functions:

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  std::string const str("decoded");

  // 1. Using iterators
  auto encoder = base64::make_encoder(str.begin(), str.end());
  std::string const b64str(encoder.begin(), encoder.end());

  // 2. Using write
  auto decoder = base64::make_decoder(b64str.begin(), b64str.end());
  std::string decoded;
  auto nb_written = decoder.write(std::back_inserter(decoded), 256);
  while (nb_written != 0)
    nb_written = decoder.write(std::back_inserter(decoded), 256);
}
```

Note
{: .label .label-blue }
Both ways will lazily transform the input.

Although more verbose, `write` is often faster than using iterators.

Caveat
{: .label .label-yellow }
Encoders and Decoders are stateful objects designed for one-time use, be careful to not reuse them!

### Use-cases

#### Adding support for user-defined types

Although `mgs` supports lots of return types, you might use one in your code that is not supported by default.

Here is the code needed to support `my_container`:

```cpp
#include <mgs/codecs/output_traits_fwd.hpp>

namespace mgs {
namespace codecs {
template <>
struct output_traits<my_container> {
  template <typename IterableTransformedInputAdapter>
  static my_container create(IterableTransformedInputAdapter&) {
    // The parameter is either an Encoder or a Decoder.
    // You can use iterators or the write member function.

    // ...
  }
};
}
}
```

Note
{: .label .label-blue }
`output_traits` has a second template parameter, defaulted to `void`, to allow SFINAE checks. 

#### Limiting memory usage

When dealing with huge volumes of data to encode/decode, memory consumption can become a problem.

Using the API shown in the [basic section](basic) demonstrates it well:

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  // Could be a few TB...
  std::ifstream enormous_file("TODO.txt");

  // oops
  auto encoded = base64::encode(enormous_file);

  std::ofstream ofs("TODO.b64");
  ofs.write(encoded.c_str(), encoded.size());
}
```

In this case, `std::bad_alloc` will likely be thrown by `std::string`'s constructor.

Here is how to handle it properly:

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  std::ifstream enormous_file("TODO.txt");
  std::istreambuf_iterator<char> begin(enormous_file);
  std::istreambuf_iterator<char> end;

  auto encoder = base64::make_encoder(std::istreambuf_iterator<char>(enormous_file),
                                      std::istreambuf_iterator<char>());

  std::ofstream ofs("TODO.b64");
  std::copy(encoder.begin(), encoder.end(), std::ostreambuf_iterator<char>(ofs));
}
```

## Predicting encoded/decoded size

lol
