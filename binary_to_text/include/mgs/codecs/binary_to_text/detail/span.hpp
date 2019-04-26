#pragma once

#include <cstddef>

#include <mgs/meta/ssize_t.hpp>

namespace mgs
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

  auto& operator[](meta::ssize_t n)
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

  meta::ssize_t size() const
  {
    return _end - _begin;
  }

  auto const& operator[](meta::ssize_t n) const
  {
    return _begin[n];
  }

private:
  T _begin;
  U _end;
};
}
}
