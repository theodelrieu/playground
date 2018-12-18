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
#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/meta/call_std/begin.hpp>
#include <mgs/meta/concepts/iterator/random_access_iterator.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/member_functions/resize.hpp>
#include <mgs/meta/detected/types/key_type.hpp>
#include <mgs/meta/detected/types/size_type.hpp>
#include <mgs/meta/detected/types/value_type.hpp>
#include <mgs/meta/priority_tag.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace detail
{
template <typename ContiguousContainer, typename T>
ContiguousContainer fill_contiguous_container(
    adapters::concepts::SizedTransformedInputAdapter<T>& adapter,
    meta::priority_tag<1>)
{
  using std::begin;

  auto const max_size = adapter.max_transformed_size();
  ContiguousContainer cont(max_size, 0);

  auto const total_read = adapter.read(begin(cont), max_size);
  cont.resize(total_read);
  return cont;
}

template <typename ContiguousContainer, typename T>
ContiguousContainer fill_contiguous_container(
    adapters::concepts::TransformedInputAdapter<T>& adapter,
    meta::priority_tag<0>)
{
  using std::begin;

  constexpr auto block_size = 256;

  ContiguousContainer cont(block_size, 0);

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

template <typename T, typename = void>
struct default_converter
{
private:
  // TODO concept? or a simple trait?
  // Contiguous containers which has the following properties:
  // - T(T::size_type, T::value_type const&)
  // - T::resize(T::size_type)
  // - begin(T&) -> RandomAccessIterator
  template <
      typename TransformedInputAdapter,
      typename U = T,
      typename SizeType = meta::detected_t<meta::detected::types::size_type, U>,
      typename ValueType =
          meta::detected_t<meta::detected::types::value_type, U>,
      typename LvalueU = std::add_lvalue_reference_t<U>,
      typename = std::enable_if_t<
          std::is_integral<ValueType>::value &&
          !std::is_same<ValueType, bool>::value && sizeof(ValueType) == 1 &&
          meta::concepts::iterator::is_random_access_iterator<
              meta::detected_t<meta::result_of_begin, LvalueU>>::value &&
          std::is_constructible<U, SizeType, ValueType const&>::value &&
          meta::is_detected_exact<void,
                                  meta::detected::member_functions::resize,
                                  LvalueU,
                                  SizeType>::value>>
  static U create_impl(TransformedInputAdapter& adapter, meta::priority_tag<1>)
  {
    return fill_contiguous_container<U>(adapter, meta::priority_tag<1>{});
  }

  template <typename TransformedInputAdapter,
            typename Iterator = meta::result_of_begin<TransformedInputAdapter&>,
            typename U = T,
            typename = std::enable_if_t<
                (std::is_copy_constructible<U>::value ||
                 std::is_move_constructible<U>::value) &&
                std::is_constructible<U, Iterator, Iterator>::value &&
                // Associative containers' range constructors are not
                // SFINAE-friendly...
                !meta::is_detected<meta::detected::types::key_type, U>::value>>
  static T create_impl(TransformedInputAdapter& adapter, meta::priority_tag<0>)
  {
    return T(adapter.begin(), adapter.end());
  }

public:
  template <typename TransformedInputAdapter>
  static auto create(
      adapters::concepts::TransformedInputAdapter<TransformedInputAdapter>&
          adapter) -> decltype(create_impl(adapter, meta::priority_tag<1>{}))
  {
    return create_impl(adapter, meta::priority_tag<1>{});
  }
};

template <typename C, std::size_t N>
struct default_converter<std::array<C, N>>
{
  template <typename T,
            typename = adapters::concepts::TransformedInputAdapter<T>,
            typename Iterator = meta::result_of_begin<T&>,
            typename = std::enable_if_t<std::is_convertible<
                typename std::iterator_traits<Iterator>::value_type,
                C>::value>>
  static std::array<C, N> create(T& adapter)
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
