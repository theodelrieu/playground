#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/meta/void_t.hpp>

namespace mgs
{
namespace meta
{
namespace detail
{
// SFINAE correct version of std::iterator_traits, see:
// https://cplusplus.github.io/LWG/issue2951
template <typename T, typename = void>
struct iterator_traits
{
};

template <typename T>
struct iterator_traits<T, std::enable_if_t<!std::is_pointer<T>::value>>
  : std::iterator_traits<T>
{
};

template <typename T>
struct iterator_traits<T*, std::enable_if_t<std::is_object<T>::value>>
  : std::iterator_traits<T*>
{
};
}

template <typename T>
struct iterator_traits : detail::iterator_traits<T>
{
};
}
}
