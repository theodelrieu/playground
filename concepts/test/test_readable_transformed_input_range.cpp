#include <iterator>

#include <catch.hpp>

#include <mgs/concepts/readable_transformed_input_range.hpp>
#include <mgs/meta/ssize_t.hpp>
#include <mgs/meta/static_asserts.hpp>

using namespace mgs::concepts;

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

  template <typename O>
  mgs::meta::ssize_t read(O, mgs::meta::ssize_t);
};

struct no_read_range
{
  using iterator = char const*;
  using underlying_iterator = iterator;
  using underlying_sentinel = iterator;

  no_read_range() = default;
  no_read_range(underlying_iterator, underlying_sentinel);

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

  int max_transformed_size() const;
};
}

TEST_CASE("ReadableTransformedInputRange", "[concepts]")
{
  static_assert(is_readable_transformed_input_range<valid_range, char*>::value,
                "");
  static_assert(
      is_readable_transformed_input_range<valid_range,
                                          std::string::iterator>::value,
      "");
  static_assert(is_readable_transformed_input_range<
                    valid_range,
                    std::back_insert_iterator<std::string>>::value,
                "");

  static_assert(
      !is_readable_transformed_input_range<no_read_range, char*>::value, "");
  static_assert(
      !is_readable_transformed_input_range<non_const_range, char*>::value, "");
  static_assert(!is_readable_transformed_input_range<invalid_return_type_range,
                                                     char*>::value,
                "");

  static_assert(!is_readable_transformed_input_range<valid_range, void*>::value,
                "");
  static_assert(!is_readable_transformed_input_range<valid_range,
                                                     struct incomplete*>::value,
                "");
  static_assert(!is_readable_transformed_input_range<void, char*>::value, "");
  static_assert(!is_readable_transformed_input_range<void*, char*>::value, "");
}
