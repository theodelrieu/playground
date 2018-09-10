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

template <typename... U>
struct filter_requirements_impl;

template <>
struct filter_requirements_impl<>
{
  using type = std::tuple<>;
};

template <typename Requirement, typename... Tail>
struct filter_requirements_impl<Requirement, Tail...>
{
  using type = decltype(
      std::tuple_cat(std::conditional_t<!Requirement::value,
                                        std::tuple<Requirement>,
                                        std::tuple<>>{},
                     typename filter_requirements_impl<Tail...>::type{}));
};

template <typename T>
struct filter_requirements;

template <typename... Requirements>
struct filter_requirements<std::tuple<Requirements...>>
{
  using type = typename filter_requirements_impl<Requirements...>::type;
};

template <typename Requirement>
struct collect_failed_requirements
{
  using AllRequirements =
      typename collect_requirements<Requirement,
                                    typename Requirement::requirements>::type;

  using type = typename filter_requirements<AllRequirements>::type;
};

template <typename Requirement, typename T>
struct collect_static_asserts_impl;

template <typename Requirement, typename... FailedRequirements>
struct collect_static_asserts_impl<Requirement, std::tuple<FailedRequirements...>>
{
  using type = std::tuple<typename Requirement::static_assert_t,
                          typename FailedRequirements::static_assert_t...>;
};

template <typename Requirement>
struct collect_static_asserts
{
  using failed_requirements =
      typename collect_failed_requirements<Requirement>::type;

  using type = typename collect_static_asserts_impl<Requirement,
                                                    failed_requirements>::type;
};

template <typename Head>
constexpr int print_static_asserts()
{
  return Head::trigger();
}

template <typename Head, typename...Tail>
constexpr auto print_static_asserts() -> std::enable_if_t<sizeof...(Tail) != 0, int>
{
  return print_static_asserts<Tail...>() + Head::trigger();
}

template <typename Requirements>
struct trigger_static_asserts;

template <typename ...Requirements>
struct trigger_static_asserts<std::tuple<Requirements...>>
{
  static constexpr auto trigger()
  {
    return print_static_asserts<Requirements...>();
  }
};
}

template <typename Requirement>
constexpr int trigger_static_asserts()
{
  return detail::trigger_static_asserts<
      typename detail::collect_static_asserts<Requirement>::type>::trigger();
}
}
}
}
