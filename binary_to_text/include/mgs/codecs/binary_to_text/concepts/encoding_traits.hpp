#pragma once

#include <type_traits>
#include <utility>

#include <mgs/codecs/binary_to_text/detail/detected/static_data_members/alphabet.hpp>
#include <mgs/codecs/binary_to_text/detail/detected/static_data_members/nb_decoded_bytes.hpp>
#include <mgs/codecs/binary_to_text/detail/detected/static_data_members/nb_encoded_bytes.hpp>
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
//  Integral<T::nb_encoded_bytes> && T::nb_encoded_bytes > 0 &&
//  Integral<T::nb_decoded_bytes> && T::nb_decoded_bytes> && 
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
    std::enable_if_t<
        meta::is_detected_convertible<
            padding_policy,
            detail::detected::static_data_members::padding_policy,
            T>::value &&
        std::is_array<
            meta::detected_t<detail::detected::static_data_members::alphabet,
                             T>>::value &&
        std::is_integral<meta::detected_t<
            detail::detected::static_data_members::nb_encoded_bytes,
            T>>::value &&
        std::is_integral<meta::detected_t<
            detail::detected::static_data_members::nb_decoded_bytes,
            T>>::value>>
{
private:
  using alphabet_t = detail::detected::static_data_members::alphabet<T>;
  using nb_encoded_bytes_t =
      detail::detected::static_data_members::nb_encoded_bytes<T>;
  using nb_decoded_bytes_t =
      detail::detected::static_data_members::nb_decoded_bytes<T>;

public:
  static constexpr auto const value =
      std::is_array<alphabet_t>::value &&
      std::is_same<char const, std::remove_extent_t<alphabet_t>>::value &&
      detail::is_power_of_2<sizeof(alphabet_t)>() && T::nb_encoded_bytes > 0 &&
      T::nb_decoded_bytes > 0 &&
      (meta::is_detected_convertible<
           char,
           detail::detected::static_data_members::padding_character,
           T>::value ||
       T::padding_policy == padding_policy::none) &&
      meta::is_detected_exact<
          int,
          detail::detected::static_member_functions::index_of,
          T,
          char>::value;
};

// TODO better name/design
template <typename T>
struct static_asserts
{
  using detected_alphabet =
      meta::detected_t<detail::detected::static_data_members::alphabet, T>;

  using detected_nb_encoded_bytes =
      meta::detected_t<detail::detected::static_data_members::nb_encoded_bytes,
                       T>;

  using detected_nb_decoded_bytes =
      meta::detected_t<detail::detected::static_data_members::nb_decoded_bytes,
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

  static_assert(
      std::is_integral<detected_nb_encoded_bytes>::value,
      "nb_encoded_bytes is missing or not of an integral type");

  static_assert(std::is_integral<detected_nb_decoded_bytes>::value,
                "nb_decoded_bytes is missing or not of an integral type");

  static_assert(meta::is_detected_exact<
                    char const,
                    detail::detected::static_data_members::padding_character,
                    T const>::value ||
                    T::padding_policy == padding_policy::none,
                "padding_character is missing, set padding_policy to "
                "padding_policy::none if padding is not required");

  static_assert(meta::is_detected_exact<
                    int,
                    detail::detected::static_member_functions::index_of,
                    T,
                    char>::value,
                "static method not found: int index_of(char)");

  using trigger = decltype(sizeof(T));
};

template <typename T>
using trigger_static_asserts = typename static_asserts<T>::trigger;
}
}
}
}
}
