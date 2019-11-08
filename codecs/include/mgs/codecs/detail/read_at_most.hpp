#pragma once

#include <mgs/meta/ssize_t.hpp>

namespace mgs
{
namespace detail
{
template <typename IS, typename O>
meta::ssize_t read_at_most(IS& is, O o, meta::ssize_t n)
{
  auto total_read = 0;
  while (n != 0)
  {
    auto const nb_read = is.read(o, n);
    if (nb_read == 0)
      break;
    total_read += nb_read;
    n -= nb_read;
  }
  return total_read;
}
}
}
