#pragma once

#include <type_traits>

#include <mgs/meta/aliases.hpp>
#include <mgs/meta/concepts/sentinel.hpp>
#include <mgs/meta/detected.hpp>

// http://en.cppreference.com/w/cpp/experimental/ranges/iterator/SizedSentinel
namespace mgs
{
inline namespace v1
{
namespace meta
{
template <typename T, typename Iterator>
struct is_sized_sentinel
{
private:
  using difference_type = detected_t<substraction_t, T, Iterator>;

public:
  static constexpr auto value =
      is_sentinel<T, Iterator>::value &&
      std::is_integral<difference_type>::value &&
      std::is_signed<difference_type>::value &&
      std::is_same<difference_type,
                   detected_t<substraction_t, Iterator, T>>::value;
};
}
}
}
