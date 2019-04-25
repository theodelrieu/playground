#pragma once

namespace mgs
{
namespace meta
{
namespace detail
{
template <typename... Ts>
struct make_void
{
  using type = void;
};
}

template <typename... Ts>
using void_t = typename detail::make_void<Ts...>::type;
}
}
