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
template <typename T, typename U = T>
class span
{
public:
  span(T begin, U const end) : _begin(begin), _end(end)
  {
  }

  auto& operator[](std::size_t n)
  {
    return _begin[n];
  }

  auto begin()
  {
    return _begin;
  }

  auto begin() const
  {
    return _begin;
  }

  auto end()
  {
    return _end;
  }

  auto end() const
  {
    return _end;
  }

  std::size_t size() const
  {
    return _end - _begin;
  }

  auto const& operator[](std::size_t n) const
  {
    return _begin[n];
  }

private:
  T _begin;
  U _end;
};
}
}
}
}
}
