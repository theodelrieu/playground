#include <forward_list>
#include <list>
#include <sstream>
#include <tuple>

#include <catch.hpp>

#include <b64/detail/meta/aliases.hpp>
#include <b64/detail/meta/concepts/derived_from.hpp>
#include <b64/detail/meta/concepts/iterator.hpp>

#include <b64/detail/iterators/adaptive_iterator.hpp>

#include <b64/encoders/base64_stream.hpp>

using namespace b64;
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
  using iterator = detail::adaptive_iterator<
      noop_encoder,
      detail::iterator_category_t<underlying_iterator_traits>>;

  friend detail::adaptive_iterator<
      noop_encoder,
      detail::iterator_category_t<underlying_iterator_traits>>;

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
    return {*this};
  }

  iterator end() const
  {
    noop_encoder enc{{}, _end};
    enc._begin = _begin;
    enc._current_it = enc._end;
    return {enc};
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
  friend bool operator==(noop_encoder<T, U> const& lhs,
                         noop_encoder<T, U> const& rhs) noexcept;

private:
  UnderlyingIterator _begin{};
  UnderlyingIterator _current_it{};
  Sentinel _end{};
  value_type mutable _last_read{};
};

template <typename T, typename U>
bool operator==(noop_encoder<T, U> const& lhs,
                noop_encoder<T, U> const& rhs) noexcept
{
  if (detail::is_derived_from<
          detail::iterator_category_t<
              typename noop_encoder<T, U>::underlying_iterator_traits>,
          std::forward_iterator_tag>::value)
  {
    // singular iterator
    auto const singular_it = typename noop_encoder<T, U>::underlying_iterator{};
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
bool operator!=(noop_encoder<T, U> const& lhs,
                noop_encoder<T, U> const& rhs) noexcept
{
  return !(lhs == rhs);
}

template <typename InputIterator, typename Sentinel>
void input_iterator_checks(InputIterator current, Sentinel end)
{
  CHECK(*current == *current);
  CHECK(*current == 'a');
  CHECK(*++current == 'b');
  current++;
  CHECK(*current == 'c');
  CHECK(current != end);
  CHECK(current == current);
  CHECK(std::distance(current, end) == 24);
}

template <typename ForwardIterator, typename Sentinel>
void forward_iterator_checks(ForwardIterator current, Sentinel end)
{
  input_iterator_checks(current, end);

  CHECK(*(current++) == 'a');
  auto copy = current;
  CHECK(copy == current);
  CHECK(++copy == ++current);
  CHECK(ForwardIterator{} == end);
  // We don't have OutputIterators, only check T const&.
  using Traits = std::iterator_traits<ForwardIterator>;
  static_assert(std::is_same<typename Traits::reference,
                             const typename Traits::value_type&>::value,
                "");
}

template <typename BidirectionalIterator, typename Sentinel>
void bidirectional_iterator_checks(BidirectionalIterator current, Sentinel end)
{
  forward_iterator_checks(current, end);

  assert(*current == 'a');
  std::advance(current, 3);
  --current;
  CHECK(*current == 'c');
  auto copy = current;
  CHECK(--copy == --current);
  current--;
  CHECK(*current == 'a');
}

template <typename RandomAccessIterator, typename Sentinel>
void random_access_iterator_checks(RandomAccessIterator current, Sentinel end)
{
  bidirectional_iterator_checks(current, end);

  CHECK(current < end);
  CHECK(current >= current);
  CHECK(current <= current);
  CHECK(current <= end);
  CHECK(end >= current);

  CHECK(*(current + 3) == 'd');
  CHECK(*(3 + current) == 'd');
  CHECK(*(current += 3) == 'd');
  CHECK(*(current - 3) == 'a');
  CHECK(*(current -= 3) == 'a');
  CHECK(current[3] == 'd');
  CHECK(end - current == 26);
}
}

TEST_CASE("adaptive_input_iterator", "[iterators][adaptive]")
{
  std::stringstream ss("abcdefghijklmnopqrstuvwxyz");
  std::istreambuf_iterator<char> underlying_begin{ss};
  std::istreambuf_iterator<char> const underlying_end{};
  noop_encoder<decltype(underlying_begin)> encoder{underlying_begin,
                                                   underlying_end};

  auto current = encoder.begin();
  auto end = encoder.end();

  static_assert(detail::is_iterator<decltype(current)>::value, "");
  static_assert(detail::is_sentinel<decltype(end), decltype(current)>::value,
                "");
  static_assert(
      std::is_same<std::input_iterator_tag,
                   detail::iterator_category_t<
                       std::iterator_traits<decltype(current)>>>::value,
      "");

  input_iterator_checks(current, end);
}

TEST_CASE("adaptive_forward_iterator", "[iterators][adaptive]")
{
  auto const alphabet = "abcdefghijklmnopqrstuvwxyz"s;
  std::forward_list<char> fl(alphabet.begin(), alphabet.end());
  noop_encoder<decltype(fl.begin())> encoder{fl.begin(), fl.end()};

  auto current = encoder.begin();
  auto end = encoder.end();

  static_assert(detail::is_iterator<decltype(current)>::value, "");
  static_assert(detail::is_sentinel<decltype(end), decltype(current)>::value,
                "");
  static_assert(
      std::is_same<std::forward_iterator_tag,
                   detail::iterator_category_t<
                       std::iterator_traits<decltype(current)>>>::value,
      "");

  forward_iterator_checks(current, end);
}

TEST_CASE("adaptive_bidirectional_iterator", "[iterators][adaptive]")
{
  auto const alphabet = "abcdefghijklmnopqrstuvwxyz"s;
  std::list<char> l(alphabet.begin(), alphabet.end());
  noop_encoder<decltype(l.begin())> encoder{l.begin(), l.end()};

  auto current = encoder.begin();
  auto end = encoder.end();

  static_assert(detail::is_iterator<decltype(current)>::value, "");
  static_assert(detail::is_sentinel<decltype(end), decltype(current)>::value,
                "");
  static_assert(
      std::is_same<std::bidirectional_iterator_tag,
                   detail::iterator_category_t<
                       std::iterator_traits<decltype(current)>>>::value,
      "");

  bidirectional_iterator_checks(current, end);
}

TEST_CASE("adaptive_random_access_iterator", "[iterators][adaptive]")
{
  auto const alphabet = "abcdefghijklmnopqrstuvwxyz"s;
  noop_encoder<decltype(alphabet.begin())> encoder{alphabet.begin(), alphabet.end()};

  auto current = encoder.begin();
  auto end = encoder.end();

  static_assert(detail::is_iterator<decltype(current)>::value, "");
  static_assert(detail::is_sentinel<decltype(end), decltype(current)>::value,
                "");
  static_assert(
      std::is_same<std::random_access_iterator_tag,
                   detail::iterator_category_t<
                       std::iterator_traits<decltype(current)>>>::value,
      "");

  random_access_iterator_checks(current, end);
}
