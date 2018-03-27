#pragma once

#include <cstdint>
#include <type_traits>

#include <b64/detail/meta/concepts/input_source.hpp>
#include <b64/detail/meta/detected.hpp>

// concept InputProcessor = requires(T const& processor) {
//   requires InputSource<T>;
//   requires InputSource<typename T::input_source>;
//   requires Constructible<T, typename T::input_source>;
// }

namespace b64
{
namespace detail
{
template <typename T, typename = void>
struct is_input_processor : std::false_type
{
};

template <typename T>
using input_source_t = typename T::input_source;

template <typename T>
struct is_input_processor<
    T,
    std::enable_if_t<
        is_input_source<T>::value &&
        std::is_constructible<T, detected_t<input_source_t, T>>::value>>
    : std::true_type
{
};
}
}
