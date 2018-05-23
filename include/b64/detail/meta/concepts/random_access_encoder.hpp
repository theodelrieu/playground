#pragma once

#include <type_traits>

#include <b64/detail/meta/aliases.hpp>
#include <b64/detail/meta/concepts/bidirectional_encoder.hpp>
#include <b64/detail/meta/detected.hpp>

// concept RandomAccessEncoder = requires(T const& v) {
//   requires BidirectionalEncoder<T>;
//   v.pos();
//   requires { v.pos() } -> typename T::difference_type;
// }
namespace b64
{
namespace detail
{
template <typename T, typename = void>
struct is_random_access_encoder : std::false_type
{
};

template <typename T>
struct is_random_access_encoder<
    T,
    std::enable_if_t<is_bidirectional_encoder<T>::value>>
{
private:
  using difference_type = difference_type_t<T>;

public:
  static auto constexpr value =
      is_detected_exact<difference_type, pos_function_t, T const&>::value;
};
}
}
