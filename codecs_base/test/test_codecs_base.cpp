#include <catch.hpp>
#include <iostream>

#include <algorithm>
#include <cstdint>
#include <deque>
#include <forward_list>
#include <list>
#include <string>
#include <utility>

#include <mgs/ranges/basic_transformed_input_range.hpp>
#include <mgs/ranges/transformed_input_range_iterator.hpp>
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

template <typename Iterator, typename Sentinel>
class noop_transformer
{
public:
  using underlying_iterator = Iterator;
  using underlying_sentinel = Sentinel;
  using buffer_type = std::vector<std::uint8_t>;

  noop_transformer() = default;

  noop_transformer(Iterator begin, Sentinel end)
    : _current(std::move(begin)), _end(std::move(end))
  {
  }

  void operator()(buffer_type& out)
  {
    out.clear();
    while (_current != _end)
      out.push_back(*_current++);
  }

private:
  Iterator _current{};
  Sentinel _end{};

  template <typename I, typename S>
  friend bool operator==(noop_transformer<I, S> const& lhs,
                         noop_transformer<I, S> const& rhs);
};

template <typename I, typename S>
bool operator==(noop_transformer<I, S> const& lhs,
                noop_transformer<I, S> const& rhs)
{
  return (lhs._current == lhs._end || rhs._current == rhs._end) ?
             (lhs._current == lhs._end && rhs._current == rhs._end) :
             lhs._current == rhs._current;
}

template <typename I, typename S>
bool operator!=(noop_transformer<I, S> const& lhs,
                noop_transformer<I, S> const& rhs)
{
  return !(lhs == rhs);
}

template <typename Iterator, typename Sentinel>
class noop_range : public ranges::basic_transformed_input_range<
                         noop_transformer<Iterator, Sentinel>>
{
public:
  using underlying_iterator = Iterator;
  using underlying_sentinel = Sentinel;

  using ranges::basic_transformed_input_range<
      noop_transformer<Iterator, Sentinel>>::basic_transformed_input_range;
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

TEST_CASE("codecs_base", "[codecs_base]")
{
  SECTION("codec output")
  {
    std::array<char, 4> const input{'t', 'e', 's', 't'};

    using Encoder = decltype(noop_codec::make_encoder(input.begin(), input.end()));

    static_assert(!concepts::is_codec_output<invalid_type, Encoder>::value, "");
    static_assert(concepts::is_codec_output<valid_type, Encoder>::value, "");

    SECTION("C arrays")
    {
      unsigned char tab[10] = {};
      char tab2[10] = {};
      char tab3[10] = "abcdefghi";

      auto const v = noop_codec::encode(tab);
      auto const v2 = noop_codec::encode(tab2);
      auto const v3 = noop_codec::encode(tab3);

      CHECK(v.size() == 10);
      CHECK(v2.size() == 0);
      CHECK(v3.size() == 9);
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
                      exceptions::unexpected_eof_error);
      CHECK_THROWS_AS((noop_codec::encode<std::array<char, 5>>(input)),
                      exceptions::unexpected_eof_error);
      CHECK_THROWS_AS((noop_codec::decode<std::array<char, 3>>(input)),
                      exceptions::unexpected_eof_error);
      CHECK_THROWS_AS((noop_codec::decode<std::array<char, 5>>(input)),
                      exceptions::unexpected_eof_error);
    }
  }
}
