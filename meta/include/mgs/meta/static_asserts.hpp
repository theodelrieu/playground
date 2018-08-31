#pragma once

#include <tuple>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace detail
{
template <typename T, typename SubRequirements, typename = void>
struct collect_requirements;

template <typename T>
struct collect_requirements<T, std::tuple<>>
{
  using type = std::tuple<T>;
};

template <typename T, typename... SubRequirements>
struct collect_requirements<T,
                            std::tuple<SubRequirements...>,
                            std::enable_if_t<sizeof...(SubRequirements) != 0>>
{
  using type = decltype(
      std::tuple_cat(std::tuple<T>{},
                     typename collect_requirements<
                         SubRequirements,
                         typename SubRequirements::requirements>::type{}...));
};

template <typename Requirement>
struct collect_failed_requirements
{
  using AllRequirements =
      typename collect_requirements<Requirement,
                                    typename Requirement::requirements>::type;
};

template <typename Requirement>
using collect_failed_requirements_t =
    typename collect_failed_requirements<Requirement>::type;
}
}
}
}
