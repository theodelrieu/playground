#include <catch.hpp>

#include <algorithm>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/codecs/concepts/codec.hpp>
#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/output_traits.hpp>
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
    return {_current, _end};
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
}

namespace mgs
{
namespace codecs
{
template <>
struct output_traits<valid_type>
{
  // template <typename Iterator>
  // static valid_type create(Iterator begin, Iterator end)
  // {
  //   return {{begin, end}};
  // }

  template <typename Iterator, typename Sentinel>
  static valid_type create(
      // CANNOT DEDUCE :(
      typename noop_codec::decoder<Iterator, Sentinel>::iterator begin,
      typename noop_codec::decoder<Iterator, Sentinel>::iterator end)
  {
  }
};
}
}

template <typename T>
struct S;

TEST_CASE("codecs_base", "[codecs_base]")
{
  SECTION("codec output")
  {
    auto const str = "test"s;

    using Encoder = decltype(noop_codec::make_encoder(str.begin(), str.end()));

    S<typename noop_codec::decoder<char*, char*>::iterator> s;
    S<Encoder::iterator> s2;
    static_assert(
        !concepts::is_codec_output<invalid_type, Encoder::iterator>::value, "");
    static_assert(
        !concepts::is_codec_output<valid_type, Encoder::iterator>::value, "");

    // SECTION("User-defined type")
    // {
    //   auto v = noop_codec::encode<valid_type>(str);
    //   CHECK(std::equal(v.vec.begin(), v.vec.end(), str.begin(), str.end()));
    //
    //   auto v2 = noop_codec::encode<valid_type>(str.begin(), str.end());
    //   CHECK(v.vec == v2.vec);
    //
    //   auto v3 = noop_codec::decode<valid_type>(str.begin(), str.end());
    //   CHECK(v.vec == v3.vec);
    //
    //   auto v4 = noop_codec::decode<valid_type>(str);
    //   CHECK(v.vec == v4.vec);
    // }
    //
    // SECTION("Containers")
    // {
    //   auto const input = "test"s;
    //
    //   auto const encoded = noop_codec::encode<std::string>(input);
    //   CHECK(input == encoded);
    // }
  }
}
