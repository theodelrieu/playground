#pragma once

#include <iterator>
#include <array>
#include <cstddef>
#include <type_traits>
#include <utility>

#include <mgs/meta/concepts/container/container.hpp>
#include <mgs/exceptions/exception.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace detail
{
template <typename T, typename = void>
struct default_converter;

template <typename T>
struct default_converter<
    T,
    std::enable_if_t<meta::concepts::container::is_container<T>::value>>
{
  template <typename Iterator,
            typename = std::enable_if_t<
                std::is_constructible<T, Iterator, Iterator>::value>>
  static T create(Iterator begin, Iterator end)
  {
    return T(std::move(begin), std::move(end));
  }
};

template <typename C, std::size_t N>
struct default_converter<std::array<C, N>>
{
  template <typename Iterator,
            typename = std::enable_if_t<std::is_convertible<
                typename std::iterator_traits<Iterator>::value_type,
                C>::value>>
  static std::array<C, N> create(Iterator begin, Iterator end)
  {
    std::array<C, N> ret;

    for (std::size_t i = 0; i < N; ++i)
      ret[i] = *begin++;
    if (begin != end)
      throw exceptions::exception("output buffer is too small");
    return ret;
  }
};
}
}
}
}
