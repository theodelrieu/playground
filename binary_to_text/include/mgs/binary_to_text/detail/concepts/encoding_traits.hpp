#pragma once

#include <mgs/meta/detected.hpp>

// template <typename T>
// concept EncodingTraits =
//  std::is_array<decltype(T::alphabet)>::value &&
//  std::is_array<decltype(T::encoding_name)>::value &&
//  Same<char const*, std::decay_t<decltype(T::alphabet)>> &&
//  Same<char const*, std::decay_t<decltype(T::encoding_name)>> &&
//  sizeof(T::alphabet) % 2 == 0 &&
//  UnsignedIntegral<T::nb_input_bytes> &&
//  UnsignedIntegral<T::nb_output_bytes> &&
//  Same<padding_policy, decltype(T::padding_policy)> &&
//  (Same<char, T::padding_character> || T::padding_policy == padding_policy::none) &&
//  requires (char c) {
//    Same<char const*, T::find(c)>;
//  };

namespace mgs
{
inline namespace v1
{
namespace binary_to_text
{
namespace detail
{
// TODO is_encoding_traits
}
}
}
}
