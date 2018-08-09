#pragma once

namespace mgs
{
inline namespace v1
{
namespace binary_to_text
{
namespace aliases
{
namespace static_data_members
{
template <typename T>
using padding_policy = decltype(T::padding_policy);
}
}
namespace static_data_member_aliases = aliases::static_data_members;
}
}
}
