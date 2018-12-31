---
layout: default
title: Intermediate
nav_order: 3
parent: Usage
---

# Intermediate usage
{:.no_toc}

This section introduces `Encoder`s and `Decoder`s, and demonstrates how they are used in more advanced features than those shown in the [basic section](basic).

If you wish to write your own codec, or want to know more about the library details, check out the [advanced section](advanced) as well.

## Table of contents
{:.no_toc .text_delta}

1. TOC
{:toc}

---

## Encoders and Decoders

Every codec is built on top of an `Encoder` and a `Decoder`.

Both are stateful objects designed for one-time use, be careful to not reuse them!

They model the [`IterableTransformedInputAdapter`]() concept and thus share a common API, allowing lazy encoding/decoding:

1. `begin`/`end` member functions, each returning an [`InputIterator`]().
1. `read` member function, filling an [`OutputIterator`]() and returning the number of characters read.

To create them, use the codecs' `make_encoder`/`make_decoder` static member functions:

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  std::string const str("decoded");

  // 1. Using iterators
  auto encoder = base64::make_encoder(str.begin(), str.end());
  std::string const b64str(encoder.begin(), encoder.end());

  // 2. Using read
  auto decoder = base64::make_decoder(b64str.begin(), b64str.end());
  std::string decoded;
  while (auto nb_read = decoder.read(std::back_inserter(decoded), 256));
}
```

Note
{: .label .label-blue }

Although more verbose, `read` can be faster than iterators.

### Use-cases

#### Adding support for user-defined types

Although `mgs` supports lots of return types, you might use one in your code that is not supported by default.

As an example, here is the code needed to support [`QLinkedList`](http://doc.qt.io/qt-5/qlinkedlist.html):

```cpp
#include <mgs/codecs/output_traits_fwd.hpp>

#include <QLinkedList>

namespace mgs {
namespace codecs {
template <typename CharT>
struct output_traits<QLinkedList<CharT>> {
  // parameter is either an Encoder or a Decoder
  template <typename U>
  static QLinkedList<CharT> create(U& adapter) {
    QLinkedList<CharT> list;
    std::copy(adapter.begin(), adapter.end(), std::back_inserter(list));
    return list;
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

Some codecs provide two methods to predict the future transformed size:

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  auto const encoded_size = base64::encoded_size(42);
  auto const decoded_size = base64::max_decoded_size(encoded_size);
}
```

Caveat
{: .label .label-yellow }
`max_decoded_size` might throw `invalid_input_error` if the given encoded size is invalid.

e.g. `base64` encoded output is always a multiple of 4, so `base64::max_decoded_size(3)` will throw. 
