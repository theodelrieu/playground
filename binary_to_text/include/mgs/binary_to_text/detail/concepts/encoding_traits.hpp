#pragma once

#include <type_traits>
#include <utility>

#include <mgs/binary_to_text/detail/math.hpp>
#include <mgs/binary_to_text/padding_policy.hpp>
#include <mgs/meta/detected.hpp>

// template <typename T>
// concept EncodingTraits =
//  std::is_array<decltype(T::alphabet)>::value &&
//  std::is_array<decltype(T::encoding_name)>::value &&
//  Same<char const*, std::decay_t<decltype(T::alphabet)>> &&
//  Same<char const*, std::decay_t<decltype(T::encoding_name)>> &&
//  pow<2, log2<sizeof(T::alphabet)>>() == sizeof(T::alphabet) &&
//  UnsignedIntegral<T::nb_input_bytes> &&
//  UnsignedIntegral<T::nb_output_bytes> &&
//  ConvertibleTo<padding_policy, decltype(T::padding_policy)> &&
//  (ConvertibleTo<char, T::padding_character> || T::padding_policy == padding_policy::none) &&
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
template <typename T>
using alphabet_t = decltype(T::alphabet);

template <typename T>
using encoding_name_t = decltype(T::encoding_name);

template <typename T>
using nb_input_bytes_t = decltype(T::nb_input_bytes);

template <typename T>
using nb_output_bytes_t = decltype(T::nb_output_bytes);

template <typename T>
using padding_policy_t = decltype(T::padding_policy);

template <typename T, typename... Args>
using find_char_function_t =
    decltype(T::find_char(std::declval<Args>()...));

template <typename T>
using padding_character_t = decltype(T::padding_character);

template <typename T, typename = void>
struct is_encoding_traits : std::false_type
{
};

template <typename T>
struct is_encoding_traits<
    T,
    std::enable_if_t<
        meta::is_detected_convertible<padding_policy, padding_policy_t, T>::
            value &&
        std::is_array<meta::detected_t<alphabet_t, T>>::value &&
        std::is_array<meta::detected_t<encoding_name_t, T>>::value &&
        std::is_unsigned<meta::detected_t<nb_input_bytes_t, T>>::value &&
        std::is_unsigned<meta::detected_t<nb_output_bytes_t, T>>::value>>
{
private:
  using alphabet_t = decltype(T::alphabet);
  using encoding_name_t = decltype(T::encoding_name);
  using nb_input_bytes_t = decltype(T::nb_input_bytes);
  using nb_output_bytes_t = decltype(T::nb_output_bytes);

public:
  static constexpr auto const value =
      std::is_same<char const*, std::decay_t<encoding_name_t>>::value &&
      std::is_same<char const*, std::decay_t<alphabet_t>>::value &&
      pow<2, log2<sizeof(T::alphabet)>>() == sizeof(T::alphabet) &&
      (meta::is_detected_convertible<char, padding_character_t, T>::value ||
       T::padding_policy == padding_policy::none) &&
      meta::is_detected_exact<char const*, find_char_function_t, T, char>::
          value;
};

template <typename T>
struct static_asserts
{
  static_assert(
      meta::is_detected_convertible<padding_policy, padding_policy_t, T>::value,
      "padding_policy is missing or of the wrong type");

  static_assert(std::is_array<meta::detected_t<alphabet_t, T>>::value &&
                    std::is_same<std::decay_t<meta::detected_t<alphabet_t, T>>,
                                 char const*>::value,
                "alphabet is missing, or not a char const[]");

  static_assert(pow<2, log2<sizeof(meta::detected_t<alphabet_t, T>)>()>() ==
                    sizeof(meta::detected_t<alphabet_t, T>),
                "alphabet length must be a power of 2");

  static_assert(
      std::is_array<meta::detected_t<encoding_name_t, T>>::value &&
          std::is_same<std::decay_t<meta::detected_t<encoding_name_t, T>>,
                       char const*>::value,
      "encoding_name is missing, or not a char const[]");

  static_assert(
      std::is_unsigned<meta::detected_t<nb_input_bytes_t, T>>::value,
      "nb_input_bytes is missing or not of an unsigned integral type");

  static_assert(
      std::is_unsigned<meta::detected_t<nb_output_bytes_t, T>>::value,
      "nb_output_bytes is missing or not of an unsigned integral type");

  static_assert(
      meta::is_detected_convertible<char, padding_character_t, T>::value ||
          T::padding_policy == padding_policy::none,
      "padding_character is missing, set padding_policy to none if "
      "padding is not required");

  static_assert(
      meta::is_detected_exact<char const*, find_char_function_t, T, char>::
          value,
      "static method not found: char const* find_char(char)");

  using trigger = decltype(sizeof(T));
};

template <typename T>
using trigger_static_asserts = typename static_asserts<T>::trigger;
}
}
}
}
