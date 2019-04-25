#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/sentinel.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/substraction.hpp>
#include <mgs/meta/iter_difference_t.hpp>

// http://en.cppreference.com/w/cpp/experimental/ranges/SizedSentinel
namespace mgs
{
namespace meta
{
namespace concepts
{
template <typename T, typename Iterator>
struct is_sized_sentinel
{
private:
  using sentinel_lvalue_const_ref =
      std::add_lvalue_reference_t<std::add_const_t<T>>;
  using iterator_lvalue_const_ref =
      std::add_lvalue_reference_t<std::add_const_t<Iterator>>;

  // no need to check nonesuch, is_sentinel will fail anyway.
  using difference_type = detected_t<meta::iter_difference_t, Iterator>;

  static constexpr auto const has_it_sent_substraction =
      is_detected_exact<difference_type,
                        detected::operators::substraction,
                        iterator_lvalue_const_ref,
                        sentinel_lvalue_const_ref>::value;

  static constexpr auto const has_sent_it_substraction =
      is_detected_exact<difference_type,
                        detected::operators::substraction,
                        sentinel_lvalue_const_ref,
                        iterator_lvalue_const_ref>::value;

public:
  using requirements = std::tuple<is_sentinel<T, Iterator>>;

  static constexpr auto value = is_sentinel<T, Iterator>::value &&
                                has_it_sent_substraction &&
                                has_sent_it_substraction;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not a SizedSentinel<Iterator>");
    static_assert(has_it_sent_substraction,
                  "Invalid or missing operator: "
                  "'std::iter_difference_t<Iterator> "
                  "operator-(Iterator const&, T const&)'");
    static_assert(has_sent_it_substraction,
                  "Invalid or missing operator: "
                  "'std::iter_difference_t<Iterator> "
                  "operator-(T const&, Iterator const&)'");
    return 1;
  }
};

template <typename T, typename Iterator>
constexpr auto is_sized_sentinel_v = is_sized_sentinel<T, Iterator>::value;
}

template <typename T,
          typename Iterator,
          typename =
              std::enable_if_t<concepts::is_sized_sentinel<T, Iterator>::value>>
using SizedSentinel = T;
}
}
