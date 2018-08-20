#pragma once

#include <array>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>

#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/key_type.hpp>

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
                std::is_constructible<U, Iterator, Iterator>::value &&
                // Associative containers' range constructors are not
                // SFINAE-friendly...
                !meta::is_detected<meta::detected::types::key_type, U>::value>>
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
      if (begin == end)
        throw exceptions::unexpected_eof_error("output buffer is too large");
      ret[i] = *begin++;
    }
    if (begin != end)
      throw exceptions::unexpected_eof_error("output buffer is too small");
    return ret;
  }
};
}
}
}
}
