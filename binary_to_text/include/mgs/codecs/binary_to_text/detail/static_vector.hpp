#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <type_traits>

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
// a very minimalistic static_vector...
template <typename T, std::size_t N>
class static_vector
{
  static_assert(std::is_integral<T>::value, "");
  static_assert(sizeof(T) == 1, "");
  static_assert(N > 0 && N <= 256, "");

public:
  using value_type = T;

  void push_back(T c)
  {
    assert(_index + 1 <= N);
    _array[_index++] = c;
  }

  std::size_t size() const
  {
    return _index;
  }

  void resize(std::size_t n)
  {
    assert(n <= N);
    _index = n;
  }

  T& operator[](std::size_t n)
  {
    assert(n <= _index);
    return _array[n];
  }

  T const& operator[](std::size_t n) const
  {
    assert(n <= _index);
    return _array[n];
  }

  T* begin()
  {
    return &_array[0];
  }

  T const* begin() const
  {
    return &_array[0];
  }

  T* data() 
  {
    return begin();
  }

  T const* data() const
  {
    return begin();
  }

  T* end()
  {
    return &_array[0] + _index;
  }

  T const* end() const
  {
    return &_array[0] + _index;
  }

private:
  std::array<T, N> _array;
  std::size_t _index{0};
};
}
}
}
}
}
