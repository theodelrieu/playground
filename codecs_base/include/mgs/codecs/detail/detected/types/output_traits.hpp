#pragma once

#include <utility>

#include <mgs/codecs/output_traits_fwd.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace detail
{
namespace detected
{
namespace types
{
template <typename T>
using output_traits = ::mgs::codecs::output_traits<T>;
}
}
}
}
}
}
