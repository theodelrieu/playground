#pragma once

#include <limits>
#include <tuple>
#include <type_traits>

// TODO define macros in config.hpp
#if __cplusplus >= 201703L
#include <cstddef> 
#endif

// template <typename T>
// concept Byte = std::Same<T, std::byte> ||
//   (std::Integral<T> &&
//   std::numeric_limits<T>::digits + std::numeric_limits<T>::is_signed ==
//   std::numeric_limits<unsigned char>::digits);

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace binary_to_text
{
namespace concepts
{
template <typename T>
struct is_byte
{
  using requirements = std::tuple<>;

  static constexpr auto const value =
#if __cplusplus >= 201703L
      std::is_same<std::byte, T>::value ||
#endif
      std::is_integral<T>::value &&
          std::numeric_limits<T>::digits + std::numeric_limits<T>::is_signed ==
              std::numeric_limits<unsigned char>::digits;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value,
                  "T must be Integral, and it must represent the same number "
                  "of bits as unsigned char");
    return -1;
  }
};

template <typename T, typename = std::enable_if_t<is_byte<T>::value>>
using Byte = T;
}
}
}
}
}
