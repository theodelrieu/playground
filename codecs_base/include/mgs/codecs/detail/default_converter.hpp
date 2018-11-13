#pragma once

#include <array>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

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

    auto const nb_read = adapter.write(ret.begin(), N);
    if (nb_read != N)
      throw exceptions::unexpected_eof_error("output buffer is too large");
    if (adapter.write(ret.begin(), 1) != 0)
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
    constexpr auto block_size = 256;
    std::string s(block_size, 0);

    auto total_read = 0;
    for (auto nb_read = adapter.write(s.begin(), block_size); nb_read != 0;
         nb_read = adapter.write(s.begin(), block_size))
    {
      total_read += nb_read;
      s.resize(total_read + block_size);
    }
    s.resize(total_read);
    return s;
  }
};

// TODO concept for byte_integral
template <typename T, typename Alloc>
struct default_converter<
    std::vector<T, Alloc>,
    std::enable_if_t<std::is_integral<T>::value &&
                     !std::is_same<T, bool>::value && sizeof(T) == 1>>
{
private:
  using Ret = std::vector<T, Alloc>;

public:
  template <typename InputAdapter>
  static Ret create(InputAdapter& adapter)
  {
    constexpr auto block_size = 256;

    Ret v(block_size);
    auto total_read = 0;
    for (auto nb_read = adapter.write(v.begin(), block_size); nb_read != 0;
         nb_read = adapter.write(v.begin(), block_size))
    {
      total_read += nb_read;
      v.resize(total_read + block_size);
    }
    v.resize(total_read);
    return v;
  }
};
}
}
}
}
