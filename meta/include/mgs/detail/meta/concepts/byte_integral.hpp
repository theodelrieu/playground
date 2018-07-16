#pragma once

// concept ByteIntegral = requires(T val) {
//   requires Integral<T>;
//   requires sizeof(T) == 1;
// }

#include <type_traits>

namespace mgs
{
inline namespace v1
{
namespace detail
{
template <typename T>
struct is_byte_integral
    : std::integral_constant<bool, std::is_integral<T>::value && sizeof(T) == 1>
{
};
}
}
}
