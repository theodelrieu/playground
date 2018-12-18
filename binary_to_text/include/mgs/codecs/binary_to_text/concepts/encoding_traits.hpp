#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

#include <mgs/codecs/binary_to_text/detail/detected/static_data_members/alphabet.hpp>
#include <mgs/codecs/binary_to_text/detail/detected/static_data_members/padding_character.hpp>
#include <mgs/codecs/binary_to_text/detail/detected/static_data_members/padding_policy.hpp>
#include <mgs/codecs/binary_to_text/detail/detected/static_member_functions/index_of.hpp>
#include <mgs/codecs/binary_to_text/detail/math.hpp>
#include <mgs/codecs/binary_to_text/padding_policy.hpp>
#include <mgs/meta/detected.hpp>

// template <typename T>
// concept EncodingTraits =
//  std::is_array<decltype(T::alphabet)>::value &&
//  Same<char const, std::remove_extent_t<decltype(T::alphabet)>> &&
//  is_power_of_2<sizeof(T::alphabet)>() &&
//  Same<padding_policy const, decltype(T::padding_policy) const> &&
//  (Same<char const, decltype(T::padding_character) const> || T::padding_policy
//  == padding_policy::none) && requires (char c) {
//    Same<int, T::index_of(c)>;
//  };

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
template <typename T, typename = void>
struct is_encoding_traits : std::false_type
{
};

template <typename T>
struct is_encoding_traits<
    T,
    std::enable_if_t<meta::is_detected_convertible<
                         padding_policy,
                         detail::detected::static_data_members::padding_policy,
                         T>::value &&
                     std::is_array<meta::detected_t<
                         detail::detected::static_data_members::alphabet,
                         T>>::value>>
{
private:
  using alphabet_t = detail::detected::static_data_members::alphabet<T>;

  static constexpr auto const is_alphabet = 
      std::is_array<alphabet_t>::value &&
      std::is_same<char const, std::remove_extent_t<alphabet_t>>::value;

  static constexpr auto const is_valid_alphabet_size =
      detail::is_power_of_2<sizeof(alphabet_t)>();

  static constexpr auto const is_valid_padding_character =
      meta::is_detected_convertible<
          char,
          detail::detected::static_data_members::padding_character,
          T>::value;

  static constexpr auto const has_index_of = meta::is_detected_exact<
      int,
      detail::detected::static_member_functions::index_of,
      T,
      char>::value;

public:
  using requirements = std::tuple<>;

  static constexpr auto const value =
      is_alphabet && is_valid_alphabet_size &&
      (is_valid_padding_character ||
       T::padding_policy == padding_policy::none) &&
      has_index_of;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not an EncodingTraits");

    static_assert(is_alphabet, "T::alphabet must be a 'char const[]'");
    static_assert(is_valid_alphabet_size, "sizeof(T::alphabet) must be a power of 2");
    static_assert(
        is_valid_padding_character || T::padding_policy == padding_policy::none,
        "T::padding_character must be convertible to char when "
        "T::padding_policy is not padding_policy::none");
    static_assert(has_index_of, "Invalid or missing function: 'int T::has_index_of(char)'");
    return 1;
  }
};

template <typename T, typename = std::enable_if_t<is_encoding_traits<T>::value>>
using EncodingTraits = T;
}
}
}
}
}
