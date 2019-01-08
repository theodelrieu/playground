---
layout: default
title: Important Notions
nav_order: 3
permalink: /docs/important-notions
---

# Important Notions
{: .no_toc }

This section introduces key notions to this documentation's good comprehension.

## Table of contents
{: .no_toc .text-delta }

1. TOC
{: toc }

---

## Concepts

`mgs`'s design is based on [C++ concepts](https://en.cppreference.com/w/cpp/language/constraints).

Although they are not yet available, the documentation refers to lots of them.

Therefore, it is important to grasp some key elements of this C++20 feature.

### Constraining templates

The main point of concepts is to properly and (more) easily constrain template code.

The following function is unconstrained, only the template parameter name helps understanding the code intent:

```cpp
template <typename SignedIntegral>
void f(SignedIntegral i){}
```

Furthermore, passing an unsigned integral type will not yield any error... Now let's add the unsigned integral overload:

```cpp
template <typename UnsignedIntegral>
void f(UnsignedIntegral i){}
```

Unfortunately, this will yield a compiler error:

`error: redefinition of ‘template<class UnsignedIntegral> void f(UnsignedIntegral)’`

To solve this, we have to use some metaprogramming.

#### Pre-concepts era

Before C++20, we must use [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae).

```cpp
template <typename SignedIntegral,
          std::enable_if_t<std::is_integral_v<SignedIntegral> &&
                           std::is_signed_v<SignedIntegral>, int> = 0>
void f(SignedIntegral i){}

template <typename UnsignedIntegral,
          std::enable_if_t<std::is_integral_v<UnsignedIntegral> &&
                           !std::is_signed_v<UnsignedIntegral>, int> = 0>
void f(UnsignedIntegral i){}
```

It works... Now imagine what you have to write when you want to make sure a type support equality/inequality operators.

#### Embracing concepts

Here is the same code using concepts:

```cpp
template <std::SignedIntegral I>
void f(I i){}

template <std::UnsignedIntegral U>
void f(U u){}
```

Much better. The `std::enable_if` code is not needed anymore (it is sort of embedded in the concept language feature).

### Documentation and concepts

Two kinds of concepts are present in this documentation:

* Standard concepts
* `mgs` concepts

Standard concepts are prefixed with the `std::` prefix, and will redirect to their page on [cppreference](https://en.cppreference.com/w/cpp/concepts).

Concepts defined by `mgs` are not prefixed, and are documented [here](/docs/concepts).
