#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/meta/aliases/types/difference_type.hpp>
#include <mgs/meta/aliases/types/iterator_category.hpp>
#include <mgs/meta/aliases/types/pointer.hpp>
#include <mgs/meta/aliases/types/reference.hpp>
#include <mgs/meta/aliases/types/value_type.hpp>
#include <mgs/meta/detected.hpp>

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
template <typename T>
struct is_iterator_traits : std::false_type
{
};

template <typename T>
struct is_iterator_traits<std::iterator_traits<T>>
{
private:
  using traits = std::iterator_traits<T>;

public:
  static constexpr auto value =
      is_detected<aliases::types::value_type, traits>::value &&
      is_detected<aliases::types::difference_type, traits>::value &&
      is_detected<aliases::types::pointer, traits>::value &&
      is_detected<aliases::types::iterator_category, traits>::value &&
      is_detected<aliases::types::reference, traits>::value;
};
}
}
namespace iterator_concepts = concepts::iterator;
}
}
}
