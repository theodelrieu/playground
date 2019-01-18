#include <algorithm>
#include <chrono>
#include <deque>
#include <forward_list>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <type_traits>
#include <vector>

#include <catch.hpp>

#include <mgs/codecs/binary_to_text/basic_codec.hpp>
#include <mgs/codecs/binary_to_text/basic_decoder.hpp>
#include <mgs/codecs/binary_to_text/basic_encoder.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/meta/concepts/core/derived_from.hpp>

#include <test_helpers/codec_helpers.hpp>

using namespace std::string_literals;
using namespace mgs;
using namespace mgs::codecs;

namespace
{
struct base2_encoding_traits
{
  using alphabet_t = char const[2];

  static constexpr alphabet_t alphabet = {'0', '1'};

  static constexpr auto const padding_policy =
      binary_to_text::padding_policy::none;

  static int index_of(char c)
  {
    auto const begin = std::begin(alphabet);
    auto const end = std::end(alphabet);
    auto it = std::find(begin, end, c);
    if (it == end)
      return -1;
    return std::distance(begin, it);
  }
};

constexpr base2_encoding_traits::alphabet_t base2_encoding_traits::alphabet;

using base2 = binary_to_text::basic_codec<base2_encoding_traits>;

template <typename I, typename S = I>
using base2_encoder = typename base2::template encoder<I, S>;

template <typename I, typename S = I>
using base2_decoder = typename base2::template decoder<I, S>;
}

TEST_CASE("base2", "[binary_to_text]")
{
  SECTION("Common tests")
  {
    auto const decoded = "abcdef"s;
    auto const encoded = "011000010110001001100011011001000110010101100110"s;
    test_helpers::basic_codec_tests<base2>(decoded, encoded);

    test_helpers::test_std_containers<base2>(decoded, encoded);
    test_helpers::test_input_streams<base2>(decoded, encoded);
    test_helpers::test_back_and_forth<base2>(decoded, encoded);
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{"***====="s, "23423423"s};
    std::vector<std::string> invalid_eof{"1010101"s, "101010101"s};

    for (auto const& elem : invalid_chars)
    {
      CHECK_THROWS_AS(base2::decode(elem),
                      mgs::exceptions::invalid_input_error);
    }

    for (auto const& elem : invalid_eof)
    {
      CHECK_THROWS_AS(base2::decode(elem),
                      mgs::exceptions::unexpected_eof_error);
    }
  }

  SECTION("basic_codec")
  {
    SECTION("encoded_size")
    {
      CHECK(base2::encoded_size(0) == 0);
      CHECK(base2::encoded_size(1) == 8);
      CHECK(base2::encoded_size(2) == 16);
      CHECK(base2::encoded_size(8) == 64);
    }

    SECTION("max_decoded_size")
    {
      CHECK(base2::max_decoded_size(0) == 0);
      CHECK(base2::max_decoded_size(8) == 1);
      CHECK(base2::max_decoded_size(64) == 8);

      CHECK_THROWS_AS(base2::max_decoded_size(1),
                      mgs::exceptions::invalid_input_error);
      CHECK_THROWS_AS(base2::max_decoded_size(2),
                      mgs::exceptions::invalid_input_error);
      CHECK_THROWS_AS(base2::max_decoded_size(3),
                      mgs::exceptions::invalid_input_error);
      CHECK_THROWS_AS(base2::max_decoded_size(4),
                      mgs::exceptions::invalid_input_error);
      CHECK_THROWS_AS(base2::max_decoded_size(5),
                      mgs::exceptions::invalid_input_error);
      CHECK_THROWS_AS(base2::max_decoded_size(6),
                      mgs::exceptions::invalid_input_error);
      CHECK_THROWS_AS(base2::max_decoded_size(7),
                      mgs::exceptions::invalid_input_error);
    }

    SECTION("read")
    {
      auto const str = "abcd"s;
      auto const encoded_str = "01100001011000100110001101100100"s;
      auto enc = base2::make_encoder(str.begin(), str.end());
      REQUIRE(base2::encode(str) == encoded_str);

      std::string encoded;
      auto nb_read = enc.read(std::back_inserter(encoded), 2);
      CHECK(nb_read == 2);
      CHECK(encoded == "01"s);
      nb_read = enc.read(std::back_inserter(encoded), 1000);
      CHECK(nb_read == encoded_str.size() - 2);
      CHECK(encoded == encoded_str);
    }
  }
}
