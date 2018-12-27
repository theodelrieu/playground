#pragma once

#include <array>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

#include <mgs/adapters/concepts/sized_transformed_input_adapter.hpp>
#include <mgs/adapters/concepts/transformed_input_adapter.hpp>
#include <mgs/adapters/transformed_input_adapter_iterator.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/meta/call_std/begin.hpp>
#include <mgs/meta/concepts/iterator/random_access_iterator.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/member_functions/resize.hpp>
#include <mgs/meta/detected/types/key_type.hpp>
#include <mgs/meta/detected/types/size_type.hpp>
#include <mgs/meta/priority_tag.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace detail
{
template <typename RandomAccessContainer, typename T>
RandomAccessContainer fill_random_access_container(
    adapters::concepts::SizedTransformedInputAdapter<
        T,
        meta::result_of_begin<RandomAccessContainer&>>& adapter,
    meta::priority_tag<1>)
{
  using std::begin;

  auto const max_size = adapter.max_transformed_size();
  RandomAccessContainer cont(max_size, 0);

  auto const total_read = adapter.read(begin(cont), max_size);
  cont.resize(total_read);
  return cont;
}

template <typename RandomAccessContainer, typename T>
RandomAccessContainer fill_random_access_container(
    adapters::concepts::TransformedInputAdapter<
        T,
        meta::result_of_begin<RandomAccessContainer&>>& adapter,
    meta::priority_tag<0>)
{
  using std::begin;

  constexpr auto block_size = 256;

  RandomAccessContainer cont(block_size, 0);

  auto total_read = 0;
  for (auto nb_read = adapter.read(begin(cont) + total_read, block_size);
       nb_read != 0;
       nb_read = adapter.read(begin(cont) + total_read, block_size))
  {
    total_read += nb_read;
    cont.resize(total_read + block_size);
  }
  cont.resize(total_read);
  return cont;
}

template <typename Output, typename = void>
struct default_converter
{
private:
  // Overload for containers which have the following properties:
  // - DefaultConstructible
  // FIXME add copy / or move constructible
  // - T::resize(T::size_type)
  // - begin(T&) -> RandomAccessIterator
  template <typename T,
            typename R = Output,
            typename SizeType = typename R::size_type,
            typename = std::enable_if_t<
                meta::concepts::iterator::is_random_access_iterator<
                    meta::result_of_begin<R&>>::value &&
                std::is_default_constructible<R>::value &&
                meta::is_detected<meta::detected::member_functions::resize,
                                  R&,
                                  SizeType>::value>>
  static R create_impl(adapters::concepts::TransformedInputAdapter<T>& adapter,
                       meta::priority_tag<1>)
  {
    return fill_random_access_container<R>(adapter, meta::priority_tag<1>{});
  }

  // Default overload, non-associative containers which:
  // - can be constructed with an Iterator range
  // - are copy or move constructible
  template <typename T,
            typename = adapters::concepts::TransformedInputAdapter<T>,
            // FIXME require IterableStuff
            typename Iterator = adapters::transformed_input_adapter_iterator<T>,
            typename R = Output,
            typename = std::enable_if_t<
                (std::is_copy_constructible<R>::value ||
                 std::is_move_constructible<R>::value) &&
                std::is_constructible<R, Iterator, Iterator>::value &&
                // Associative containers' iterator-range constructors are not
                // SFINAE-friendly...
                !meta::is_detected<meta::detected::types::key_type, R>::value>>
  static R create_impl(T& adapter, meta::priority_tag<0>)
  {
    return R(Iterator(adapter), Iterator());
  }

public:
  template <typename T>
  static auto create(adapters::concepts::TransformedInputAdapter<T>& adapter)
      -> decltype(create_impl(adapter, meta::priority_tag<1>{}))
  {
    return create_impl(adapter, meta::priority_tag<1>{});
  }
};

template <typename C, std::size_t N>
struct default_converter<std::array<C, N>>
{
  template <typename T>
  static std::array<C, N> create(
      // FIXME C const*
      adapters::concepts::TransformedInputAdapter<T, C*>& adapter)
  {
    std::array<C, N> ret;

    auto const nb_read = adapter.read(ret.begin(), N);
    if (nb_read != N)
      throw exceptions::unexpected_eof_error("output buffer is too large");
    if (adapter.read(ret.begin(), 1) != 0)
      throw exceptions::unexpected_eof_error("output buffer is too small");
    return ret;
  }
};
}
}
}
}
