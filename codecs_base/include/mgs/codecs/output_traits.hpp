#pragma once

#include <mgs/codecs/output_traits_fwd.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
// TODO namespace detail, support types by default (Containers + std::array
// special case)
template <typename T>
struct output_traits<T>
{
  template <typename Iterator, typename Sentinel>
  static T create(Iterator it, Sentinel end)
  {
    // TODO static_assert iterator/sentinel
    return {};
  }
};
}
}
}
