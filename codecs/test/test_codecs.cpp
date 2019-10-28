#include <catch2/catch.hpp>
#include <iostream>

#include <algorithm>
#include <cstdint>
#include <deque>
#include <forward_list>
#include <list>
#include <string>
#include <utility>

#include <mgs/codecs/basic_codec.hpp>
#include <mgs/codecs/concepts/codec.hpp>
#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/output_traits.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/meta/static_asserts.hpp>
#include <mgs/ranges/basic_transformed_input_range.hpp>

#include <test_helpers/codec_helpers.hpp>
#include <test_helpers/noop_transformer.hpp>

using namespace mgs;
using namespace mgs::codecs;
using namespace std::string_literals;

namespace
{
struct invalid_type
{
};

struct valid_type
{
  std::vector<std::uint8_t> vec;

  auto begin()
  {
    return vec.begin();
  }

  auto end()
  {
    return vec.end();
  }

  auto begin() const
  {
    return vec.begin();
  }

  auto end() const
  {
    return vec.end();
  }
};

template <typename Iterator, typename Sentinel>
class noop_range : public mgs::ranges::basic_transformed_input_range<
                       test_helpers::noop_transformer<Iterator, Sentinel>>
{
public:
  using underlying_iterator = Iterator;
  using underlying_sentinel = Sentinel;

  using mgs::ranges::basic_transformed_input_range<
      test_helpers::noop_transformer<Iterator,
                                     Sentinel>>::basic_transformed_input_range;
};

struct noop_codec_traits
{
  using default_encoded_output = std::string;
  using default_decoded_output = std::string;

  template <typename Iterator, typename Sentinel>
  static auto make_encoder(Iterator begin, Sentinel end){
    return noop_range<Iterator, Sentinel>(std::move(begin), std::move(end));
  }

  template <typename Iterator, typename Sentinel>
  static auto make_decoder(Iterator begin, Sentinel end){
    return noop_range<Iterator, Sentinel>(std::move(begin), std::move(end));
  }
};

using noop_codec = mgs::codecs::basic_codec<noop_codec_traits>;
}

namespace mgs
{
namespace codecs
{
template <>
struct output_traits<valid_type>
{
  template <typename InputRange>
  static valid_type create(InputRange& range)
  {
    return {{range.begin(), range.end()}};
  }
};

template <typename T>
struct output_traits<std::vector<T>>
{
  template <typename InputRange>
  static std::vector<T> create(InputRange& range)
  {
    return {range.begin(), range.end()};
  }
};
}
}

static_assert(is_codec<noop_codec>::value, "");

TEST_CASE("codecs")
{
  SECTION("codec output")
  {
    std::array<char, 4> const input{'t', 'e', 's', 't'};

    noop_codec::decode<std::list<char>>(input);
    using Encoder = decltype(noop_codec::make_encoder(input.begin(), input.end()));

    static_assert(!is_codec_output<invalid_type, Encoder>::value, "");
    static_assert(is_codec_output<valid_type, Encoder>::value, "");

    SECTION("C arrays")
    {
      unsigned char tab[10] = {};
      char tab2[10] = {};
      char tab3[10] = "abcdefghi";

      SECTION("encode")
      {
        auto const v = noop_codec::encode(tab);
        auto const v2 = noop_codec::encode(tab2);
        auto const v3 = noop_codec::encode(tab3);
        auto const v4 = noop_codec::encode(static_cast<char*>(tab3));
        auto const v5 = noop_codec::encode("abcdefghi");

        CHECK(v.size() == 10);
        CHECK(v2.size() == 0);
        CHECK(v3.size() == 9);
        CHECK(v4.size() == 9);
        CHECK(v5.size() == 9);
      }

      SECTION("decode")
      {
        auto const v = noop_codec::decode(tab);
        auto const v2 = noop_codec::decode(tab2);
        auto const v3 = noop_codec::decode(tab3);
        auto const v4 = noop_codec::decode(static_cast<char*>(tab3));
        auto const v5 = noop_codec::decode("abcdefghi");

        CHECK(v.size() == 10);
        CHECK(v2.size() == 0);
        CHECK(v3.size() == 9);
        CHECK(v4.size() == 9);
        CHECK(v5.size() == 9);
      }
    }

    SECTION("User-defined types")
    {
      test_helpers::basic_codec_tests<noop_codec, valid_type>(input, input);
    }

    SECTION("Common tests")
    {
      test_helpers::basic_codec_tests<noop_codec>(input, input);

      test_helpers::test_std_containers<noop_codec>(input, input);
      test_helpers::test_input_streams<noop_codec>(input, input);
      test_helpers::test_back_and_forth<noop_codec>(input, input);
      test_helpers::test_encode_twice<noop_codec>(input, input);
    }

    SECTION("Array conversion")
    {
      CHECK_THROWS_AS((noop_codec::encode<std::array<char, 3>>(input)),
                      mgs::exceptions::unexpected_eof_error);
      CHECK_THROWS_AS((noop_codec::encode<std::array<char, 5>>(input)),
                      mgs::exceptions::unexpected_eof_error);
      CHECK_THROWS_AS((noop_codec::decode<std::array<char, 3>>(input)),
                      mgs::exceptions::unexpected_eof_error);
      CHECK_THROWS_AS((noop_codec::decode<std::array<char, 5>>(input)),
                      mgs::exceptions::unexpected_eof_error);
    }
  }
}
