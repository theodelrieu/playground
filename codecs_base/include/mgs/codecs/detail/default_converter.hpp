#pragma once

#include <array>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>

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
struct default_converter
{
  template <typename Iterator,
            typename U = T,
            typename = std::enable_if_t<
                (std::is_copy_constructible<U>::value ||
                 std::is_move_constructible<U>::value) &&
                std::is_constructible<U, Iterator, Iterator>::value>>
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
    {
        // FIXME use unexpected_eof_error? Remove encoding_name from traits?
      if (begin == end)
        throw exceptions::exception("unexpected end of input");
      ret[i] = *begin++;
    }
    if (begin != end)
      throw exceptions::exception("output buffer is too small");
    return ret;
  }
};
}
}
}
}
