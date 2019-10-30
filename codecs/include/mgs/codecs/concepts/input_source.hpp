#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/codecs/detected/member_functions/read.hpp>
#include <mgs/meta/concepts/convertible_to.hpp>
#include <mgs/meta/concepts/copyable.hpp>
#include <mgs/meta/concepts/default_constructible.hpp>
#include <mgs/meta/concepts/output_iterator.hpp>
#include <mgs/meta/concepts/same_as.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/element_type.hpp>
#include <mgs/meta/ssize_t.hpp>

namespace mgs
{
namespace codecs
{
template <typename T,
          typename O =
              meta::detected_t<meta::detected::types::element_type, T>*>
struct is_input_source
{
private:
  using element_type = meta::detected_t<meta::detected::types::element_type, T>;

  static constexpr auto const has_read_member_function =
      meta::is_convertible_to<meta::detected_t<detected::member_functions::read,
                                               std::add_lvalue_reference_t<T>,
                                               O,
                                               meta::ssize_t>,
                              meta::ssize_t>::value;

public:
  using requirements = std::tuple<meta::is_copyable<T>,
                                  meta::is_output_iterator<O, element_type>>;

  static constexpr auto const value =
      meta::is_copyable<T>::value &&
      meta::is_output_iterator<O, element_type>::value &&
      has_read_member_function &&
      meta::is_default_constructible<element_type>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model codecs::input_source");
    static_assert(!meta::is_same_as<O, meta::nonesuch*>::value,
                  "T::element_type does not exist");
    static_assert(meta::is_default_constructible<element_type>::value,
                  "T::element_type does not model meta::default_constructible");
    static_assert(
        has_read_member_function,
        "invalid or missing member function: "
        "'meta::convertible_to<mgs::ssize_t> T::read(O, mgs::ssize_t)''");
    return 1;
  }
};

template <typename T>
constexpr auto is_input_source_v = is_input_source<T>::value;

template <typename T, typename = std::enable_if_t<is_input_source<T>::value>>
using input_source = T;
}
}
