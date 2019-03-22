#pragma once

#include <cstddef>
#include <utility>

#include <mgs/meta/ssize_t.hpp>

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
template <meta::ssize_t A, meta::ssize_t Multiple>
constexpr meta::ssize_t round_to_multiple_of()
{
  return ((A + (Multiple / 2)) / Multiple) * Multiple;
}

template <meta::ssize_t N>
constexpr meta::ssize_t log2()
{
  meta::ssize_t ret = 0;
  auto n = N;

  while (n >>= 1)
    ++ret;
  return ret;
}

template <meta::ssize_t N>
constexpr meta::ssize_t is_power_of_2()
{
  return N != 0 && ((N & (N - 1)) == 0);
}

template <meta::ssize_t X, meta::ssize_t N>
constexpr meta::ssize_t pow()
{
  meta::ssize_t ret = 1;
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

template <meta::ssize_t A, meta::ssize_t B>
constexpr meta::ssize_t gcd()
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

template <meta::ssize_t AlphabetSize>
constexpr std::pair<meta::ssize_t, meta::ssize_t> encoding_bytes()
{
  constexpr auto a = log2<AlphabetSize>();
  // log2(256) -- ASCII set
  constexpr auto b = 8;
  constexpr auto divisor = gcd<a, b>();

  return {a / divisor, b / divisor};
}

template <meta::ssize_t AlphabetSize>
constexpr meta::ssize_t decoded_bytes()
{
  return encoding_bytes<AlphabetSize>().first;
}

template <meta::ssize_t AlphabetSize>
constexpr meta::ssize_t encoded_bytes()
{
  return encoding_bytes<AlphabetSize>().second;
}
}
}
}
}
}
