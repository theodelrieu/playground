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

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/codecs/concepts/codec.hpp>
#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/output_traits.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/iterators/adaptive_iterator.hpp>

using namespace mgs;
using namespace mgs::codecs;
using namespace std::string_literals;

// TODO make noop_codec an instantiation of basic_codec
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
  using value_type = std::vector<std::uint8_t>;

  noop_transformer() = default;

  noop_transformer(Iterator begin, Sentinel end)
    : _current(std::move(begin)), _end(std::move(end))
  {
  }

  value_type operator()()
  {
    value_type ret;
    while (_current != _end)
      ret.push_back(*_current++);
    return ret;
  }

private:
  Iterator _current{};
  Sentinel _end{};
};

class noop_codec
{

public:
  template <typename Iterator, typename Sentinel>
  using encoder =
      adapters::transformer_adapter<noop_transformer<Iterator, Sentinel>>;

  template <typename Iterator, typename Sentinel>
  using decoder =
      adapters::transformer_adapter<noop_transformer<Iterator, Sentinel>>;

  template <typename Iterator, typename Sentinel>
  static auto make_encoder(Iterator it, Sentinel sent)
  {
    return encoder<Iterator, Sentinel>(it, sent);
  }

  template <typename Iterator, typename Sentinel>
  static auto make_decoder(Iterator it, Sentinel sent)
  {
    return decoder<Iterator, Sentinel>(it, sent);
  }

  template <typename T, typename Iterator, typename Sentinel>
  static auto encode(Iterator it, Sentinel sent)
  {
    auto enc = make_encoder(it, sent);
    return output_traits<T>::create(enc.begin(), enc.end());
  }

  template <typename T, typename Iterator, typename Sentinel>
  static auto decode(Iterator it, Sentinel sent)
  {
    auto dec = make_decoder(it, sent);
    return output_traits<T>::create(dec.begin(), dec.end());
  }

  template <typename T, typename Iterable>
  static auto encode(Iterable const& it)
  {
    using std::begin;
    using std::end;
    return noop_codec::encode<T>(begin(it), end(it));
  }

  template <typename T, typename Iterable>
  static auto decode(Iterable const& it)
  {
    using std::begin;
    using std::end;
    return noop_codec::decode<T>(begin(it), end(it));
  }
};

template <typename T, typename Input>
void check_output_container(Input const& input)
{
  auto const output = noop_codec::encode<T>(input);
  CHECK(std::equal(output.begin(), output.end(), input.begin(), input.end()));
}
}

namespace mgs
{
namespace codecs
{
template <>
struct output_traits<valid_type>
{
  template <typename Iterator>
  static valid_type create(Iterator begin, Iterator end)
  {
    return {{begin, end}};
  }
};

template <typename T>
struct output_traits<std::vector<T>>
{
  template <typename Iterator>
  static std::vector<T> create(Iterator begin, Iterator end)
  {
    return {begin, end};
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

    static_assert(
        !concepts::is_codec_output<invalid_type, Encoder::iterator>::value, "");
    static_assert(
        concepts::is_codec_output<valid_type, Encoder::iterator>::value, "");

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
        check_output_container<std::string>(input);
        check_output_container<std::vector<char>>(input);
        check_output_container<std::vector<std::uint8_t>>(input);
        check_output_container<std::list<char>>(input);
        check_output_container<std::deque<std::uint8_t>>(input);
        check_output_container<std::forward_list<char>>(input);
        check_output_container<std::array<std::uint8_t, 4>>(input);
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
        CHECK_THROWS_AS((check_output_container<std::array<char, 3>>(input)),
                        exceptions::unexpected_eof_error);
        CHECK_THROWS_AS((check_output_container<std::array<char, 5>>(input)),
                        exceptions::unexpected_eof_error);
      }
    }
  }
}
