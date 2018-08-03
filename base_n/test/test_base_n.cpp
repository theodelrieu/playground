#include <algorithm>
#include <iostream>
#include <chrono>
#include <deque>
#include <forward_list>
#include <fstream>
#include <iterator>
#include <list>
#include <sstream>
#include <type_traits>
#include <vector>

#include <catch.hpp>

#include <mgs/base_n/basic_decoder.hpp>
#include <mgs/base_n/basic_encoder.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/meta/concepts/derived_from.hpp>
#include <mgs/meta/concepts/iterable_input_adapter.hpp>

#include <test_helpers/base_n.hpp>

using namespace std::string_literals;
using namespace mgs;

// namespace
// {
// template <std::size_t In, std::size_t Out>
// struct base2_encoding_traits
// {
//   using alphabet_t = char const[2];
//
//   static constexpr alphabet_t alphabet = {'0', '1'};
//
//   static constexpr char const encoding_name[] = "base2";
//   static constexpr auto const padding_policy = base_n::padding_policy::none;
//   static constexpr auto const nb_input_bytes = In;
//   static constexpr auto const nb_output_bytes = Out;
//
//   static auto find_char(char c)
//   {
//     return std::find(std::begin(alphabet), std::end(alphabet), c);
//   }
// };
//
// template <std::size_t In, std::size_t Out>
// constexpr typename base2_encoding_traits<In, Out>::alphabet_t
//     base2_encoding_traits<In, Out>::alphabet;
// template <std::size_t In, std::size_t Out>
// constexpr char const base2_encoding_traits<In, Out>::encoding_name[];
//
// template <typename Iterator, typename Sentinel = Iterator>
// using base2_encoder = adapters::transformer_adapter<
//     base_n::basic_encoder<base2_encoding_traits<1, 8>>,
//     Iterator,
//     Sentinel>;
//
// template <typename Iterator, typename Sentinel = Iterator>
// using base2_decoder = adapters::transformer_adapter<
//     base_n::basic_decoder<base2_encoding_traits<8, 1>>,
//     Iterator,
//     Sentinel>;
// }
//
// TEST_CASE("base2", "[base_n]")
// {
//   std::vector<std::string> decoded{"abcd"s, "abcde"s, "abcdef"s};
//   std::vector<std::string> encoded{
//       "01100001011000100110001101100100"s,
//       "0110000101100010011000110110010001100101"s,
//       "011000010110001001100011011001000110010101100110"s};
//
//   SECTION("encoding")
//   {
//     SECTION("common_checks")
//     {
//       common_checks<base2_encoder>(decoded, encoded);
//     }
//
//     SECTION("sentinel")
//     {
//       sentinel_check<base2_encoder>(decoded.back(), encoded.back());
//     }
//
//     SECTION("Inception")
//     {
//       inception_check<base2_encoder>(
//           "a"s,
//           "01100001"s,
//           "0011000000110001001100010011000000110000001100000011000000110001"s);
//     }
//   }
//
//   SECTION("decoding")
//   {
//     SECTION("common_checks")
//     {
//       common_checks<base2_decoder>(encoded, decoded);
//     }
//
//     SECTION("sentinel")
//     {
//       sentinel_check<base2_decoder>(encoded.back(), decoded.back());
//     }
//
//     SECTION("Inception")
//     {
//       inception_check<base2_decoder>(
//           "0011000000110001001100010011000000110000001100000011000000110001"s,
//           "01100001"s,
//           "a"s);
//     }
//   }
//
//   SECTION("back and forth")
//   {
//     SECTION("decode(encode())")
//     {
//       back_and_forth_check<base2_encoder, base2_decoder>("a"s);
//     }
//
//     SECTION("encode(decode())")
//     {
//       back_and_forth_check<base2_decoder, base2_encoder>(
//           "0011000000110001001100010011000000110000001100000011000000110001"s);
//     }
//   }
//
//   SECTION("invalid input")
//   {
//     std::vector<std::string> invalid_chars{"*"s, "2"s};
//     std::vector<std::string> invalid_eof{"1010101"s, "101010101"s};
//
//     invalid_input_checks<base2_decoder, mgs::exceptions::invalid_input_error>(
//         invalid_chars);
//     invalid_input_checks<base2_decoder, mgs::exceptions::unexpected_eof_error>(
//         invalid_eof);
//   }
// }
//
// namespace
// {
// template <std::size_t In, std::size_t Out>
// struct octal_encoding_traits
// {
//   using alphabet_t = char const[8];
//
//   static constexpr alphabet_t alphabet = {'0', '1', '2', '3', '4', '5', '6', '7'};
//
//   static constexpr char const encoding_name[] = "octal";
//   static constexpr auto const padding_policy = base_n::padding_policy::none;
//   static constexpr auto const nb_input_bytes = In;
//   static constexpr auto const nb_output_bytes = Out;
//
//   static auto find_char(char c)
//   {
//     return std::find(std::begin(alphabet), std::end(alphabet), c);
//   }
// };
//
// template <std::size_t In, std::size_t Out>
// constexpr typename octal_encoding_traits<In, Out>::alphabet_t
//     octal_encoding_traits<In, Out>::alphabet;
// template <std::size_t In, std::size_t Out>
// constexpr char const octal_encoding_traits<In, Out>::encoding_name[];
//
// template <typename Iterator, typename Sentinel = Iterator>
// using octal_encoder = adapters::transformer_adapter<
//     base_n::basic_encoder<octal_encoding_traits<1, 3>>,
//     Iterator,
//     Sentinel>;
//
// template <typename Iterator, typename Sentinel = Iterator>
// using octal_decoder = adapters::transformer_adapter<
//     base_n::basic_decoder<octal_encoding_traits<3, 1>>,
//     Iterator,
//     Sentinel>;
// }
//
// TEST_CASE("octal", "[base_n]")
// {
//   std::vector<std::string> decoded{"abcd"s, "abcde"s, "abcdef"s};
//   std::vector<std::string> encoded{
//       "141142143144"s, "141142143144145"s, "141142143144145146"s};
//
//   SECTION("encoding")
//   {
//     SECTION("common_checks")
//     {
//       common_checks<octal_encoder>(decoded, encoded);
//     }
//
//     SECTION("sentinel")
//     {
//       sentinel_check<octal_encoder>(decoded.back(), encoded.back());
//     }
//
//     SECTION("Inception")
//     {
//       inception_check<octal_encoder>("a"s, "141"s, "061064061"s);
//     }
//   }
//
//   SECTION("decoding")
//   {
//     SECTION("common_checks")
//     {
//       common_checks<octal_decoder>(encoded, decoded);
//     }
//
//     SECTION("sentinel")
//     {
//       sentinel_check<octal_decoder>(encoded.back(), decoded.back());
//     }
//
//     SECTION("Inception")
//     {
//       inception_check<octal_decoder>("061064061"s, "141"s, "a"s);
//     }
//   }
//
//   SECTION("back and forth")
//   {
//     SECTION("decode(encode())")
//     {
//       back_and_forth_check<octal_encoder, octal_decoder>("a"s);
//     }
//
//     SECTION("encode(decode())")
//     {
//       back_and_forth_check<octal_decoder, octal_encoder>("061064061"s);
//     }
//   }
//
//   SECTION("invalid input")
//   {
//     std::vector<std::string> invalid_chars{"*"s, "8"s};
//     std::vector<std::string> invalid_eof{"1010101"s, "10101011"s};
//
//     invalid_input_checks<octal_decoder, mgs::exceptions::invalid_input_error>(
//         invalid_chars);
//     invalid_input_checks<octal_decoder, mgs::exceptions::unexpected_eof_error>(
//         invalid_eof);
//   }
// }
//
namespace
{
template <std::size_t In, std::size_t Out>
struct diy_encoding_traits
{
  using alphabet_t = char const[4];

