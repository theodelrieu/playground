#pragma once

#include <cstddef>

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
template <typename T>
class span
{
public:
  span(T begin, T const end) : begin(begin), end(end)
  {
  }
  auto& operator[](std::size_t n)
  {
    return begin[n];
  }

  std::size_t size() const
  {
    return end - begin;
  }

  auto const& operator[](std::size_t n) const
  {
    return begin[n];
  }

private:
  T begin;
  T end;
};
}
}
}
}
}
