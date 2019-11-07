#pragma once

#include <algorithm>
#include <type_traits>

#include <mgs/codecs/detail/string_literal_char_type.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/input_range.hpp>
#include <mgs/meta/concepts/output_iterator.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/concepts/sized_sentinel_for.hpp>
#include <mgs/meta/iter_value_t.hpp>
#include <mgs/meta/iterator_t.hpp>
#include <mgs/meta/sentinel_t.hpp>
#include <mgs/meta/ssize_t.hpp>

namespace mgs
{
namespace codecs
{
template <typename I, typename S = I>
class input_source_view
{
  static_assert(meta::is_input_iterator<I>::value,
                "I does not model meta::input_iterator");
  static_assert(meta::is_sentinel_for<S, I>::value,
                "S does not model meta::sentinel_for<I>");

public:
  using element_type = meta::iter_value_t<I>;

  input_source_view() = default;
  input_source_view(I begin, S end);

  I begin() const;
  S end() const;

  template <typename OutputIterator,
            typename = std::enable_if_t<
                meta::is_output_iterator<OutputIterator, element_type>::value>>
  meta::ssize_t read(OutputIterator out, meta::ssize_t n);

  template <
      typename S2 = S,
      typename = std::enable_if_t<meta::is_sized_sentinel_for<S2, I>::value>>
  meta::ssize_t max_remaining_size() const;

private:
  I _current{};
  S _end{};
};

template <typename I, typename S>
auto make_input_source_view(meta::input_iterator<I> begin,
                            meta::sentinel_for<S, I> end)
    -> input_source_view<I, S>
{
  return {std::move(begin), std::move(end)};
}

// For some "fun" reasons, trying to disambiguate the following overloads with
// priority_tag results in the following warning: ISO C++ says that these are
// ambiguous, even though the worst conversion for the first is better than the
// worst conversion for the second.

template <typename R, typename = std::enable_if_t<!std::is_array<R>::value>>
auto make_input_source_view(meta::input_range<R>& rng)
    -> input_source_view<meta::iterator_t<R>, meta::sentinel_t<R>>
{
  using std::begin;
  using std::end;

  return {begin(rng), end(rng)};
}

template <typename CharT,
          std::size_t N,
          std::enable_if_t<!detail::is_string_literal_char_type<CharT>::value,
                           int> = 0>
auto make_input_source_view(CharT const (&tab)[N])
    -> input_source_view<CharT const*>
{
  auto const b = std::begin(tab);
  auto const e = std::end(tab);

  return {b, e};
}

template <typename CharT,
          std::size_t N,
          std::enable_if_t<detail::is_string_literal_char_type<CharT>::value,
                           int> = 0>
auto make_input_source_view(CharT const (&tab)[N])
    -> input_source_view<CharT const*>
{
  auto const b = std::begin(tab);
  auto const e = std::end(tab);

  return {b, std::find(b, e, CharT{})};
}
}
}

#include <mgs/codecs/detail/input_source_view_impl.hpp>
