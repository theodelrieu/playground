#pragma once

#include <type_traits>

#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/substraction.hpp>

// http://en.cppreference.com/w/cpp/experimental/ranges/iterator/SizedSentinel
namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
namespace iterator
{
template <typename T, typename Iterator>
struct is_sized_sentinel
{
private:
  using difference_type =
      detected_t<detected::operators::substraction, T const&, Iterator const&>;

public:
  static constexpr auto value =
      is_sentinel<T, Iterator>::value &&
      std::is_integral<difference_type>::value &&
      std::is_signed<difference_type>::value &&
      std::is_same<difference_type,
                   detected_t<detected::operators::substraction,
                              Iterator const&,
                              T const&>>::value;
};
}
}
namespace iterator_concepts = concepts::iterator;
}
}
}
