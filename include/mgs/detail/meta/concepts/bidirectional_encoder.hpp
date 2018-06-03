#pragma once

#include <type_traits>

#include <mgs/detail/meta/aliases.hpp>
#include <mgs/detail/meta/concepts/encoder.hpp>
#include <mgs/detail/meta/detected.hpp>

// concept BidirectionalEncoder = requires(T& u) {
//   requires Encoder<T>;
//   requires(typename T::difference_type n) {
//     u.seek_backward(n);
//     requires { u.seek_backward(n) } -> void;
//   }
// }
namespace mgs
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
