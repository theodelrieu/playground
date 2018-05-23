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
using underlying_iterator_t = typename T::underlying_iterator;

template <typename T>
using underlying_sentinel_t = typename T::underlying_sentinel;

template <typename T>
using iterator_category_t = typename T::iterator_category;

template <typename T, typename U>
using substraction_t = decltype(std::declval<T>() - std::declval<U>());

template <typename T, typename U>
using addition_t = decltype(std::declval<T>() + std::declval<U>());

template <typename T, typename U>
using addition_assignment_t = decltype(std::declval<T>() += std::declval<U>());

template <typename T, typename U>
using array_subscript_t = decltype(std::declval<T>()[std::declval<U>()]);

template <typename T, typename U>
using substraction_assignment_t =
    decltype(std::declval<T>() -= std::declval<U>());

template <typename T>
using post_increment_t = decltype(std::declval<T>()++);

template <typename T>
using pre_increment_t = decltype(++std::declval<T>());

template <typename T>
using post_decrement_t = decltype(std::declval<T>()--);

template <typename T>
using pre_decrement_t = decltype(--std::declval<T>());

template <typename T, typename U>
using less_than_t = decltype(std::declval<T>() < std::declval<U>());

template <typename T, typename U>
using less_or_equal_than_t = decltype(std::declval<T>() <= std::declval<U>());

template <typename T, typename U>
using greater_than_t = decltype(std::declval<T>() > std::declval<U>());

template <typename T, typename U>
using greater_or_equal_than_t =
    decltype(std::declval<T>() >= std::declval<U>());

template <typename T>
using get_function_t = decltype(std::declval<T>().get());

template <typename T, typename... Args>
using seek_forward_function_t =
    decltype(std::declval<T>().seek_forward(std::declval<Args>()...));

template <typename T, typename... Args>
using seek_backward_function_t =
    decltype(std::declval<T>().seek_backward(std::declval<Args>()...));
}
}
