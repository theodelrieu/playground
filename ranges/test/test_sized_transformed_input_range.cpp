#include <iterator>

#include <catch2/catch.hpp>

#include <mgs/meta/ssize_t.hpp>
#include <mgs/ranges/concepts/sized_transformed_input_range.hpp>

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

  mgs::meta::ssize_t max_transformed_size() const;
};

struct no_max_transformed_size_range
{
  using iterator = char const*;
  using underlying_iterator = iterator;
  using underlying_sentinel = iterator;

  no_max_transformed_size_range() = default;
  no_max_transformed_size_range(underlying_iterator, underlying_sentinel);

  iterator begin();
  iterator end();
};

struct non_const_range
{
  using iterator = char const*;
  using underlying_iterator = iterator;
  using underlying_sentinel = iterator;

  non_const_range() = default;
  non_const_range(underlying_iterator, underlying_sentinel);

  iterator begin();
  iterator end();

  mgs::meta::ssize_t max_transformed_size();
};

struct invalid_return_type_range
{
  using iterator = char const*;
  using underlying_iterator = iterator;
  using underlying_sentinel = iterator;

  invalid_return_type_range() = default;
  invalid_return_type_range(underlying_iterator, underlying_sentinel);

  iterator begin();
  iterator end();

  char* max_transformed_size() const;
};
}

TEST_CASE("SizedTransformedInputRange", "[concepts]")
{
  static_assert(is_sized_transformed_input_range<valid_range>::value, "");

  static_assert(!is_sized_transformed_input_range<struct incomplete>::value,
                "");
  static_assert(!is_sized_transformed_input_range<void>::value, "");
  static_assert(!is_sized_transformed_input_range<void*>::value, "");

  static_assert(
      !is_sized_transformed_input_range<invalid_return_type_range>::value, "");
  static_assert(!is_sized_transformed_input_range<non_const_range>::value, "");
  static_assert(
      !is_sized_transformed_input_range<no_max_transformed_size_range>::value,
      "");
}
