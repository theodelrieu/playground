#pragma once

#include <utility>

namespace b64
{
namespace detail
{
template <typename T>
using value_type_t = typename T::value_type;

template <typename T>
using difference_type_t = typename T::difference_type;

template <typename T>
using pointer_t = typename T::pointer;

template <typename T>
using reference_t = typename T::reference;

template <typename T>
using iterator_category_t = typename T::iterator_category;

template <typename T>
using next_char_t = decltype(std::declval<T const&>().next_char());

template <typename T>
using eof_t = decltype(std::declval<T const&>().eof());

template <typename T>
using category_t = typename T::category;
}
}
