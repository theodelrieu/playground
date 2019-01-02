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

If this does not suit your needs, check out the [intermediate section](intermediate).

## Table of contents
{:.no_toc .text_delta}

1. TOC
{:toc}

---

## Encoding & Decoding

Each codec has two symmetrical APIs: `encode` and `decode`.

### Input parameters

Codecs share a common set of contraints on `encode`/`decode` input parameters. They can be one of the following:

1. [`Iterable`]()
1. [`Iterator`]() range (more precisely: an [`InputIterator`]() and a [`Sentinel`]())
1. `std::istream&`

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  // 1. Iterable
  std::string const decoded_str("decoded text");
  std::string const encoded_str("ZGVjb2RlZCB0ZXh0");
  std::list<char> const encoded_list(encoded_str.begin(), encoded_str.end());

  auto encoded = base64::encode(decoded_str);
  auto decoded = base64::decode(encoded_list);

  // 2. Iterator range
  char const* decoded_c_str = decoded_str.c_str();
  char const* encoded_c_str = encoded_str.c_str();

  encoded = base64::encode(decoded_c_str, decoded_c_str + decoded_str.size());
  decoded = base64::decode(encoded_c_str, encoded_c_str + encoded_str.size());

  // 3. Input stream
  std::ifstream decoded_file("decoded.txt");
  std::ifstream encoded_file("encoded.b64");

  encoded = base64::encode(decoded_file);
  decoded = base64::decode(encoded_file);
}
```

Caveat
{: .label .label-yellow }

Using the [`Iterable`]()(1) overloads with a `char[]` has a caveat:

It will discard the last character if it is the null terminator (`'\0'`).

If you want to avoid this behavior, use the [`Iterator`]() range overloads(2) instead.

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

  // For base64:
  // decltype(encoded) -> std::string
  // decltype(decoded) -> std::vector<std::uint8_t>

  auto const encoded_vec = base64::encode<std::vector<char>>("decoded text");
  auto const decoded_list = base64::decode<std::list<std::uint8_t>>(encoded_vec);
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
