#pragma once

#include <type_traits>
#include <utility>

#include <mgs/binary_to_text/detail/detected/static_data_members/alphabet.hpp>
#include <mgs/binary_to_text/detail/detected/static_data_members/encoding_name.hpp>
#include <mgs/binary_to_text/detail/detected/static_data_members/nb_input_bytes.hpp>
#include <mgs/binary_to_text/detail/detected/static_data_members/nb_output_bytes.hpp>
#include <mgs/binary_to_text/detail/detected/static_data_members/padding_character.hpp>
#include <mgs/binary_to_text/detail/detected/static_data_members/padding_policy.hpp>
#include <mgs/binary_to_text/detail/detected/static_member_functions/find_char.hpp>
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
//  Same<padding_policy const, decltype(T::padding_policy) const> &&
//  (Same<char const, decltype(T::padding_character) const> || T::padding_policy
//  == padding_policy::none) && requires (char c) {
//    Same<char const*, T::find(c)>;
//  };

namespace mgs
{
inline namespace v1
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
                         T>>::value &&
                     std::is_array<meta::detected_t<
                         detail::detected::static_data_members::encoding_name,
                         T>>::value &&
                     std::is_unsigned<meta::detected_t<
                         detail::detected::static_data_members::nb_input_bytes,
                         T>>::value &&
                     std::is_unsigned<meta::detected_t<
                         detail::detected::static_data_members::nb_output_bytes,
                         T>>::value>>
{
private:
  using alphabet_t = detail::detected::static_data_members::alphabet<T>;
  using encoding_name_t =
      detail::detected::static_data_members::encoding_name<T>;
  using nb_input_bytes_t =
      detail::detected::static_data_members::nb_input_bytes<T>;
  using nb_output_bytes_t =
      detail::detected::static_data_members::nb_output_bytes<T>;

public:
  static constexpr auto const value =
      std::is_same<char const*, std::decay_t<encoding_name_t>>::value &&
      std::is_same<char const*, std::decay_t<alphabet_t>>::value &&
      detail::pow<2, detail::log2<sizeof(alphabet_t)>>() ==
          sizeof(alphabet_t) &&
      (meta::is_detected_convertible<
           char,
           detail::detected::static_data_members::padding_character,
           T>::value ||
       T::padding_policy == padding_policy::none) &&
      meta::is_detected_exact<
          char const*,
          detail::detected::static_member_functions::find_char,
          T,
          char>::value;
};

// TODO better name/design
template <typename T>
struct static_asserts
{
  using detected_alphabet =
      meta::detected_t<detail::detected::static_data_members::alphabet, T>;

  using detected_encoding_name =
      meta::detected_t<detail::detected::static_data_members::encoding_name, T>;

  using detected_nb_input_bytes =
      meta::detected_t<detail::detected::static_data_members::nb_input_bytes,
                       T>;

  using detected_nb_output_bytes =
      meta::detected_t<detail::detected::static_data_members::nb_output_bytes,
                       T>;

  using detected_padding_policy =
      meta::detected_t<detail::detected::static_data_members::padding_policy,
                       T>;

  using detected_padding_character =
      meta::detected_t<detail::detected::static_data_members::padding_character,
                       T>;

  static_assert(meta::is_detected_exact<
                    padding_policy const,
                    detail::detected::static_data_members::padding_policy,
                    T const>::value,
                "padding_policy is missing or of the wrong type");

  static_assert(
      std::is_array<detected_alphabet>::value &&
          std::is_same<std::decay_t<detected_alphabet>, char const*>::value,
      "alphabet is missing, or not a char const[]");

  static_assert(detail::pow<2, detail::log2<sizeof(detected_alphabet)>()>() ==
                    sizeof(detected_alphabet),
                "alphabet length must be a power of 2");

  static_assert(std::is_array<detected_encoding_name>::value &&
                    std::is_same<std::decay_t<detected_encoding_name>,
                                 char const*>::value,
                "encoding_name is missing, or not a char const[]");

  static_assert(
      std::is_unsigned<detected_nb_input_bytes>::value,
      "nb_input_bytes is missing or not of an unsigned integral type");

  static_assert(
      std::is_unsigned<detected_nb_output_bytes>::value,
      "nb_output_bytes is missing or not of an unsigned integral type");

  static_assert(meta::is_detected_exact<
                    char const,
                    detail::detected::static_data_members::padding_character,
                    T const>::value ||
                    T::padding_policy == padding_policy::none,
                "padding_character is missing, set padding_policy to "
                "padding_policy::none if padding is not required");

  static_assert(meta::is_detected_exact<
                    char const*,
                    detail::detected::static_member_functions::find_char,
                    T,
                    char>::value,
                "static method not found: char const* find_char(char)");

  using trigger = decltype(sizeof(T));
};

template <typename T>
using trigger_static_asserts = typename static_asserts<T>::trigger;
}
}
}
}
