---
layout: default
title: New codec
nav_order: 2
parent: Write your own codec
permalink: /docs/wyoc/new
---

# New codec
{: .no_toc }

## Table of contents
{: .no_toc .text_delta }

1. TOC
{: toc }

---

If you cannot/do not want to create a codec variant, you can either start from scratch, or use more abstract building blocks provided by `mgs`.

## Write an input transformer

Input transformers are `mgs` lowest level constructs, they process an input range, and produces blocks of transformed output.

They must model [`InputTransformer`](/docs/concepts/input_transformer).

Let's write a no-op one, copying its input range in one go:

```cpp
class noop_transformer
{
public:
  using underlying_iterator = char const*;
  using underlying_sentinel = char const*;
  using buffer_type = std::vector<char>;

  noop_transformer() = default;
  noop_transformer(char const* begin, char const* end): _current(begin), _end(end)
  {
  }

  void operator()(buffer_type& out)
  {
    out.resize(max_transformed_size());
    _current = std::copy(_current, _end, out.data());
  }

  std::size_t max_transformed_size()
  {
    return _end - _current;
  }

  auto iterator() const
  {
    return _current;
  }

  auto sentinel() const
  {
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

using noop_range = ranges::transformed_input_range<noop_transformer>;

int main() {
  std::string s("Hello, World!");
  noop_range nv(s.c_str(), s.c_str() + s.size());
  auto nv2 = nv;

  std::string s2(nv.begin(), nv.end()); // "Hello, World!"

  // will output "Hello, World!"
  nv2.read(std::ostreambuf_iterator<char>(std::cout), s.size());
}
```

`ranges::transformed_input_range` takes an [`InputTransformer`](/docs/concepts/input_transformer) and models [`ReadableTransformedInputRange`](/docs/concepts/readable_transformed_input_range).

If its template parameter models [`SizedInputTransformer`](/docs/concepts/sized_input_transformer), it will also model [`SizedTransformedInputRange`](/docs/concepts/sized_transformed_input_range).

## Define the codec traits

TODO

## Write your codec

Here we are, the final step! TODO

<!-- #### basic_codec -->
<!--  -->
<!-- `mgs::codecs::basic_codec<T>` is a helper class which defines, and properly constrains the following functions: -->
<!--  -->
<!-- * `make_encoder` -->
<!-- * `make_decoder` -->
<!-- * `encode` -->
<!-- * `decode` -->
<!--  -->
<!-- It expects a single template parameter, which must model the [`CodecTraits`]() concept. -->
<!--  -->
<!-- #### basic_transformed_input_adapter -->
<!--  -->
<!-- `mgs::adapters::basic_transformed_input_adapter<T>` is a helper class which defines functions and type aliases required by the [`IterableTransformedInputAdapter`]() concept. -->
<!-- The most important being: -->
<!--  -->
<!-- * `read` -->
<!-- * `begin` -->
<!-- * `end` -->
<!-- * `operator==`/`operator!=` -->
<!--  -->
<!-- This class will help you create `Encoder`s and `Decoder`s. -->
<!--  -->
<!-- It expects a single template parameter, which must model the [`InputTransformer`]() concept. -->
<!--  -->
<!-- Note -->
<!-- {: .label .label-blue } -->
<!-- If the template parameter also models [`SizedInputTransformer`](), `basic_transformed_input_adapter` will model [`SizedTransformedInputAdapter`]() as well. -->
<!--  -->
<!-- #### Example -->
<!--  -->
<!-- Here is a working example inspired from `mgs`' test suite. -->
<!--  -->
<!-- It is a no-op codec, but it uses the previously mentioned building blocks and can be copy-pasted to get started easily. -->
<!--  -->
<!-- ```cpp -->
<!-- #include <mgs/adapters/basic_transformed_input_adapter.hpp> -->
<!-- #include <mgs/codecs/basic_codec.hpp> -->
<!--  -->
<!-- class noop_transformer -->
<!-- { -->
<!-- public: -->
<!--   using underlying_iterator = char const*; -->
<!--   using underlying_sentinel = char const*; -->
<!--   using buffer_type = std::vector<char>; -->
<!--  -->
<!--   noop_transformer() = default; -->
<!--   noop_transformer(char const* begin, char const* end) -->
<!--     : _current(begin), _end(end) -->
<!--   { -->
<!--   } -->
<!--  -->
<!--   void operator()(buffer_type& out) -->
<!--   { -->
<!--     out.clear(); -->
<!--     while (_current != _end) -->
<!--       out.push_back(*_current++); -->
<!--   } -->
<!--  -->
<!--   std::size_t max_transformed_size() const -->
<!--   { -->
<!--     return _end - _current; -->
<!--   } -->
<!--  -->
<!-- private: -->
<!--   char const* _current{}; -->
<!--   char const* _end{}; -->
<!--  -->
<!--   friend bool operator==(noop_transformer const& lhs, -->
<!--                          noop_transformer const& rhs) -->
<!--   { -->
<!--     // A default-constructed value is a valid sentinel. -->
<!--     // As is the usual past-the-end iterator. Must check both cases. -->
<!--     if (lhs._current == lhs._end || rhs._current == rhs._end) -->
<!--       return lhs._current == lhs._end && rhs._current == rhs._end; -->
<!--     return lhs._current == rhs._current; -->
<!--   } -->
<!-- }; -->
<!--  -->
<!-- bool operator!=(noop_transformer const& lhs, -->
<!--                 noop_transformer const& rhs) -->
<!-- { -->
<!--   return !(lhs == rhs); -->
<!-- } -->
<!--  -->
<!-- using noop_adapter = mgs::adapters::basic_transformed_input_adapter<noop_transformer>; -->
<!--  -->
<!-- struct noop_codec_traits -->
<!-- { -->
<!--   using default_encoded_output = std::string; -->
<!--   using default_decoded_output = std::string; -->
<!--  -->
<!--   static auto make_encoder(char const* begin, char const* end){ -->
<!--     return noop_adapter(begin, end); -->
<!--   } -->
<!--  -->
<!--   static auto make_decoder(char const* begin, char const* end){ -->
<!--     return noop_adapter(begin, end); -->
<!--   } -->
<!-- }; -->
<!--  -->
<!-- using noop_codec = mgs::codecs::basic_codec<noop_codec_traits>; -->
<!--  -->
<!-- int main() { -->
<!--   auto str = noop_codec::encode("Hello, World!"); -->
<!--   // str == "Hello, World!" -->
<!-- } -->
<!-- ``` -->
