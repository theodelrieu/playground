#include <catch.hpp>
#include <iostream>

#include <algorithm>
#include <cstdint>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include <mgs/adapters/basic_transformed_input_adapter.hpp>
#include <mgs/adapters/transformed_input_adapter_iterator.hpp>
#include <mgs/codecs/basic_codec.hpp>
#include <mgs/codecs/concepts/codec.hpp>
#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/output_traits.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/meta/static_asserts.hpp>
#include <test_helpers/codecs_base.hpp>

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
class noop_adapter : public adapters::basic_transformed_input_adapter<
                         noop_transformer<Iterator, Sentinel>>
{
public:
  using underlying_iterator = Iterator;
  using underlying_sentinel = Sentinel;

  using adapters::basic_transformed_input_adapter<
      noop_transformer<Iterator, Sentinel>>::basic_transformed_input_adapter;
};

struct noop_codec_traits
{
  using default_encoded_output = std::string;
  using default_decoded_output = std::string;

  template <typename Iterator, typename Sentinel>
  static auto make_encoder(Iterator begin, Sentinel end){
    return noop_adapter<Iterator, Sentinel>(std::move(begin), std::move(end));
  }

  template <typename Iterator, typename Sentinel>
  static auto make_decoder(Iterator begin, Sentinel end){
    return noop_adapter<Iterator, Sentinel>(std::move(begin), std::move(end));
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
  template <typename InputAdapter>
  static valid_type create(InputAdapter& adapter)
  {
    return {{adapter.begin(), adapter.end()}};
  }
};

template <typename T>
struct output_traits<std::vector<T>>
{
  template <typename InputAdapter>
  static std::vector<T> create(InputAdapter& adapter)
  {
    return {adapter.begin(), adapter.end()};
  }
};
}
}

TEST_CASE("codecs_base", "[codecs_base]")
{
  SECTION("codec output")
  {
    auto const str = "test"s;

    using Encoder = decltype(noop_codec::make_encoder(str.begin(), str.end()));

    static_assert(!concepts::is_codec_output<invalid_type, Encoder>::value, "");
    static_assert(concepts::is_codec_output<valid_type, Encoder>::value, "");

    SECTION("User-defined types")
    {
      auto v = noop_codec::encode<valid_type>(str);
      CHECK(std::equal(v.vec.begin(), v.vec.end(), str.begin(), str.end()));

      auto v2 = noop_codec::encode<valid_type>(str.begin(), str.end());
      CHECK(v.vec == v2.vec);

      auto v3 = noop_codec::decode<valid_type>(str.begin(), str.end());
      CHECK(v.vec == v3.vec);

      auto v4 = noop_codec::decode<valid_type>(str);
      CHECK(v.vec == v4.vec);
    }

    SECTION("Containers")
    {
      auto const input = "test"s;

      SECTION("Sequence")
      {
        SECTION("std::string")
        {
          test_helpers::run_codec_tests<std::string>(
              noop_codec{}, "test"s, "test"s);
        }

        SECTION("std::vector<char>")
        {
          test_helpers::run_codec_tests<std::vector<char>>(
              noop_codec{}, "test"s, "test"s);
        }

        SECTION("std::vector<std::uint8_t>")
        {
          test_helpers::run_codec_tests<std::vector<std::uint8_t>>(
              noop_codec{}, "test"s, "test"s);
        }

        SECTION("std::list<char>")
        {
          test_helpers::run_codec_tests<std::list<char>>(
              noop_codec{}, "test"s, "test"s);
        }

        SECTION("std::forward_list<char>")
        {
          test_helpers::run_codec_tests<std::forward_list<char>>(
              noop_codec{}, "test"s, "test"s);
        }

        SECTION("std::deque<std::uint8_t>")
        {
          test_helpers::run_codec_tests<std::deque<std::uint8_t>>(
              noop_codec{}, "test"s, "test"s);
        }

        SECTION("std::array<std::uint8_t, 4>")
        {
          test_helpers::run_codec_tests<std::array<std::uint8_t, 4>>(
              noop_codec{}, "test"s, "test"s);
        }
      }

      SECTION("Associative")
      {
        static_assert(!concepts::is_codec_output<std::map<char, char>,
                                                 Encoder::iterator>::value,
                      "");

        static_assert(!concepts::is_codec_output<std::multimap<char, char>,
                                                 Encoder::iterator>::value,
                      "");

        static_assert(!concepts::is_codec_output<std::multiset<char>,
                                                 Encoder::iterator>::value,
                      "");

        static_assert(!concepts::is_codec_output<std::set<char>,
                                                 Encoder::iterator>::value,
                      "");

        static_assert(!concepts::is_codec_output<std::unordered_map<char, char>,
                                                 Encoder::iterator>::value,
                      "");

        static_assert(
            !concepts::is_codec_output<std::unordered_multimap<char, char>,
                                       Encoder::iterator>::value,
            "");

        static_assert(!concepts::is_codec_output<std::unordered_multiset<char>,
                                                 Encoder::iterator>::value,
                      "");

        static_assert(!concepts::is_codec_output<std::unordered_set<char>,
                                                 Encoder::iterator>::value,
                      "");
      }

      SECTION("std::array out of bounds")
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
}