  static constexpr alphabet_t alphabet = {'O', 'U', 'I', 'N'};

  static constexpr char const encoding_name[] = "diy";
  static constexpr auto const padding_policy = base_n::padding_policy::none;
  static constexpr auto const nb_input_bytes = In;
  static constexpr auto const nb_output_bytes = Out;

  static auto find_char(char c)
  {
    return std::find(std::begin(alphabet), std::end(alphabet), c);
  }
};

template <std::size_t In, std::size_t Out>
constexpr typename diy_encoding_traits<In, Out>::alphabet_t
    diy_encoding_traits<In, Out>::alphabet;
template <std::size_t In, std::size_t Out>
constexpr char const diy_encoding_traits<In, Out>::encoding_name[];

template <typename Iterator, typename Sentinel = Iterator>
using diy_encoder = adapters::transformer_adapter<
    base_n::basic_encoder<diy_encoding_traits<1, 5>>,
    Iterator,
    Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using diy_decoder = adapters::transformer_adapter<
    base_n::basic_decoder<diy_encoding_traits<5, 1>>,
    Iterator,
    Sentinel>;
}

TEST_CASE("diy", "[base_n]")
{
  // static constexpr alphabet_t alphabet = {'O', 'U', 'I', 'N'};
  std::vector<std::string> decoded{"abcd"s, "abcde"s, "abcdef"s};
  std::vector<std::string> encoded{"OUIOUOUIOIOUIONOUIUO"s,
                                   "OUIOUOUIOIOUIONOUIUOOUIUU"s,
                                   "OUIOUOUIOIOUIONOUIUOOUIUUOUIUI"s};

  // 00 01 00 10 01
  SECTION("encoding")
  {
    SECTION("common_checks")
    {
      common_checks<diy_encoder>(decoded, encoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<diy_encoder>(decoded.back(), encoded.back());
    }

    SECTION("Inception")
    {
      inception_check<diy_encoder>("a"s, "OUIOU"s, "OUONNOUUUUOUOIUOUONNOUUUU"s);
    }
  }

  SECTION("decoding")
  {
    SECTION("common_checks")
    {
      common_checks<diy_decoder>(encoded, decoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<diy_decoder>(encoded.back(), decoded.back());
    }

    SECTION("Inception")
    {
      inception_check<diy_decoder>("OUONNOUUUUOUOIUOUONNOUUUU"s, "OUIOU"s, "a"s);
    }

    SECTION("first encoded char is not used")
    {
      auto const forged = "NUIOU"s;
      diy_decoder<std::string::const_iterator> dec(forged.begin(), forged.end());
      std::string s(dec.begin(), dec.end());
      CHECK(s == "a"s);
    }
  }

  SECTION("back and forth")
  {
    SECTION("decode(encode())")
    {
      back_and_forth_check<diy_encoder, diy_decoder>("a"s);
    }

    SECTION("encode(decode())")
    {
      back_and_forth_check<diy_decoder, diy_encoder>("OUIOU"s);
    }
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{"*"s, "Q"s};
    std::vector<std::string> invalid_eof{"OUIU"s, "OUIUUU"s};

    invalid_input_checks<diy_decoder, mgs::exceptions::invalid_input_error>(
        invalid_chars);
    invalid_input_checks<diy_decoder, mgs::exceptions::unexpected_eof_error>(
        invalid_eof);
  }
}
