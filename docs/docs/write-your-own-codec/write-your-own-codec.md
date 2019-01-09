TODO split
## Write your own codec

There are two possible use-cases when writing a codec:

1. Write a variant of an existing one
1. Write a new codec

`mgs` provides building blocks for both cases.

### Codec variant

While codec variants cannot be customized (e.g. `base64` has no template parameters), the lower level constructs can.

#### BaseX codecs

Every `BaseX` codec (e.g. [`base64`](), [`base32`]() etc...) uses the same generic implementation: `mgs::binary_to_base::basic_codec`:

```cpp
// Header <mgs/codecs/binary_to_base/basic_codec.hpp>

template <typename EncodingTraits, typename DecodingTraits = EncodingTraits>
class basic_codec;
```

This class inherits from [`mgs::codecs::basic_codec`]() and thus defines the same member functions.

By using this helper class, you can easily create a Base64-like codec (e.g. using a different alphabet, without padding).

To do so, both template parameters must model the [`EncodingTraits`]() concept.

### New codec

If you cannot/do not want to create a codec variant, you can use more abstract building blocks.

#### basic_codec

`mgs::codecs::basic_codec<T>` is a helper class which defines, and properly constrains the following functions:

* `make_encoder`
* `make_decoder`
* `encode`
* `decode`

It expects a single template parameter, which must model the [`CodecTraits`]() concept.

#### basic_transformed_input_adapter

`mgs::adapters::basic_transformed_input_adapter<T>` is a helper class which defines functions and type aliases required by the [`IterableTransformedInputAdapter`]() concept.
The most important being:

* `read`
* `begin`
* `end`
* `operator==`/`operator!=`

This class will help you create `Encoder`s and `Decoder`s.

It expects a single template parameter, which must model the [`InputTransformer`]() concept.

Note
{: .label .label-blue }
If the template parameter also models [`SizedInputTransformer`](), `basic_transformed_input_adapter` will model [`SizedTransformedInputAdapter`]() as well.

#### Example

Here is a working example inspired from `mgs`' test suite.

It is a no-op codec, but it uses the previously mentioned building blocks and can be copy-pasted to get started easily.

```cpp
#include <mgs/adapters/basic_transformed_input_adapter.hpp>
#include <mgs/codecs/basic_codec.hpp>

class noop_transformer
{
public:
  using underlying_iterator = char const*;
  using underlying_sentinel = char const*;
  using buffer_type = std::vector<char>;

  noop_transformer() = default;
  noop_transformer(char const* begin, char const* end)
    : _current(begin), _end(end)
  {
  }

  void operator()(buffer_type& out)
  {
    out.clear();
    while (_current != _end)
      out.push_back(*_current++);
  }

  std::size_t max_transformed_size() const
  {
    return _end - _current;
  }

private:
  char const* _current{};
  char const* _end{};

  friend bool operator==(noop_transformer const& lhs,
                         noop_transformer const& rhs)
  {
    // A default-constructed value is a valid sentinel.
    // As is the usual past-the-end iterator. Must check both cases.
    if (lhs._current == lhs._end || rhs._current == rhs._end)
      return lhs._current == lhs._end && rhs._current == rhs._end;
    return lhs._current == rhs._current;
  }
};

bool operator!=(noop_transformer const& lhs,
                noop_transformer const& rhs)
{
  return !(lhs == rhs);
}

using noop_adapter = mgs::adapters::basic_transformed_input_adapter<noop_transformer>;

struct noop_codec_traits
{
  using default_encoded_output = std::string;
  using default_decoded_output = std::string;

  static auto make_encoder(char const* begin, char const* end){
    return noop_adapter(begin, end);
  }

  static auto make_decoder(char const* begin, char const* end){
    return noop_adapter(begin, end);
  }
};

using noop_codec = mgs::codecs::basic_codec<noop_codec_traits>;

int main() {
  auto str = noop_codec::encode("Hello, World!");
  // str == "Hello, World!"
}
```
