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

Both can take the following parameters:

1. An [`Iterable`]()
1. An [`Iterator`]() range (more precisely: an [`InputIterator`]() and a [`Sentinel`]())
1. `std::istream&`

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  // 1. Iterable
  std::string decoded_str("decoded text");
  std::list<char> decoded_list(decoded_str.begin(), decoded_str.end());

  std::string encoded_str("ZGVjb2RlZCB0ZXh0");
  std::list<char> encoded_list(encoded_str.begin(), encoded_str.end());

  assert(base64::encode(decoded_str) == base64::encode(decoded_list));
  assert(base64::decode(encoded_str) == base64::decode(encoded_list));

  // 2. Iterator range
  char const* decoded_c_str = decoded_str.c_str();
  char const* encoded_c_str = encoded_str.c_str();

  assert(base64::encode(decoded_c_str, decoded_c_str + decoded_str.size()) ==
         base64::encode(decoded_list.begin(), decoded_list.end()));

  assert(base64::decode(encoded_c_str, encoded_c_str + encoded_str.size()) ==
         base64::decode(encoded_list.begin(), encoded_list.end()));

  // 3. Input stream
  std::ifstream decoded_file("decoded.txt");
  std::ifstream encoded_file("encoded.b64");

  auto const encoded_content = base64::encode(decoded_file);
  auto const decoded_content = base64::decode(encoded_file);
}
```

Caveat
{: .label .label-yellow }

Using the [`Iterable`]()(1) overloads with a `char[]` has a caveat:

It will stop encoding at the first encountered null character (`'\0'`).

If you want to avoid this behavior, use the [`Iterator`]() range overloads(2) instead.

Note
{: .label .label-blue }

More detail about input parameter requirements can be found [here](advanced).

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

Note
{: .label .label-blue }

More detail about return type requirements can be found [here](advanced).

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
