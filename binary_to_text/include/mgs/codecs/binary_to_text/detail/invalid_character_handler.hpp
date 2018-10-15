#pragma once

#include <cassert>
#include <string>

#include <mgs/codecs/binary_to_text/detail/bitshift_traits.hpp>
#include <mgs/codecs/binary_to_text/detail/math.hpp>
#include <mgs/codecs/binary_to_text/padding_policy.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace binary_to_text
{
namespace detail
{
template <typename EncodingTraits,
          padding_policy = EncodingTraits::padding_policy>
struct invalid_character_handler
{
private:
  using BitshiftTraits = bitshift_traits<EncodingTraits>;

  template <typename Iterator, typename Sentinel>
  static void expect_padding_bytes(Iterator& current, Sentinel const end, int n)
  {
    assert(current != end);
    assert(n > 0);

    for (; n > 0; --n)
    {
      auto const c = static_cast<char>(*current++);
      if (c != EncodingTraits::padding_character)
      {
        using namespace std::string_literals;
        throw exceptions::invalid_input_error{"invalid encoded character: '"s +
                                              c + "'"};
      }
      if (current == end && n != 1)
      {
        throw exceptions::unexpected_eof_error{
            "unexpected end of encoded input"};
      }
    }
    if (current != end)
    {
      throw exceptions::invalid_input_error{"invalid encoded input"};
    }
  }

public:
  template <typename Iterator, typename Sentinel>
  static void handle(int i, char c, Iterator& current, Sentinel const sent)
  {
    if (c != EncodingTraits::padding_character)
    {
      using namespace std::string_literals;
      throw exceptions::invalid_input_error{"invalid encoded character: '"s +
                                            c + "'"};
    }
    // find out if padding character is at a correct position
    auto const res = std::div(i * BitshiftTraits::nb_index_bits, 8);
    if (res.quot == 0 || res.rem >= BitshiftTraits::nb_index_bits)
      throw exceptions::invalid_input_error{"invalid encoded input"};
    if (current != sent)
      expect_padding_bytes(current, sent, BitshiftTraits::nb_encoded_bytes - i - 1);
  }
};

template <typename EncodingTraits>
struct invalid_character_handler<EncodingTraits, padding_policy::none>
{
  template <typename Iterator, typename Sentinel>
  static void handle(int, char c, Iterator&, Sentinel const)
  {
    using namespace std::string_literals;
    throw exceptions::invalid_input_error{"invalid encoded character: '"s + c +
                                          "'"};
  }
};
}
}
}
}
}
