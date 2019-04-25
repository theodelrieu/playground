#pragma once

#include <type_traits>

#include <mgs/concepts/detail/detected/types/default_decoded_output.hpp>
#include <mgs/concepts/detail/detected/types/default_encoded_output.hpp>
#include <mgs/meta/detected.hpp>

namespace mgs
{
namespace concepts
{
namespace detail
{
template <typename T>
using default_encoded_output =
    meta::detected_t<detail::detected::types::default_encoded_output, T>;

template <typename T>
using default_decoded_output =
    meta::detected_t<detail::detected::types::default_decoded_output, T>;

template <typename T>
using default_encoded_output_lvalue_ref =
    std::add_lvalue_reference_t<default_encoded_output<T>>;

template <typename T>
using default_decoded_output_lvalue_ref =
    std::add_lvalue_reference_t<default_decoded_output<T>>;
}
}
}
