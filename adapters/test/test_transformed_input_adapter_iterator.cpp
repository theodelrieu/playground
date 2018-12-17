#include <forward_list>
#include <list>
#include <sstream>
#include <tuple>

#include <catch.hpp>

#include <mgs/meta/concepts/core/derived_from.hpp>
#include <mgs/meta/concepts/iterator/bidirectional_iterator.hpp>
#include <mgs/meta/concepts/iterator/forward_iterator.hpp>
#include <mgs/meta/concepts/iterator/input_iterator.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/random_access_iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/detected/types/iterator_category.hpp>

#include <mgs/adapters/transformed_input_adapter_iterator.hpp>

using namespace mgs;
using namespace std::string_literals;

namespace
{
template <typename UnderlyingIterator, typename Sentinel = UnderlyingIterator>
class noop_encoder
{
public:
  using underlying_iterator = UnderlyingIterator;
  using underlying_sentinel = Sentinel;

private:
  using underlying_iterator_traits = std::iterator_traits<UnderlyingIterator>;
  using iterator = adapters::transformed_input_adapter_iterator<noop_encoder>;

  friend iterator;

public:
  using difference_type = typename underlying_iterator_traits::difference_type;
  using value_type = typename underlying_iterator_traits::value_type;

  noop_encoder() = default;
  noop_encoder(UnderlyingIterator it, Sentinel end)
    : _begin(it), _current_it(it), _end(end)
  {
  }

  iterator begin()
  {
    return iterator{*this};
  }

  iterator end() const
  {
    noop_encoder enc{{}, _end};
    enc._begin = _begin;
    enc._current_it = enc._end;
    return iterator{enc};
  }

  void seek_forward(difference_type n)
  {
    std::advance(_current_it, n);
  }

  std::size_t read(value_type*, std::size_t)
  {
    return 0;
  }

  value_type const& get() const
  {
    // be sure to return a reference
    _last_read = *_current_it;
    return _last_read;
  }

  template <typename T>
  friend bool operator==(noop_encoder<T> const& lhs,
                         noop_encoder<T> const& rhs) noexcept;

private:
  UnderlyingIterator _begin{};
  UnderlyingIterator _current_it{};
  Sentinel _end{};
  value_type mutable _last_read{};
};

template <typename T>
bool operator==(noop_encoder<T> const& lhs, noop_encoder<T> const& rhs) noexcept
{
  return std::tie(lhs._begin, lhs._current_it, lhs._end) ==
         std::tie(rhs._begin, rhs._current_it, rhs._end);
}

template <typename T>
bool operator!=(noop_encoder<T> const& lhs, noop_encoder<T> const& rhs) noexcept
{
  return !(lhs == rhs);
}
}

TEST_CASE("transformed_input_adapter_iterator", "[adapters][transformed_input_adapter_iterator]")
{
  std::stringstream ss("abcdefghijklmnopqrstuvwxyz");
  using Iterator = std::istreambuf_iterator<char>;
  using Sentinel = Iterator;

  Iterator it{ss};
  Sentinel sent{};

  noop_encoder<Iterator, Sentinel> encoder{it, sent};
  auto current = encoder.begin();
  auto end = encoder.end();

  static_assert(meta::concepts::iterator::is_iterator<decltype(current)>::value,
                "");
  static_assert(meta::concepts::iterator::is_sentinel<decltype(end),
                                                      decltype(current)>::value,
                "");
  using encoder_tag = meta::detected::types::iterator_category<
      std::iterator_traits<decltype(current)>>;
  using underlying_tag =
      meta::detected::types::iterator_category<std::iterator_traits<Iterator>>;
  static_assert(std::is_same<underlying_tag, encoder_tag>::value, "");

  static_assert(
      meta::concepts::iterator::is_input_iterator<decltype(current)>::value,
      "");
  CHECK(*current == *current);
  CHECK(*current == 'a');
  CHECK(*++current == 'b');
  current++;
  CHECK(*current == 'c');
  CHECK(current != end);
  CHECK(current == current);
  CHECK(std::distance(current, end) == 24);
}
