#include <iterator>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/semiregular.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/static_asserts.hpp>
#include <mgs/ranges/concepts/transformed_input_range.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs;
using namespace mgs::ranges;

namespace
{
struct valid_range
{
  using iterator = char const*;
  using underlying_iterator = iterator;
  using underlying_sentinel = iterator;

  valid_range() = default;
  valid_range(underlying_iterator, underlying_sentinel);

  iterator begin();
  iterator end();
};

struct non_semiregular_range
{
  using iterator = char const*;
  using underlying_iterator = iterator;
  using underlying_sentinel = iterator;

  non_semiregular_range(underlying_iterator, underlying_sentinel);

  iterator begin();
  iterator end();
};

struct non_range
{
  using iterator = char const*;
  using underlying_iterator = iterator;
  using underlying_sentinel = iterator;

  non_range() = default;
  non_range(underlying_iterator, underlying_sentinel);

  iterator begin();
};

struct no_underlying_iterator_range
{
  using iterator = char const*;
  using underlying_sentinel = iterator;

  no_underlying_iterator_range() = default;
  no_underlying_iterator_range(iterator, underlying_sentinel);

  iterator begin();
  iterator end();
};

struct no_underlying_sentinel_range
{
  using iterator = char const*;
  using underlying_iterator = iterator;

  no_underlying_sentinel_range() = default;
  no_underlying_sentinel_range(underlying_iterator, iterator);

  iterator begin();
  iterator end();
};

struct non_constructible_from_iterator_sentinel_range
{
  using iterator = char const*;
  using underlying_iterator = iterator;
  using underlying_sentinel = iterator;

  non_constructible_from_iterator_sentinel_range() = default;

  iterator begin();
  iterator end();
};

struct invalid_underlying_sentinel_range
{
  using iterator = char const*;
  using underlying_iterator = iterator;
  using underlying_sentinel = std::istreambuf_iterator<char>;

  invalid_underlying_sentinel_range() = default;
  invalid_underlying_sentinel_range(underlying_iterator, underlying_sentinel);

  iterator begin();
  iterator end();
};

struct invalid_iterator_range
{
  using iterator = std::ostreambuf_iterator<char>;
  using underlying_iterator = iterator;
  using underlying_sentinel = iterator;

  invalid_iterator_range() = default;
  invalid_iterator_range(underlying_iterator, underlying_sentinel);

  iterator begin();
  iterator end();
};
}

TEST_CASE("TransformedInputRange", "[concepts]")
{
  static_assert(is_transformed_input_range<valid_range>::value, "");

  static_assert(!is_transformed_input_range<struct incomplete>::value, "");
  static_assert(!is_transformed_input_range<void>::value, "");
  static_assert(!is_transformed_input_range<void*>::value, "");

  static_assert(!is_transformed_input_range<non_semiregular_range>::value, "");
  static_assert(!is_transformed_input_range<non_range>::value, "");
  static_assert(
      !is_transformed_input_range<no_underlying_iterator_range>::value, "");
  static_assert(
      !is_transformed_input_range<no_underlying_sentinel_range>::value, "");
  static_assert(!is_transformed_input_range<
                    non_constructible_from_iterator_sentinel_range>::value,
                "");
  static_assert(
      !is_transformed_input_range<invalid_underlying_sentinel_range>::value,
      "");
  static_assert(!is_transformed_input_range<invalid_iterator_range>::value, "");
}
