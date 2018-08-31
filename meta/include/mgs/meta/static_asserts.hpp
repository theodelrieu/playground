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
template <typename Requirement, typename T, typename = void>
struct trigger_static_asserts;

template <typename Requirement>
struct trigger_static_asserts<Requirement, std::tuple<>>
{
  using _ = typename Requirement::static_assert_t;
};

template <typename Requirement, typename Requirements, typename Seq>
struct trigger_static_asserts_impl;

template <typename Requirement, typename... Requirements, std::size_t... N>
struct trigger_static_asserts_impl<Requirement,
                                   std::tuple<Requirements...>,
                                   std::index_sequence<N...>>
{
  using _ =
      std::tuple<typename Requirement::static_assert_t,
                 typename std::tuple_element_t<N, std::tuple<Requirements...>>::
                     static_assert_t...>;
};

template <typename Requirement, typename... Requirements>
struct trigger_static_asserts<Requirement,
                              std::tuple<Requirements...>,
                              std::enable_if_t<sizeof...(Requirements) != 0>>
  : trigger_static_asserts_impl<
        Requirement,
        std::tuple<Requirements...>,
        decltype(std::make_index_sequence<sizeof...(Requirements)>())>
{
};
}

template <typename Requirement>
struct trigger_static_asserts
{
  using _ = typename detail::trigger_static_asserts<
      Requirement,
      typename Requirement::requirements>::_{};
};
}
}
}
