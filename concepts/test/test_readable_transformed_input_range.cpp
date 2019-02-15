#include <iterator>

#include <catch.hpp>

#include <mgs/meta/static_asserts.hpp>
#include <mgs/concepts/readable_transformed_input_range.hpp>

using namespace mgs;

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
  std::size_t read(O, std::size_t);
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

  std::size_t max_transformed_size();
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

struct invalid_read_range
{
  using iterator = char const*;
  using underlying_iterator = iterator;
  using underlying_sentinel = iterator;

  invalid_read_range() = default;
  invalid_read_range(underlying_iterator, underlying_sentinel);

  iterator begin();
  iterator end();

  std::size_t read(char const*, std::size_t);
};
}

TEST_CASE("ReadableTransformedInputRange", "[concepts]")
{
  static_assert(concepts::is_readable_transformed_input_range<valid_range, char*>::value, "");
  static_assert(concepts::is_readable_transformed_input_range<valid_range, std::string::iterator>::value, "");
  // static_assert(concepts::is_readable_transformed_input_range<valid_range, std::back_insert_iterator<std::string>>::value, "");
  // meta::trigger_static_asserts<concepts::is_readable_transformed_input_range<valid_range, std::back_insert_iterator<std::vector<char>>>>();
  // static_assert(!concepts::is_readable_transformed_input_range<valid_range, >::value, "");
  //
  // static_assert(!concepts::is_readable_transformed_input_range<struct incomplete>::value,
  //               "");
  // static_assert(!concepts::is_readable_transformed_input_range<void>::value, "");
  // static_assert(!concepts::is_readable_transformed_input_range<void*>::value, "");
}
