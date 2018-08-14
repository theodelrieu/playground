#include <catch.hpp>

#include <algorithm>
#include <cstdint>
#include <string>
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

class noop_transformer
{
public:
  using value_type = std::vector<std::uint8_t>;

  template <typename Iterator, typename Sentinel>
  value_type operator()(Iterator& begin, Sentinel end) const
  {
    value_type v;
    v.reserve(std::distance(begin, end));
    while (begin != end)
      v.push_back(*begin++);
    return v;
  }
};

class noop_codec
{
  template <typename Iterator, typename Sentinel>
  using adapter =
      adapters::transformer_adapter<noop_transformer, Iterator, Sentinel>;

public:
  template <typename Iterator, typename Sentinel>
  static auto make_encoder(Iterator it, Sentinel sent)
  {
    return adapter<Iterator, Sentinel>(it, sent);
  }

  template <typename Iterator, typename Sentinel>
  static auto make_decoder(Iterator it, Sentinel sent)
  {
    return adapter<Iterator, Sentinel>(it, sent);
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
// TODO test full specialization
template <>
struct output_traits<valid_type>
{
  template <typename Iterator>
  static valid_type create(Iterator begin, Iterator end)
  {
    return {{begin, end}};
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

    static_assert(
        !concepts::is_codec_output<invalid_type, Encoder::iterator>::value, "");
    static_assert(
        concepts::is_codec_output<valid_type, Encoder::iterator>::value, "");

    SECTION("User-defined type")
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

    // SECTION("Containers")
    // {
    //   auto const input = "test"s;
    //
    //   auto const encoded = noop_codec::encode<std::string>(input);
    //   CHECK(input == encoded);
    // }
  }
}
