---
layout: default
title: New codec
nav_order: 2
parent: Write your own codec
permalink: /docs/wyoc/new
---

# New codec
{: .no_toc }
This section introduces building blocks that can be used to create codecs from scratch.

## Table of contents
{: .no_toc .text_delta }

1. TOC
{: toc }

---

## Write an input transformer

Input transformers are `mgs` lowest level constructs, they process an input range, and produce blocks of transformed output.

They must model [`InputTransformer`](/docs/concepts/input_transformer).

Let's write a no-op one, copying its input range in one go:

```cpp
class noop_transformer {
public:
  using underlying_iterator = char const*;
  using underlying_sentinel = char const*;
  using buffer_type = std::vector<char>;

  noop_transformer() = default;
  noop_transformer(char const* begin, char const* end): _current(begin), _end(end) {}

  void operator()(buffer_type& out) {
    out.resize(max_transformed_size());
    _current = std::copy(_current, _end, out.data());
  }

  std::size_t max_transformed_size() {
    return _end - _current;
  }

  auto iterator() const {
    return _current;
  }

  auto sentinel() const {
    return _end;
  }

private:
  char const* _current{};
  char const* _end{};
};
```

Note
{: .label .label-blue }
Although `noop_transformer` copies the whole input range in one go, it could very well copy small chunks instead.

## Write a transformed input range

Transformed input ranges allow iterating over transformed input.

`mgs` provides a building block to easily create one from an input transformer:

```cpp
#include <mgs/ranges/transformed_input_range.hpp>

using namespace mgs;

using noop_encoder = ranges::transformed_input_range<noop_transformer>;

int main() {
  std::string s("Hello, World!");
  noop_encoder nv(s.c_str(), s.c_str() + s.size());

  std::string s2(nv.begin(), nv.end()); // "Hello, World!"

  noop_encoder nv2(s.c_str(), s.c_str() + s.size());
  // will output "Hello, World!"
  nv2.read(std::ostreambuf_iterator<char>(std::cout), s.size());
}
```

`ranges::transformed_input_range` takes an [`InputTransformer`](/docs/concepts/input_transformer) and models [`ReadableTransformedInputRange`](/docs/concepts/readable_transformed_input_range).

If its template parameter models [`SizedInputTransformer`](/docs/concepts/sized_input_transformer), it will also model [`SizedTransformedInputRange`](/docs/concepts/sized_transformed_input_range).

## Define the codec traits

Now that we have our no-op encoder, we can almost create our codec.

We still have to write a small codec traits type, which must model [`CodecTraits`](/docs/concepts/codec_traits) and which will define: 

* default types returned by `encode` and `decode`
* `make_encoder` and `make_decoder` functions 

```cpp
struct noop_codec_traits {
  using default_encoded_output = std::string;
  using default_decoded_output = std::vector<char>;

  auto make_encoder(char const* begin, char const* end) {
    return noop_encoder(begin, end);
  }

  auto make_decoder(char const* begin, char const* end) {
    // not a typo, it is a no-op so no need to define a decoder
    return noop_encoder(begin, end);
  }
};
```

## Write your codec

Here we are, the last step!

```cpp
#include <mgs/codecs/basic_codec.hpp>

using namespace mgs;

using noop_codec = codecs::basic_codec<noop_codec_traits>;
```

`codecs::basic_codec` takes a [`CodecTraits`](/docs/concepts/codec_traits) template parameter and models [`Codec`](/docs/concepts/codec).

Congratulations, you can now use your no-op codec!

```cpp
int main() {
  std::string const s = noop_codec::encode("Hello, World"!);
  auto const s2 = noop_codec::decode<std::vector<char>>(s);
}
```
