#pragma once

#include <iosfwd>

#include <catch.hpp>

#include <mgs/meta/concepts/iterator/sentinel.hpp>

namespace test_helpers
{
namespace detail
{
struct stream_sentinel
{
};

template <typename CharT, typename Traits>
bool operator==(std::istreambuf_iterator<CharT, Traits> lhs, stream_sentinel)
{
  return lhs.equal({});
}

template <typename CharT, typename Traits>
bool operator==(stream_sentinel, std::istreambuf_iterator<CharT, Traits> rhs)
{
  return rhs.equal({});
}

template <typename CharT, typename Traits>
bool operator!=(std::istreambuf_iterator<CharT, Traits> lhs, stream_sentinel s)
{
  return !(lhs == s);
}

template <typename CharT, typename Traits>
bool operator!=(stream_sentinel s, std::istreambuf_iterator<CharT, Traits> rhs)
{
  return !(s == rhs);
}

static_assert(mgs::meta::concepts::iterator::is_sentinel<
                  stream_sentinel,
                  std::istreambuf_iterator<char>>::value,
              "");

static_assert(!mgs::meta::concepts::iterator::is_sentinel<
                  stream_sentinel,
                  std::vector<char>::iterator>::value,
              "");
}

// inception
// back and forth

}
