---
layout: default
title: Base64
nav_order: 2
parent: Codecs
---

# Base64
{: .no_toc }

Implementation of the Base64 codec, as defined in [RFC4648](https://tools.ietf.org/html/rfc4648).

## Table of contents
{: .no_toc .text_delta }

1. TOC
{: toc }

---

## Synopsis

Defined in header `<mgs/base64.hpp>`.

```cpp
class base64
```

## Example

```cpp
mgs::base64::encode("Hello, World!"); 
mgs::base64::decode("SGVsbG8sIFdvcmxkIQ==");
```

## Customization
