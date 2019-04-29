#pragma once

namespace mgs
{
namespace binary_to_base
{
namespace detected
{
namespace static_data_members
{
template <typename T>
using padding_policy = decltype(T::padding_policy);
}
}
}
}
