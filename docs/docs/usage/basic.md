---
layout: default
title: Basic
nav_order: 2
parent: Usage
---

# Basic usage
{:.no_toc}

This section demonstrates the library's most common usage.

It is sufficient for users wanting to encode and decode data with no particular constraints.

## Table of contents
{:.no_toc .text_delta}

1. TOC
{:toc}

---

## Encoding & Decoding

Each codec has two symmetrical APIs: `encode` and `decode`.

### Input parameters

Codecs share a common set of contraints on `encode`/`decode` input parameters. They can be one of the following:

1. [`std::Range`]()
1. [`std::Iterator`]() range (more precisely: an [`std::InputIterator`]() and a [`std::Sentinel`]())
1. `char*`

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  // 1. Range
  std::string const decoded_str("decoded text");
  std::string const encoded_str("ZGVjb2RlZCB0ZXh0");
  std::list<char> const encoded_list(encoded_str.begin(), encoded_str.end());
  std::list<char> const decoded_list(decoded_str.begin(), decoded_str.end());

  auto encoded = base64::encode(decoded_str);
  auto decoded = base64::decode(encoded_list);
  // decltype(encoded) -> std::string
  // decltype(decoded) -> std::vector<unsigned char>

  // 2. Iterator range
  encoded = base64::encode(decoded_list.begin(), decoded_list.end());
  decoded = base64::decode(encoded_list.begin(), encoded_list.end());

  // 3. char*
  encoded = base64::encode("decoded text");
  decoded = base64::decode(encoded.c_str());
}
```

Caveat
{: .label .label-yellow }

Passing a `char[]` to `encode/decode` will have the same effect than passing a `char*`. It will encode/decode upon the first encountered null terminator (`'\0'`).

If you want to avoid this behavior, use the [`std::Iterator`]() range overloads(2) instead.

Note
{: .label .label-blue }

Codecs can have additional constraints, which are described in each codec's section (e.g. [`base64`]()).

### Return types

Each codec defines two default return types. One for `encode`, one for `decode`.

Both can be overriden:

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  auto const encoded = base64::encode("decoded text");
  auto const decoded = base64::decode(encoded);

  // decltype(encoded) -> std::string const
  // decltype(decoded) -> std::vector<unsigned char> const

  auto const encoded_vec = base64::encode<std::vector<char>>("decoded text");
  auto const decoded_list = base64::decode<std::list<unsigned char>>(encoded_vec);
}
```

Default types are listed in each codec's page (e.g. [`base64`]()).

Note
{: .label .label-blue }

You can find the list of supported return types [here]().

If a type you wish to use is not supported by default, take a look [here]().

## Error handling

As of now, `mgs` reports errors with exceptions.

Every exception resides in the namespace `mgs::exceptions`.

| class                  | derived from             | description                          |
|------------------------+--------------------------+--------------------------------------|
| `exception`            | [`std::runtime_error`]() | Base class for every `mgs` exception |
| `decode_error`         | `exception`              | Base class for decoding exceptions   |
| `invalid_input_error`  | `decode_error`           | Thrown on invalid encoded input      |
| `unexpected_eof_error` | `decode_error`           | Thrown when more input was expected  |

Note
{: .label .label-yellow }

I am well aware that using exceptions will put off users who disable them.
I would like to support this use-case in future versions.

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
