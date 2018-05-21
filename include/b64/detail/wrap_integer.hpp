#pragma once

#include <cstdlib>

#include <type_traits>

namespace b64
{
namespace detail
{
template <int Low, int High>
constexpr int wrap_assign(int n)
{
  static_assert(Low < High, "");
  auto constexpr nb_values = High - Low + 1;
  auto const m = (n - Low) % (High + 1 - Low);
  if (m < 0)
    return m + nb_values + Low;
  return m + Low;
}

template <int, int, typename = void>
class wrap_integer;

template <int Low, int High>
class wrap_integer<Low, High, std::enable_if_t<(Low < High)>>
{
public:
  constexpr wrap_integer() = default;
  constexpr wrap_integer(int n) noexcept
  {
    _val = wrap_assign<Low, High>(n);
  }

  constexpr wrap_integer& operator=(int n) noexcept
  {
    _val = wrap_assign<Low, High>(n);
    return *this;
  }

  constexpr wrap_integer& operator+=(int n) noexcept
  {
    _val = wrap_assign<Low, High>(_val + n);
    return *this;
  }

  constexpr wrap_integer& operator-=(int n) noexcept
  {
    return *this += -n;
  }

  constexpr wrap_integer& operator++() noexcept
  {
    return *this += 1;
  }

  constexpr wrap_integer operator++(int) noexcept
  {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  constexpr wrap_integer& operator--() noexcept
  {
    return *this += -1;
  }

  constexpr wrap_integer operator--(int) noexcept
  {
    auto tmp = *this;
    --(*this);
    return tmp;
  }

  constexpr operator int() const noexcept
  {
    return _val;
  }

private:
  int _val{Low};
};
}
}
