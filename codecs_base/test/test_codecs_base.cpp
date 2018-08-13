#include <catch.hpp>

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

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

template <typename UnderlyingIterator, typename Sentinel = UnderlyingIterator>
class noop_adapter
{
public:
  using underlying_iterator = UnderlyingIterator;
  using underlying_sentinel = Sentinel;

private:
  using underlying_iterator_traits = std::iterator_traits<UnderlyingIterator>;
  using iterator = iterators::adaptive_iterator<
      noop_adapter,
      meta::detected::types::iterator_category<underlying_iterator_traits>>;

  friend iterators::adaptive_iterator<
      noop_adapter,
      meta::detected::types::iterator_category<underlying_iterator_traits>>;

public:
  using difference_type = typename underlying_iterator_traits::difference_type;
  using value_type = typename underlying_iterator_traits::value_type;

  noop_adapter() = default;
  noop_adapter(UnderlyingIterator it, Sentinel end)
    : _begin(it), _current_it(it), _end(end)
  {
  }

  iterator begin() const
  {
    return iterator{*this};
  }

  iterator end() const
  {
    noop_adapter enc{{}, _end};
    enc._begin = _begin;
    enc._current_it = enc._end;
    return iterator{enc};
  }

  void seek_backward(difference_type n)
  {
    std::advance(_current_it, n);
  }

  void seek_forward(difference_type n)
  {
    std::advance(_current_it, n);
  }

  value_type const& get() const
  {
    // be sure to return a reference
    _last_read = *_current_it;
    return _last_read;
  }

  auto pos() const
  {
    return std::distance(_begin, _current_it);
  }

  template <typename T, typename U>
  friend bool operator==(noop_adapter<T, U> const& lhs,
                         noop_adapter<T, U> const& rhs) noexcept;

private:
  UnderlyingIterator _begin{};
  UnderlyingIterator _current_it{};
  Sentinel _end{};
  value_type mutable _last_read{};
};

template <typename T, typename U>
bool operator==(noop_adapter<T, U> const& lhs,
                noop_adapter<T, U> const& rhs) noexcept
{
  if (meta::concepts::core::is_derived_from<
          meta::detected::types::iterator_category<
              typename noop_adapter<T, U>::underlying_iterator_traits>,
          std::forward_iterator_tag>::value)
  {
    // singular iterator
    auto const singular_it = typename noop_adapter<T, U>::underlying_iterator{};
    if (lhs._begin == singular_it)
    {
      return rhs._current_it == rhs._end;
    }
    else if (rhs._begin == singular_it)
    {
      return lhs._current_it == lhs._end;
    }
  }
  return std::tie(lhs._begin, lhs._current_it, lhs._end) ==
         std::tie(rhs._begin, rhs._current_it, rhs._end);
}

template <typename T, typename U>
bool operator!=(noop_adapter<T, U> const& lhs,
                noop_adapter<T, U> const& rhs) noexcept
{
  return !(lhs == rhs);
}

class noop_codec
{
public:
  template <typename Iterator, typename Sentinel>
  static auto make_encoder(Iterator it, Sentinel sent)
  {
    return noop_adapter<Iterator, Sentinel>(it, sent);
  }

  template <typename Iterator, typename Sentinel>
  static auto make_decoder(Iterator it, Sentinel sent)
  {
    return noop_adapter<Iterator, Sentinel>(it, sent);
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
  template <typename Iterator>
  static valid_type create(Iterator begin, Iterator end)
  {
    return {{begin, end}};
  }
};
}
}

TEST_CASE("codecs_base", "[codecs_base]")
{
  SECTION("codec output")
  {
    static_assert(
        !concepts::is_codec_output<invalid_type, noop_adapter<char*>>::value,
        "");
    static_assert(
        concepts::is_codec_output<valid_type, noop_adapter<char*>>::value, "");

    auto const str = "test"s;

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

    SECTION("Containers")
    {
      auto const input = "test"s;

      auto const encoded = noop_codec::encode<std::string>(input);
      CHECK(input == encoded);
    }
  }
}
