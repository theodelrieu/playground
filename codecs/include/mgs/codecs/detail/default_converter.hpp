#pragma once

#include <array>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

#include <mgs/config.hpp>

#include <mgs/codecs/basic_input_range.hpp>
#include <mgs/codecs/concepts/input_source.hpp>
#include <mgs/codecs/concepts/sized_input_source.hpp>
#include <mgs/codecs/detail/read_at_most.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/meta/concepts/copyable.hpp>
#include <mgs/meta/concepts/default_constructible.hpp>
#include <mgs/meta/concepts/movable.hpp>
#include <mgs/meta/concepts/output_iterator.hpp>
#include <mgs/meta/concepts/output_range.hpp>
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
  // TODO resizable output range
template <typename ResizableOutputRange, typename T>
ResizableOutputRange fill_resizable_output_range(
    codecs::sized_input_source<T>& is, meta::priority_tag<1>)
{
  using std::begin;
  using size_type = typename ResizableOutputRange::size_type;

  auto const max_size = is.max_remaining_size();
  if (max_size == -1)
    throw exceptions::unexpected_eof_error("invalid input size");
  ResizableOutputRange ret(max_size, 0);

  // TODO refactor
  auto it = begin(ret);
  auto total_read = is.read(it, max_size);
  ret.resize(static_cast<size_type>(total_read));

  for (auto nb_read = is.read(it + total_read, max_size); nb_read != 0;
       nb_read = is.read(it + total_read, max_size))
  {
    total_read += nb_read;
  }
  ret.resize(static_cast<size_type>(total_read));
  return ret;
}

template <typename ResizableOutputRange, typename T>
ResizableOutputRange fill_resizable_output_range(T& is, meta::priority_tag<0>)
{
  using std::begin;
  using size_type = typename ResizableOutputRange::size_type;

  constexpr auto block_size = 256;

  ResizableOutputRange ret(block_size, 0);

  meta::ssize_t total_read = 0;
  for (auto nb_read = is.read(begin(ret) + total_read, block_size);
       nb_read != 0;
       nb_read = is.read(begin(ret) + total_read, block_size))
  {
    total_read += nb_read;
    ret.resize(static_cast<size_type>(total_read + block_size));
  }
  ret.resize(static_cast<size_type>(total_read));
  return ret;
}

template <typename Output, typename = void>
struct default_converter
{
private:
  // Overload for containers which have the following properties:
  // - meta::default_constructible
  // - meta::copyable or meta::movable
  // - T::resize(T::size_type)
  template <typename T,
            typename R = meta::output_range<Output, typename T::element_type>,
            typename SizeType = typename R::size_type,
            typename = std::enable_if_t<
                meta::is_default_constructible<R>::value &&
                // Keep those in C++17 as well, fill_resizable_container
                // relies on NRVO, not on Guaranteed Copy Elision.
                (meta::is_copyable<R>::value || meta::is_movable<R>::value) &&
                meta::is_detected<meta::detected::member_functions::resize,
                                  R&,
                                  SizeType>::value>>
  static R create_impl(T& is, meta::priority_tag<1>)
  {
    return fill_resizable_output_range<R>(is, meta::priority_tag<1>{});
  }

  // Fallback overload, non-associative containers which:
  // - can be constructed with an Iterator range
  // - are copy or move constructible (pre C++17)
  template <typename T,
            typename = codecs::input_source<T>,
            typename R = Output,
            typename Iterator = meta::iterator_t<codecs::basic_input_range<T>>,
            typename = std::enable_if_t<
  // Guaranteed copy-elision
#if MGS_HAS_CPP17
                (meta::is_move_constructible<R>::value ||
                 meta::is_copy_constructible<R>::value) &&
#endif
                meta::is_constructible_from<R, Iterator, Iterator>::value &&
                // Associative containers' iterator-range constructors are not
                // SFINAE-friendly...
                !meta::is_detected<meta::detected::types::key_type, R>::value>>
  static R create_impl(T is, meta::priority_tag<0>)
  {
    auto input_range = make_input_range(is);
    return R(input_range.begin(), input_range.end());
  }

public:
  template <typename T>
  static auto create(codecs::input_source<T> is)
      -> decltype(create_impl(std::move(is), meta::priority_tag<1>{}))
  {
    return create_impl(is, meta::priority_tag<1>{});
  }
};

template <typename C, std::size_t N>
struct default_converter<std::array<C, N>>
{
  template <typename T>
  static std::array<C, N> create(codecs::input_source<T, C*> is)
  {
    std::array<C, N> ret;

    auto const res = detail::read_at_most(is, ret.data(), N);
    if (res.second < N)
      throw exceptions::unexpected_eof_error("output buffer is too large");
    if (detail::read_at_most(is, ret.data(), 1).second != 0)
      throw exceptions::unexpected_eof_error("output buffer is too small");
    return ret;
  }
};
}
}
