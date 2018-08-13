#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/difference_type.hpp>
#include <mgs/meta/detected/types/iterator_category.hpp>
#include <mgs/meta/detected/types/pointer.hpp>
#include <mgs/meta/detected/types/reference.hpp>
#include <mgs/meta/detected/types/value_type.hpp>

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
      is_detected<detected::types::value_type, traits>::value &&
      is_detected<detected::types::difference_type, traits>::value &&
      is_detected<detected::types::pointer, traits>::value &&
      is_detected<detected::types::iterator_category, traits>::value &&
      is_detected<detected::types::reference, traits>::value;
};
}
};
}
}
}
