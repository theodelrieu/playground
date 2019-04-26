#pragma once

#include <array>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

#include <mgs/concepts/readable_transformed_input_range.hpp>
#include <mgs/concepts/sized_transformed_input_range.hpp>
#include <mgs/concepts/transformed_input_range.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/meta/concepts/constructible.hpp>
#include <mgs/meta/concepts/copy_constructible.hpp>
#include <mgs/meta/concepts/random_access_iterator.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/member_functions/resize.hpp>
#include <mgs/meta/detected/types/key_type.hpp>
#include <mgs/meta/detected/types/size_type.hpp>
#include <mgs/meta/iterator_t.hpp>
#include <mgs/meta/priority_tag.hpp>

namespace mgs
{
namespace detail
{
template <typename RandomAccessContainer,
          typename T,
          typename = mgs::ReadableTransformedInputRange<
              T,
              meta::iterator_t<RandomAccessContainer>>,
          typename = mgs::SizedTransformedInputRange<T>>
RandomAccessContainer fill_random_access_container(T& range,
                                                   meta::priority_tag<1>)
{
  using std::begin;

  auto const max_size = range.max_transformed_size();
  RandomAccessContainer cont(max_size, 0);

  auto const total_read = range.read(begin(cont), max_size);
  cont.resize(
      static_cast<typename RandomAccessContainer::size_type>(total_read));
  return cont;
}

template <typename RandomAccessContainer, typename T>
RandomAccessContainer fill_random_access_container(
    mgs::ReadableTransformedInputRange<T,
                                       meta::iterator_t<RandomAccessContainer>>&
        range,
    meta::priority_tag<0>)
{
  using std::begin;
  using size_type = typename RandomAccessContainer::size_type;

  constexpr auto block_size = 256;

  RandomAccessContainer cont(block_size, 0);

  meta::ssize_t total_read = 0;
  for (auto nb_read = range.read(begin(cont) + total_read, block_size);
       nb_read != 0;
       nb_read = range.read(begin(cont) + total_read, block_size))
  {
    total_read += nb_read;
    cont.resize(static_cast<size_type>(total_read + block_size));
  }
  cont.resize(static_cast<size_type>(total_read));
  return cont;
}

template <typename Output, typename = void>
struct default_converter
{
private:
  // Overload for containers which have the following properties:
  // - DefaultConstructible
  // - Copy or Move constructible
  // - T::resize(T::size_type)
  // - begin(T&) -> RandomAccessIterator
  template <typename T,
            typename R = Output,
            typename SizeType = typename R::size_type,
            typename = std::enable_if_t<
                meta::concepts::is_random_access_iterator<
                    meta::iterator_t<R>>::value &&
                meta::concepts::is_constructible<R>::value &&
                // Keep those in C++17 as well, fill_random_access_container
                // relies on NRVO, not on Guaranteed Copy Elision.
                (meta::concepts::is_copy_constructible<R>::value ||
                 meta::concepts::is_move_constructible<R>::value) &&
                meta::is_detected<meta::detected::member_functions::resize,
                                  R&,
                                  SizeType>::value>>
  static R create_impl(mgs::TransformedInputRange<T>& range,
                       meta::priority_tag<1>)
  {
    return fill_random_access_container<R>(range, meta::priority_tag<1>{});
  }

  // Default overload, non-associative containers which:
  // - can be constructed with an Iterator range
  // - are copy or move constructible (pre C++17)
  template <
      typename T,
      typename = mgs::TransformedInputRange<T>,
      typename Iterator = meta::iterator_t<T>,
      typename R = Output,
      typename = std::enable_if_t<
  // Guaranteed copy-elision
#if __cplusplus < 201703L
          (meta::concepts::is_move_constructible<R>::value ||
           meta::concepts::is_copy_constructible<R>::value) &&
#endif
          meta::concepts::is_constructible<R, Iterator, Iterator>::value &&
          // Associative containers' iterator-range constructors are not
          // SFINAE-friendly...
          !meta::is_detected<meta::detected::types::key_type, R>::value>>
  static R create_impl(T& range, meta::priority_tag<0>)
  {
    using std::begin;
    using std::end;
    return R(begin(range), end(range));
  }

public:
  template <typename T>
  static auto create(mgs::TransformedInputRange<T>& range)
      -> decltype(create_impl(range, meta::priority_tag<1>{}))
  {
    return create_impl(range, meta::priority_tag<1>{});
  }
};

template <typename C, std::size_t N>
struct default_converter<std::array<C, N>>
{
  template <typename T>
  static std::array<C, N> create(
      mgs::ReadableTransformedInputRange<T, C*>& range)
  {
    std::array<C, N> ret;

    auto const nb_read = range.read(ret.begin(), N);
    if (nb_read != N)
      throw exceptions::unexpected_eof_error("output buffer is too large");
    if (range.read(ret.begin(), 1) != 0)
      throw exceptions::unexpected_eof_error("output buffer is too small");
    return ret;
  }
};
}
}
