#pragma once

namespace mgs
{
inline namespace v1
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
using encoding_name = decltype(T::encoding_name);
}
}
}
}
}
}
}
