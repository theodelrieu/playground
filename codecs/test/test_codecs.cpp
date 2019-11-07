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

#include <test_helpers/codec_helpers.hpp>

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

struct valid_codec_traits
{
  using default_encoded_output = std::string;
  using default_decoded_output = std::string;

  template <typename I, typename S>
  // only works thanks to input_source_view being copyable
  static auto make_encoder(input_source_view<I, S>& is)
  {
    return is;
  }

  template <typename I, typename S>
  static auto make_decoder(input_source_view<I, S>& is)
  {
    return is;
  }

  template <typename I, typename S>
  static input_source_view<I, S> make_encoder(I i, S s)
  {
    return make_input_source_view(i, s);
  }

  template <typename I, typename S>
  static input_source_view<I, S> make_decoder(I i, S s)
  {
    return make_input_source_view(i, s);
  }

  template <typename R>
  static auto make_encoder(R& r)
  {
    return make_input_source_view(r);
  }

  template <typename R>
  static auto make_decoder(R& r)
  {
    return make_input_source_view(r);
  }
  // FIXME remove test_helpers and put everything in codecs
};

using valid_codec = codecs::basic_codec<valid_codec_traits>;
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

// auto _ = meta::trigger_static_asserts<is_codec<valid_codec>>();

TEST_CASE("codecs")
{
  SECTION("codec output")
  {
    std::array<char, 4> const input{'t', 'e', 's', 't'};

    using Encoder = decltype(valid_codec::traits::make_encoder(
        std::declval<input_source_view<decltype(input.begin())>&>()));

    static_assert(!is_codec_output<invalid_type, Encoder>::value, "");
    static_assert(is_codec_output<valid_type, Encoder>::value, "");

    SECTION("C arrays")
    {
      unsigned char tab[10] = {};
      char tab2[10] = {};
      char tab3[10] = "abcdefghi";

      SECTION("encode")
      {
        auto const v = valid_codec::encode(tab);
        auto const v2 = valid_codec::encode(tab2);
        auto const v3 = valid_codec::encode(tab3);
        auto const v5 = valid_codec::encode("abcdefghi");

        CHECK(v.size() == 10);
        CHECK(v2.size() == 0);
        CHECK(v3.size() == 9);
        CHECK(v5.size() == 9);
      }

      SECTION("decode")
      {
        auto const v = valid_codec::decode(tab);
        auto const v2 = valid_codec::decode(tab2);
        auto const v3 = valid_codec::decode(tab3);
        auto const v5 = valid_codec::decode("abcdefghi");

        CHECK(v.size() == 10);
        CHECK(v2.size() == 0);
        CHECK(v3.size() == 9);
        CHECK(v5.size() == 9);
      }
    }

    SECTION("User-defined types")
    {
      test_helpers::basic_codec_tests<valid_codec, valid_type>(input, input);
    }

    SECTION("Common tests")
    {
      test_helpers::basic_codec_tests<valid_codec>(input, input);

      test_helpers::test_std_containers<valid_codec>(input, input);
      test_helpers::test_input_streams<valid_codec>(input, input);
      test_helpers::test_back_and_forth<valid_codec>(input, input);
      test_helpers::test_encode_twice<valid_codec>(input, input);
    }

    SECTION("Array conversion")
    {
      CHECK_THROWS_AS((valid_codec::encode<std::array<char, 3>>(input)),
                      mgs::exceptions::unexpected_eof_error);
      CHECK_THROWS_AS((valid_codec::encode<std::array<char, 5>>(input)),
                      mgs::exceptions::unexpected_eof_error);
      CHECK_THROWS_AS((valid_codec::decode<std::array<char, 3>>(input)),
                      mgs::exceptions::unexpected_eof_error);
      CHECK_THROWS_AS((valid_codec::decode<std::array<char, 5>>(input)),
                      mgs::exceptions::unexpected_eof_error);
    }
  }
}
