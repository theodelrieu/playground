#pragma once

#include <cassert>
#include <string>

#include <mgs/base_n/detail/math.hpp>
#include <mgs/base_n/padding_policy.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

namespace mgs
{
inline namespace v1
{
namespace base_n
{
namespace detail
{
template <typename EncodingTraits,
          padding_policy = EncodingTraits::padding_policy>
struct invalid_character_handler
{
private:
  static constexpr auto nb_output_bytes = EncodingTraits::nb_output_bytes;
  static constexpr auto nb_input_bytes = EncodingTraits::nb_input_bytes;

  static constexpr auto nb_output_bits =
      detail::round_to_multiple_of<nb_output_bytes * 8, nb_input_bytes>();
  static constexpr auto nb_encoded_bits = nb_output_bits / nb_input_bytes;

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
        throw exceptions::invalid_input_error{
            EncodingTraits::encoding_name,
            "invalid encoded character: '"s + c + "'"};
      }
      if (current == end && n != 1)
      {
        throw exceptions::unexpected_eof_error{
            EncodingTraits::encoding_name, "unexpected end of encoded input"};
      }
    }
    if (current != end)
    {
      throw exceptions::invalid_input_error{EncodingTraits::encoding_name,
                                            "invalid encoded input"};
    }
  }

public:
  template <typename Iterator, typename Sentinel>
  static void handle(int i, char c, Iterator& current, Sentinel const sent)
  {
    if (c != EncodingTraits::padding_character)
    {
      using namespace std::string_literals;
      throw exceptions::invalid_input_error{
          EncodingTraits::encoding_name,
          "invalid encoded character: '"s + c + "'"};
    }
    // find out if padding character is at a correct position
    auto const res = std::div(i * nb_encoded_bits, 8);
    if (res.quot == 0 || res.rem >= nb_encoded_bits)
    {
      throw exceptions::invalid_input_error{EncodingTraits::encoding_name,
                                            "invalid encoded input"};
    }
    if (current != sent)
      expect_padding_bytes(current, sent, nb_input_bytes - i - 1);
  }
};

template <typename EncodingTraits>
struct invalid_character_handler<EncodingTraits, padding_policy::none>
{
  template <typename Iterator, typename Sentinel>
  static void handle(int, char c, Iterator&, Sentinel const)
  {
    using namespace std::string_literals;
    throw exceptions::invalid_input_error{
        EncodingTraits::encoding_name,
        "invalid encoded character: '"s + c + "'"};
  }
};
}
}
}
}
