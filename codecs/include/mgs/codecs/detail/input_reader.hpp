#pragma once

#include <algorithm>
#include <type_traits>

#include <mgs/meta/concepts/sized_sentinel_for.hpp>
#include <mgs/meta/ssize_t.hpp>

namespace mgs
{
namespace detail
{
template <typename I, typename S, typename = void>
struct input_reader
{
  template <typename OutputIterator>
  static meta::ssize_t read(I& current,
                            S end,
                            OutputIterator out,
                            meta::ssize_t n)
  {
    while (current != end && n > 0)
    {
      *out = *current++;
      --n;
    }
  }
};

template <typename I, typename S>
struct input_reader<I,
                    S,
                    std::enable_if_t<meta::is_sized_sentinel_for<S, I>::value>>
{
  template <typename OutputIterator>
  static meta::ssize_t read(I& current,
                            S end,
                            OutputIterator out,
                            meta::ssize_t n)
  {
    auto const to_read = std::min<meta::ssize_t>(end - current, n);
    current = std::copy_n(current, n, out);
    return to_read;
  }
};
}
}
