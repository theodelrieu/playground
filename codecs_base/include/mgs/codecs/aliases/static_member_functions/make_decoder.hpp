#pragma once

#include <mgs/meta/detected.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace aliases
{
namespace static_member_functions
{
template <typename T, typename... Args>
using make_decoder = decltype(T::make_decoder(std::declval<Args>()...));
}
}
namespace static_member_function_aliases = aliases::static_member_functions;
}
}
}
