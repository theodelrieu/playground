---
# Feel free to add content and custom Front Matter to this file.
# To modify the layout, see https://jekyllrb.com/docs/themes/#overriding-theme-defaults

layout: default
title: Introduction
nav_order: 1
permalink: /
---

# Introduction

Mgs is a C++14 codec library.

Its main design goals are:

* Ease of use
* Extensibility
* API genericity
* Package manager friendliness

## What is a codec?

{:.text-delta}
Wikipedia:

> A codec is a [...] computer program for encoding or decoding a digital data stream [...].
>
> `Codec` is a portmanteau of coder-decoder. 

Some famous codecs: `base64`, `FLAC`, `zip`.

## Why should I use Mgs?

Mgs defines a common interface for all codecs, that is both generic and customizable.

```cpp
#include <mgs/base64.hpp>

using namespace mgs;

int main() {
  // Parameter must be Iterable
  std::string const a = base64::encode("Hello, World!");
  std::vector<std::uint8_t> const b = base64::decode(encoded);

  // Default return types can be overriden
  auto const c = base64::encode<std::forward_list<char>>(b);
  auto const d = base64::decode<std::array<char, 13>>(c);

  // Iterator ranges are supported
  auto const e = base64::encode(d.begin(), d.end());
  auto const f = base64::encode(e.begin(), e.end());
}
```

This should cover most people's needs.
More advanced use-cases are discussed in later sections.

## Requirements

Mgs is header-only, it only requires a C++14 compiler.
