#pragma once
#include <cstring>

#include <array>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>

#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/meta/call_std/begin.hpp>
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
  template <typename InputAdapter,
            typename U = T,
            typename Iterator = meta::result_of_begin<InputAdapter&>,
            typename = std::enable_if_t<
                (std::is_copy_constructible<U>::value ||
                 std::is_move_constructible<U>::value) &&
                std::is_constructible<U, Iterator, Iterator>::value &&
                // Associative containers' range constructors are not
                // SFINAE-friendly...
                !meta::is_detected<meta::detected::types::key_type, U>::value>>
  static T create(InputAdapter& adapter)
  {
    return T(adapter.begin(), adapter.end());
  }
};

template <typename C, std::size_t N>
struct default_converter<std::array<C, N>>
{
  template <typename InputAdapter,
            typename Iterator = meta::result_of_begin<InputAdapter&>,
            typename = std::enable_if_t<std::is_convertible<
                typename std::iterator_traits<Iterator>::value_type,
                C>::value>>
  static std::array<C, N> create(InputAdapter& adapter)
  {
    std::array<C, N> ret;
    // TODO optimize this one

    auto begin = adapter.begin();
    auto end = adapter.end();
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

template <>
struct default_converter<std::string>
{
  template <typename InputAdapter>
  static std::string create(InputAdapter& adapter)
  {
    std::string s;

    while (auto const bsize = adapter.block().size())
    {
      auto const old_size = s.size();
      s.resize(old_size + bsize);
      std::memcpy(&*(s.begin() + old_size), adapter.block().begin(), bsize);
      adapter.read_block();
    }
    return s;
  }
};
}
}
}
}
