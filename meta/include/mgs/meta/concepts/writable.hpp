#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/detected.hpp>
#include <mgs/meta/iter_reference_t.hpp>

namespace mgs
{
namespace meta
{
namespace concepts
{
namespace detail
{
// Too hard to implement with detected, use expression SFINAE instead
template <typename Out, typename T>
auto writable_first_requirement(Out&& o, T&& t)
    -> decltype(*o = std::forward<T>(t));

template <typename Out, typename T>
auto writable_second_requirement(Out&& o, T&& t)
    -> decltype(*std::forward<Out>(o) = std::forward<T>(t));

// Those last requirements ensure that Out's operator* does not return a prvalue
// (except for proxy types) since writing to a temporary value is unlikely to be
// the intended behavior.
//
// Casting the result of *o to iter_reference_t<Out> const&& does not have any
// effect if the result is a reference type (including rvalue references), due
// to reference collapsing.
//
// Example:
//
// struct It { std::string& operator*(); };
// struct It2 { std::string operator*(); };
//
// The result of the const_cast is:
//
// - for It, std::string&
// - for It2, std::string const&&
//
// Since there is no overload of std::string::operator= for const rvalue
// references, the check fails.
template <typename Out, typename T>
auto writable_third_requirement(Out&& o, T&& t) -> decltype(
    const_cast<iter_reference_t<Out> const&&>(*o) = std::forward<T>(t));

template <typename Out, typename T>
auto writable_fourth_requirement(Out&& o, T&& t)
    -> decltype(const_cast<iter_reference_t<Out> const&&>(
                    *std::forward<Out>(o)) = std::forward<T>(t));

template <typename Out, typename T>
using writable_requirements = decltype(
    writable_first_requirement(std::declval<Out>(), std::declval<T>()),
    writable_second_requirement(std::declval<Out>(), std::declval<T>()),
    writable_third_requirement(std::declval<Out>(), std::declval<T>()),
    writable_fourth_requirement(std::declval<Out>(), std::declval<T>()));
}

template <typename Out, typename T>
struct is_writable
{
  using requirements = std::tuple<>;

  static constexpr auto const value =
      is_detected<detail::writable_requirements, Out, T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value,
                  "Out does not model Writable<T>. Make sure Out::operator* "
                  "does not return by-value (unless Out is a proxy-type, in "
                  "this case you should provide const/rvalue/lvalue overloads");
    return 1;
  }
};

template <typename Out, typename T>
constexpr auto is_writable_v = is_writable<Out, T>::value;
}

template <typename Out,
          typename T,
          typename = std::enable_if_t<concepts::is_writable<Out, T>::value>>
using Writable = Out;
}
}
