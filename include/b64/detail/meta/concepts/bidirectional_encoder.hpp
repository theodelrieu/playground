#pragma once

#include <type_traits>

#include <b64/detail/meta/aliases.hpp>
#include <b64/detail/meta/concepts/encoder.hpp>
#include <b64/detail/meta/detected.hpp>

// concept BidirectionalEncoder = requires(T const& v) {
//   requires Encoder<T>;
//   requires(typename T::difference_type n) {
//     v.seek_backward(n);
//     requires { v.seek_backward(n) } -> void;
//   }
// }
namespace b64
{
namespace detail
{
template <typename T, typename = void>
struct is_bidirectional_encoder : std::false_type
{
};

template <typename T>
struct is_bidirectional_encoder<T, std::enable_if_t<is_encoder<T>::value>>
{
private:
  using difference_type = difference_type_t<T>;

public:
  static auto constexpr value =
      is_detected_exact<void, seek_backward_function_t, T&, difference_type>::
          value;
};
}
}
