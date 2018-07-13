#pragma once

#include <cstddef>
#include <utility>

namespace mgs
{
namespace detail
{
// constexpr functions caused apple-clang ICEs...
template <std::size_t N>
constexpr std::size_t log2()
{
  std::size_t ret = 0;
  auto n = N;

  while (n >>= 1)
    ++ret;
  return ret;
}

template <std::size_t X, std::size_t N>
constexpr std::size_t pow()
{
  std::size_t ret = 1;
  auto x = X;
  auto n = N;

  while (n > 0)
  {
    if (n & 1)
      ret *= x;
    n >>= 1;
    x *= x;
  }
  return ret;
}

template <std::size_t A, std::size_t B>
constexpr std::size_t gcd()
{
  auto a = A;
  auto b = B;

  while (b != 0)
  {
    auto const r = a % b;
    a = b;
    b = r;
  }
  return a;
}

template <std::size_t AlphabetSize>
constexpr std::pair<std::size_t, std::size_t> encoding_bytes()
{
  constexpr auto a = log2<AlphabetSize>();
  // log2(256) -- ASCII set
  constexpr auto b = 8;
  static_assert(pow<2, a>() == AlphabetSize,
                "AlphabetSize must be a power of 2");
  constexpr auto divisor = gcd<a, b>();

  return {a / divisor, b / divisor};
}

template <std::size_t AlphabetSize>
constexpr std::size_t decoded_bytes()
{
  return encoding_bytes<AlphabetSize>().first;
}

template <std::size_t AlphabetSize>
constexpr std::size_t encoded_bytes()
{
  return encoding_bytes<AlphabetSize>().second;
}
}
}
