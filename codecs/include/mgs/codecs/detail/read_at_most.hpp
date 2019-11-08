#pragma once

#include <utility>

#include <mgs/meta/ssize_t.hpp>

namespace mgs
{
namespace detail
{
template <typename IS, typename O>
std::pair<O, meta::ssize_t> read_at_most(IS& is, O o, meta::ssize_t n)
{
  auto total_read = 0;
  while (n != 0)
  {
    auto const res = is.read(o, n);
    o = res.first;
    if (res.second == 0)
      break;
    total_read += res.second;
    n -= res.second;
  }
  return {o, total_read};
}
}
}
