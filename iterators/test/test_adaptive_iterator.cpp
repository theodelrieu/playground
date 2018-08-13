#include <forward_list>
#include <list>
#include <sstream>
#include <tuple>

#include <catch.hpp>

#include <mgs/meta/detected/types/iterator_category.hpp>
#include <mgs/meta/concepts/core/derived_from.hpp>
#include <mgs/meta/concepts/iterator/bidirectional_iterator.hpp>
#include <mgs/meta/concepts/iterator/forward_iterator.hpp>
#include <mgs/meta/concepts/iterator/input_iterator.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/random_access_iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>

#include <mgs/iterators/adaptive_iterator.hpp>

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
  using iterator = iterators::adaptive_iterator<
      noop_encoder,
      meta::detected::types::iterator_category<underlying_iterator_traits>>;

  friend iterators::adaptive_iterator<
      noop_encoder,
      meta::detected::types::iterator_category<underlying_iterator_traits>>;

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
  if (meta::core_concepts::is_derived_from<
          meta::detected::types::iterator_category<
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
void iterator_checks(InputIterator current,
                     Sentinel end,
                     std::input_iterator_tag)
{
  static_assert(
      meta::iterator_concepts::is_input_iterator<InputIterator>::value, "");
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
void iterator_checks(ForwardIterator current,
                     Sentinel end,
                     std::forward_iterator_tag)
{
  static_assert(
      meta::iterator_concepts::is_forward_iterator<ForwardIterator>::value, "");
  iterator_checks(current, end, std::input_iterator_tag{});

  CHECK(*(current++) == 'a');
  auto copy = current;
  CHECK(copy == current);
  CHECK(++copy == ++current);
  CHECK(ForwardIterator{} == end);
  // TODO add is_output_iterator and check reference
  using Traits = std::iterator_traits<ForwardIterator>;
  static_assert(std::is_same<typename Traits::reference,
                             const typename Traits::value_type&>::value,
                "");
}

template <typename BidirectionalIterator, typename Sentinel>
void iterator_checks(BidirectionalIterator current,
                     Sentinel end,
                     std::bidirectional_iterator_tag)
{
  static_assert(meta::iterator_concepts::is_bidirectional_iterator<
                    BidirectionalIterator>::value,
                "");
  iterator_checks(current, end, std::forward_iterator_tag{});

  REQUIRE(*current == 'a');
  std::advance(current, 3);
  --current;
  CHECK(*current == 'c');
  auto copy = current;
  CHECK(--copy == --current);
  current--;
  CHECK(*current == 'a');
}

template <typename RandomAccessIterator, typename Sentinel>
void iterator_checks(RandomAccessIterator current,
                     Sentinel end,
                     std::random_access_iterator_tag)
{
  static_assert(meta::iterator_concepts::is_random_access_iterator<
                    RandomAccessIterator>::value,
                "");
  iterator_checks(current, end, std::bidirectional_iterator_tag{});

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

template <typename Iterator, typename Sentinel>
void iterator_checks(Iterator it, Sentinel sent)
{
  noop_encoder<Iterator, Sentinel> encoder{it, sent};
  auto current = encoder.begin();
  auto end = encoder.end();

  static_assert(meta::iterator_concepts::is_iterator<decltype(current)>::value,
                "");
  static_assert(meta::iterator_concepts::is_sentinel<decltype(end),
                                                     decltype(current)>::value,
                "");
  using encoder_tag = meta::detected::types::iterator_category<
      std::iterator_traits<decltype(current)>>;
  using underlying_tag =
      meta::detected::types::iterator_category<std::iterator_traits<Iterator>>;
  static_assert(std::is_same<underlying_tag, encoder_tag>::value, "");

  iterator_checks(current, end, encoder_tag{});
}
}

TEST_CASE("adaptive_input_iterator", "[iterators][adaptive]")
{
  std::stringstream ss("abcdefghijklmnopqrstuvwxyz");
  std::istreambuf_iterator<char> underlying_begin{ss};
  std::istreambuf_iterator<char> const underlying_end{};

  iterator_checks(underlying_begin, underlying_end);
}

TEST_CASE("adaptive_forward_iterator", "[iterators][adaptive]")
{
  auto const alphabet = "abcdefghijklmnopqrstuvwxyz"s;
  std::forward_list<char> fl(alphabet.begin(), alphabet.end());

  iterator_checks(fl.begin(), fl.end());
}

TEST_CASE("adaptive_bidirectional_iterator", "[iterators][adaptive]")
{
  auto const alphabet = "abcdefghijklmnopqrstuvwxyz"s;
  std::list<char> l(alphabet.begin(), alphabet.end());

  iterator_checks(l.begin(), l.end());
}

TEST_CASE("adaptive_random_access_iterator", "[iterators][adaptive]")
{
  auto const alphabet = "abcdefghijklmnopqrstuvwxyz"s;

  iterator_checks(alphabet.begin(), alphabet.end());
}
