#pragma once

namespace mgs
{
namespace codecs
{
namespace binary_to_text
{
namespace detail
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
}
}
